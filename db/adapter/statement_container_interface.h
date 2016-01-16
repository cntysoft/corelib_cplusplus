#ifndef SN_CORELIB_DB_ADAPTER_STATEMENT_CONTAINER_INTERFACE_H  
#define SN_CORELIB_DB_ADAPTER_STATEMENT_CONTAINER_INTERFACE_H

#include <QString>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{

class ParameterContainer;

class SN_CORELIB_EXPORT StatementContainerInterface
{
public:
   virtual void setSql() = 0;
   virtual void getSql() = 0;
   virtual void setParameterContainer(const ParameterContainer &container);
   virtual void getParameterContainer();
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ADAPTER_STATEMENT_CONTAINER_INTERFACE_H

