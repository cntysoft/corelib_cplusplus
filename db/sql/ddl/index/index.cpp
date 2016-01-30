#include "index.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace index{

Index::Index(const QString &column, const QString &name, const QStringList &lengths)
   : AbstractConstraint(QStringList{column}, name),
     m_lengths(lengths)
{
   m_specification = "INDEX %s(...)";
}

AbstractConstraint::ExpressionDataType Index::getExpressionData()const
{
   int colCount = m_columns.size();
   QStringList values({
                         m_name.isEmpty() ? m_name : ""
                      });
   QStringList newSpecTypes{
      AbstractExpression::TYPE_IDENTIFIER
   };
   QStringList newSpecParts;
   for(int i = 0; i < colCount; i++){
      QString specPart("%s");
      if(i < m_lengths.size() && !m_lengths[i].isEmpty()){
         specPart += QString("(%1)").arg(m_lengths[i]);
      }
      newSpecParts.append(specPart);
      newSpecTypes.append(AbstractExpression::TYPE_IDENTIFIER);
   }
   values.append(m_columns);
   QString spec(m_specification);
   return QList<QVariant>{
      QList<QVariant>{
         spec.replace("...", 
                      newSpecParts.join(", ")),
               values,newSpecTypes
      }
   };
}

}//index
}//ddl
}//sql
}//db
}//corelib
}//sn
