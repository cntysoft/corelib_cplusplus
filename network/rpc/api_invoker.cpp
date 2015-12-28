#include <QTcpSocket>

#include "api_invoker.h"

#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{
namespace network{

using sn::corelib::ErrorInfo;

ApiInvoker::ApiInvoker(const QTcpSocket &socket)
   :m_socket(socket)
{
   if(!m_socket.isValid()){
      throw ErrorInfo("socket is invalid");
   }
}

ApiInvokeResponse ApiInvoker::requestSync(const ApiInvokeRequest& request)
{
   
}

}//network
}//corelib
}//sn