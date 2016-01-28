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
     m_specification("%1 %2"),
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
   QStringList params;
   params.append(m_name);
   params.append(m_type);
   QStringList types{
      AbstractColumn::TYPE_IDENTIFIER, AbstractColumn::TYPE_LITERAL
   };
   if(!m_isNullable){
      spec += " NOT NULL";
   }
   if(!m_default.isNull()){
      spec += " DEFAULT %3";
   }
   QList<QVariant> data{
      QList<QVariant>{
         spec, params, types
      }
   };
   
}

}//column
}//ddl
}//sql
}//db
}//corelib
}//sn