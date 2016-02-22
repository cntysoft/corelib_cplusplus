#include "std_error_type.h"
#include "global/std_error_code.h"
#include "global/common_funcs.h"

namespace sn{
namespace corelib{

const QMap<int, QString> StdErrorType::sm_map{
   {SN_E_FILE_NOT_EXIST, "file : %1 is not exist"},
   {SN_E_DIR_NOT_EXIST , "directory : %1 is not exist"},
   {SN_E_API_INVOKE_LEAK_ARGS, "API Invoke need param %1"},
   {SN_E_PROTOCOL_ILL_FORMAT, "protocol error"}
};

QString StdErrorType::msg(int errorCode, const QStringList &args)throw(ErrorInfo)
{
   if(!StdErrorType::sm_map.contains(errorCode)){
      throw ErrorInfo(QString("ERROR type %1 is not exist").arg(errorCode));
   }
   return format_str(sm_map[errorCode], args);
}

}//corelib
}//sn