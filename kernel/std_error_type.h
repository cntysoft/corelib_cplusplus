#ifndef SN_CORELIB_GLOBAL_STD_ERROR_TYPE_H
#define SN_CORELIB_GLOBAL_STD_ERROR_TYPE_H

#include <QMap>
#include <QStringList>

#include "global/global.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{

class SN_CORELIB_EXPORT StdErrorType
{
public:
   static QString msg(int errorCode, const QStringList &args = QStringList())throw(ErrorInfo); 
protected:
   static const QMap<int, QString> sm_map;
};

}//corelib
}//sn

#endif // SN_CORELIB_GLOBAL_STD_ERROR_TYPE_H
