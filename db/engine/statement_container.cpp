#include "statement_container.h"

namespace sn{
namespace corelib{
namespace db{
namespace engine{

StatementContainer::StatementContainer(const QString &sql, const ParameterContainer &parameterContainer)
   : m_parameterContainer(parameterContainer)
{
   setSql(sql);
}

StatementContainer& StatementContainer::setSql(const QString &sql)
{
   if(!sql.isNull()){
      m_sql = sql;
   }
   return *this;
}

const QString& StatementContainer::getSql()
{
   return m_sql;
}

StatementContainer& StatementContainer::setParameterContainer(const ParameterContainer &paramContainer)
{
   m_parameterContainer = paramContainer;
   return *this;
}

ParameterContainer& StatementContainer::getParameterConatiner()
{
   return m_parameterContainer;
}

}//engine
}//db
}//corelib
}//sn