#ifndef SN_CORELIB_DB_ADAPTER_DRIVER_RESULT_INTERFACE_H
#define SN_CORELIB_DB_ADAPTER_DRIVER_RESULT_INTERFACE_H

#include <QString>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{

class SN_CORELIB_EXPORT ResultInterface
{
public:
   virtual void buffer() = 0;
   virtual bool isBuffered() = 0;
   virtual bool isQueryResult() = 0;
   virtual int getAffectedRows() = 0;
   virtual void getGeneratedValue() = 0;
   virtual void getResource() = 0;
   virtual int getFieldCount() = 0;
   virtual ~ResultInterface() = 0;
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ADAPTER_DRIVER_RESULT_INTERFACE_H

