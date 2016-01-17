#ifndef SN_CORELIB_DB_ADAPTER_ADAPTER_INTERFACE_H
#define SN_CORELIB_DB_ADAPTER_ADAPTER_INTERFACE_H

#include "kernel/errorinfo.h"
#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{

class SN_CORELIB_EXPORT AdapterInterface
{
public:
   virtual void getDriver() = 0;
   virtual void getPlatform() = 0;
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ADAPTER_ADAPTER_INTERFACE_H
