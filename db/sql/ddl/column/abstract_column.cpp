#include <algorithm>

#include "abstract_column.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace column {

AbstractColumn::AbstractColumn(const QString &name, bool nullable, const QVariant &defaultValue, 
                               const QMap<QString, QVariant> &options)
   : m_default(defaultValue),
     m_isNullable(nullable),
     m_name(name),
     m_options(options),
     m_specification("%s %s"),
     m_type("INTEGER")
{
}

AbstractColumn& AbstractColumn::setName(const QString &name)
{
   m_name = name;
   return *this;
}

const QString& AbstractColumn::getName()const
{
   return m_name;
}

AbstractColumn& AbstractColumn::setNullable(bool flag)
{
   m_isNullable = flag;
   return *this;
}

bool AbstractColumn::isNullable()const
{
   return m_isNullable;
}

AbstractColumn& AbstractColumn::setDefault(const QVariant &defaultValue)
{
   m_default = defaultValue;
   return *this;
}

const QVariant& AbstractColumn::getDefault() const
{
   return m_default;
}

AbstractColumn& AbstractColumn::setOptions(const QMap<QString, QVariant> &options)
{
   m_options.clear();
   m_options = options;
   return *this;
}

const QMap<QString, QVariant>& AbstractColumn::getOptions() const
{
   return m_options;
}

AbstractColumn& AbstractColumn::addConstraint(const QSharedPointer<AbstractConstraint> &constraint)
{
   m_constraints.append(constraint);
   return *this;
}

AbstractExpression::ExpressionDataType AbstractColumn::getExpressionData()const
{
   QString spec(m_specification);
   QList<QVariant> params;
   params.append(m_name);
   params.append(m_type);
   QList<QVariant> types{
      AbstractColumn::TYPE_IDENTIFIER, AbstractColumn::TYPE_LITERAL
   };
   if(!m_isNullable){
      spec += " NOT NULL";
   }
   if(!m_default.isNull()){
      spec += " DEFAULT %s";
      params.append(m_default);
      types.append(AbstractExpression::TYPE_VALUE);
   }
   QList<QVariant> data{
      QList<QVariant>{
         spec, params, types
      }
   };
   std::for_each(m_constraints.cbegin(), m_constraints.cend(), [&data](const QSharedPointer<AbstractConstraint> &constraint){
      data.append(QVariant(" "));
      data.append(constraint->getExpressionData());
   });
   return data;
}

AbstractLengthColumn::AbstractLengthColumn(const QString &name, const QVariant &length, bool nullable, const QVariant &defaultValue, const QMap<QString, QVariant> &options)
   : AbstractColumn(name, nullable, defaultValue, options),
     m_length(length)
{
}

AbstractLengthColumn& AbstractLengthColumn::setLength(int length)
{
   m_length.setValue(length);
   return *this;
}

int AbstractLengthColumn::getLength()const
{
   return m_length.toInt();
}

QString AbstractLengthColumn::getLengthExpression()const
{
   return QString("%1").arg(m_length.toInt());
}

AbstractExpression::ExpressionDataType AbstractLengthColumn::getExpressionData() const
{
   ExpressionDataType data = AbstractColumn::getExpressionData();
   QString lengthExpr(getLengthExpression());
   if(!lengthExpr.isEmpty()){
      QList<QVariant> targetList = data[0].toList();
      QStringList params = targetList[1].value<QStringList>();\
      targetList[1] = QVariant::fromValue(params);
      params[1] += "(" + getLengthExpression() + ")";
      data[0] = QVariant::fromValue(targetList);
   }
   return data;
}

AbstractExpression::ExpressionDataType AbstractTimestampColumn::getExpressionData()const
{
   QString spec(m_specification);
   QStringList params{
      m_name, m_type 
   };
   QStringList types{
       AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_LITERAL
   };
   if(!m_isNullable){
      spec += " NOT NULL";
   }
   if(!m_default.isNull()){
      spec += " DEFAULT %s";
      params.append(m_default.toString());
      types.append(AbstractExpression::TYPE_VALUE);
   }
   if(m_options.contains("on_update")){
      spec += " %s";
      params.append("ON UPDATE CURRENT_TIMESTAMP");
      types.append(AbstractExpression::TYPE_LITERAL);
   }
   QList<QVariant> data{
      QList<QVariant>{
         spec, params, types
      }
   };
   std::for_each(m_constraints.cbegin(), m_constraints.cend(), [&data](const QSharedPointer<AbstractConstraint> &constraint){
      data.append(QVariant(" "));
      data.append(constraint->getExpressionData());
   });
   return data;
}

AbstractPrecisionColumn::AbstractPrecisionColumn(const QString &name, const QVariant & digits, const QVariant & decimal, 
                                                 bool nullable, const QVariant &defaultValue, 
                                                 const QMap<QString, QVariant> &options)
   : AbstractLengthColumn(name, digits, nullable, defaultValue, options),
     m_decimal(decimal)
{
   
}

AbstractPrecisionColumn& AbstractPrecisionColumn::setDigits(int digits)
{
   setLength(digits);
   return *this;
}

int AbstractPrecisionColumn::getDigits()const
{
   return getLength();
}

int AbstractPrecisionColumn::getDecimal()const
{
   return m_decimal.toInt();
}

AbstractPrecisionColumn& AbstractPrecisionColumn::setDecimal(int decimal)
{
   m_decimal.setValue(decimal);
   return *this;
}

QString AbstractPrecisionColumn::getLengthExpression()const
{
   if(!m_decimal.isNull()){
      return QString("%1, %2").arg(m_length.toInt(), m_decimal.toInt());
   }
   return QString("%1").arg(m_length.toInt());
}

AbstractTimestampColumn::AbstractTimestampColumn(const QString &name, bool nullable, const QVariant &defaultValue, 
                                                 const QMap<QString, QVariant> &options)
   : AbstractColumn(name, nullable, defaultValue, options)
{
   
}

}//column
}//ddl
}//sql
}//db
}//corelib
}//sn