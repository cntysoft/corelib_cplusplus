#include "api_invoker.h"

namespace sn{
namespace network{

ApiInvoker::ApiInvoker(const QTcpSocket &socket)
   :m_socket(socket)
{
}

ApiInvokeResponse ApiInvoker::requestSync(const ApiInvokeRequest& request)
{
   
}

}//network
}//sn