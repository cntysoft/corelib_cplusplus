#ifndef SN_CORELIB_DB_ENGINE_STATEMENT_CONTAINER_H
#define SN_CORELIB_DB_ENGINE_STATEMENT_CONTAINER_H

#include "interface.h"
#include "parameter_container.h"

namespace sn{
namespace corelib{
namespace db{
namespace engine{

class SN_CORELIB_EXPORT StatementContainer : public StatementContainerInterface
{
public:
   StatementContainer(const QString &sql = QString(), const ParameterContainer &parameterContainer = ParameterContainer());
   virtual StatementContainer& setSql(const QString &sql);
   virtual const QString& getSql();
   virtual StatementContainer& setParameterContainer(const ParameterContainer &paramContainer);
   virtual ParameterContainer& getParameterConatiner();
protected:
   QString m_sql;
   ParameterContainer m_parameterContainer;
};

}//engine
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ENGINE_STATEMENT_CONTAINER_H
