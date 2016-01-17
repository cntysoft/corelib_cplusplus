#ifndef SN_CORELIB_DB_ADAPTER_DRIVER_STATEMENT_INTERFACE_H
#define SN_CORELIB_DB_ADAPTER_DRIVER_STATEMENT_INTERFACE_H

#include <QString>

#include "global/global.h"
#include "result_interface.h"
#include "db/adapter/parameter_container.h"

namespace sn{
namespace corelib{
namespace db{

class SN_CORELIB_EXPORT StatementInterface
{
public:
   virtual void getResource() = 0;
   virtual void prepare(const QString &sql = QString()) = 0;
   virtual bool isPrepared() = 0;
   virtual ResultInterface execute(const ParameterContainer &parameters = ParameterContainer()) = 0; 
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ADAPTER_DRIVER_STATEMENT_INTERFACE_H
