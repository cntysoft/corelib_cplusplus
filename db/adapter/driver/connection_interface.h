#ifndef SN_CORELIB_DB_ADAPTER_DRIVER_CONNECTION_INTERFACE_H
#define SN_CORELIB_DB_ADAPTER_DRIVER_CONNECTION_INTERFACE_H

#include <QString>

#include "global/global.h"
#include "result_interface.h"

namespace sn{
namespace corelib{
namespace db{

class SN_CORELIB_EXPORT ConnectionInterface
{
public:
   virtual const QString& getCurrentSchema() = 0;
   virtual void getResource() = 0;
   virtual ConnectionInterface& connect() = 0;
   virtual bool isConnected() = 0;
   virtual ConnectionInterface& disconnect() = 0;
   virtual ConnectionInterface& beginTransaction() = 0;
   virtual ConnectionInterface& commit() = 0;
   virtual ConnectionInterface& rollback() = 0;
   virtual ResultInterface execute() = 0;
   virtual int getLastGeneratedValue(const QString &name = QString()) = 0;
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ADAPTER_DRIVER_CONNECTION_INTERFACE_H
