#ifndef SN_CORELIB_DB_ADAPTER_DRIVER_DRIVER_INTERFACE_H 
#define SN_CORELIB_DB_ADAPTER_DRIVER_DRIVER_INTERFACE_H

#include <QString>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{

class SN_CORELIB_EXPORT DriverInterface
{
public:
   const static QString PARAMETERIZATION_POSITIONAL = "positional";
   const static QString PARAMETERIZATION_NAMED = "named";
   const static QString NAME_FORMAT_CAMELCASE = "camelCase";
   const static QString NAME_FORMAT_NATURAL = "natural";
public:
   virtual QString getDatabasePlatformName(nameFormat = NAME_FORMAT_CAMELCASE) = 0;
   virtual bool checkEnvironment() = 0;
   virtual void getConnection() = 0;
   virtual void createStatement(const QString &sql) = 0;
   virtual void createResult() = 0;
   virtual void getPrepareType() = 0;
   virtual void formatParameterName() = 0;
   virtual void getLastGeneratedValue() = 0;
   virtual ~DriverInterface() = 0;
};

}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ADAPTER_DRIVER_DRIVER_INTERFACE_H

