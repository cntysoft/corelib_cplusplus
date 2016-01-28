#include "abstract_constraint.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace constraint{

AbstractConstraint::AbstractConstraint(const QStringList &columns, const QString &name)
   : m_columnSpecification(" (%s)"),
     m_namedSpecification("CONSTRAINT %s "),
     m_name(name),
     m_columns(columns)
{
   
}

AbstractConstraint& AbstractConstraint::setName(const QString &name)
{
   m_name = name;
   return *this;
}

const QString& AbstractConstraint::getName() const
{
   return m_name;
}

AbstractConstraint& AbstractConstraint::setColumns(const QStringList &columns)
{
   m_columns = columns;
   return *this;
}

const QStringList& AbstractConstraint::getColumns() const
{
   return m_columns;
}

AbstractConstraint& AbstractConstraint::addColumn(const QString &column)
{
   m_columns.append(column);
   return *this;
}

AbstractExpression::ExpressionDataType AbstractConstraint::getExpressionData()const
{
   int colCount = m_columns.size();
   QStringList newSpecTypes;
   QStringList values;
   QString newSpec;
   if(!m_name.isEmpty()){
      newSpec += m_namedSpecification;
      values.append(m_name);
      newSpecTypes.append(AbstractConstraint::TYPE_IDENTIFIER);
   }
   newSpec += m_specification;
   if(colCount > 0){
      values.append(m_columns);
      QStringList newSpecParts;
      for(int i = 0; i < colCount; i++){
         newSpecParts.append("%s");
         newSpecTypes.append(AbstractExpression::TYPE_IDENTIFIER);
      }
      newSpec += QString(m_columnSpecification).arg(newSpecParts.join(", "));
   }
   return QList<QVariant>{
      QList<QVariant>{
         newSpec, values, newSpecTypes
      }
   };
}

AbstractConstraint::~AbstractConstraint()
{}

}//constraint
}//ddl
}//sql
}//db
}//corelib
}//sn