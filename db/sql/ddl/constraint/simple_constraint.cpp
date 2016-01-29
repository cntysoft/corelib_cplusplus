#include "simple_constraint.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace constraint{

Check::Check(const QString &expression, const QString &name)
   : Check(QVariant(expression), name)
{
}

Check::Check(const QSharedPointer<AbstractExpression> expression, const QString &name)
   : Check(QVariant::fromValue(expression), name)
{}

Check::Check(const QVariant &expression, const QString &name)
   : m_expression(expression)
{
   m_name = name;
   m_specification = "CHECK (%s)";
}

AbstractExpression::ExpressionDataType Check::getExpressionData()const
{
   QStringList newSpecTypes{
      AbstractExpression::TYPE_LITERAL 
   };
   QList<QVariant> values{
      m_expression
   };
   QString newSpec;
   if(!m_name.isEmpty()){
      newSpec += m_namedSpecification;
      values.prepend(m_name);
      newSpecTypes.prepend(AbstractExpression::TYPE_IDENTIFIER);
   }
   return QList<QVariant>{
      QList<QVariant>{
         newSpec + m_specification, values, newSpecTypes
      }
   };
}

PrimaryKey::PrimaryKey(const QStringList &columns, const QString &name)
   : AbstractConstraint(columns, name)
{
   m_specification = "PRIMARY KEY";
}

UniqueKey::UniqueKey(const QStringList &columns, const QString &name)
   : AbstractConstraint(columns, name)
{
   m_specification = "UNIQUE";
}

ForeignKey::ForeignKey(const QString &name, const QStringList columns, 
                       const QString &referenceTable, const QStringList &referenceColumn, 
                       const QString &onDeleteRule, const QString &onUpdateRule)
   : AbstractConstraint(columns, name),
     m_onDeleteRule(onDeleteRule),
     m_onUpdateRule(onUpdateRule),
     m_referenceColumn(referenceColumn),
     m_referenceTable(referenceTable),
     m_referenceSpecification({"REFERENCES %s ", "ON DELETE %s ON UPDATE %s"})
{
   m_columnSpecification = "FOREIGN KEY (%s) ";
}

ForeignKey& ForeignKey::setReferenceTable(const QString &referenceTable)
{
   m_referenceTable = referenceTable;
   return *this;
}

const QString& ForeignKey::getReferenceTable() const
{
   return m_referenceTable;
}

ForeignKey& ForeignKey::setReferenceColumn(const QStringList &referenceColumn)
{
   m_referenceColumn = referenceColumn;
   return *this;
}

const QStringList& ForeignKey::getReferenceColumn() const
{
   return m_referenceColumn;
}


ForeignKey& ForeignKey::setOnDeleteRule(const QString &onDeleteRule)
{
   m_onDeleteRule = onDeleteRule;
   return *this;
}

const QString& ForeignKey::getOnDeleteRule() const
{
   return m_onDeleteRule;
}

ForeignKey& ForeignKey::setOnUpdateRule(const QString &onUpdateRule)
{
   m_onUpdateRule = onUpdateRule;
   return *this;
}

const QString& ForeignKey::getOnUpdateRule() const
{
   return m_onUpdateRule;
}

AbstractExpression::ExpressionDataType ForeignKey::getExpressionData()const
{
   ExpressionDataType data = AbstractConstraint::getExpressionData();
   int colCount     = m_referenceColumn.size();
   QStringList newSpecTypes{
      AbstractExpression::TYPE_IDENTIFIER
   };
   QStringList values{
      m_referenceTable
   };
   QList<QVariant> targetData = data[0].toList();
   QString spec = targetData[0].toString();
   spec += m_referenceSpecification[0];
   if(colCount > 0){
      values.append(m_referenceColumn);
      QStringList newSpecParts;
      for(int i = 0; i < colCount; i++){
         newSpecParts.append("%s");
         newSpecTypes.append(AbstractExpression::TYPE_IDENTIFIER);
      }
      spec += QString("(%1)").arg(newSpecParts.join(", "));
   }
   spec += m_referenceSpecification[1];
   values.append(m_onDeleteRule);
   values.append(m_onUpdateRule);
   newSpecTypes.append(AbstractExpression::TYPE_LITERAL);
   newSpecTypes.append(AbstractExpression::TYPE_LITERAL);
   QStringList targetValues = targetData[1].toStringList();
   QStringList targetTypes = targetData[2].toStringList();
   targetValues.append(values);
   targetTypes.append(newSpecTypes);
   targetData[0] = QVariant(spec);
   targetData[1] = QVariant(targetValues);
   targetData[2] = QVariant(targetTypes);
   data[0] = QVariant(targetData);
   return data;
}

}//constraint
}//ddl
}//sql
}//db
}//corelib
}//sn