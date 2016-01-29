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

}//constraint
}//ddl
}//sql
}//db
}//corelib
}//sn