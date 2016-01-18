#ifndef SN_CORELIB_DB_ENGINE_INTERFACE_H
#define SN_CORELIB_DB_ENGINE_INTERFACE_H

#include "global/global.h"
#include "parameter_container.h"

namespace sn{
namespace corelib{
namespace db{
namespace engine{

class SN_CORELIB_EXPORT StatementContainerInterface
{
public:
   virtual StatementContainerInterface& setSql(const QString &sql) = 0;
   virtual const QString& getSql() = 0;
   virtual StatementContainerInterface& setParameterContainer(const ParameterContainer &paramContainer) = 0;
   virtual ParameterContainer& getParameterConatiner() = 0;
};


}//engine
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ENGINE_INTERFACE_H
