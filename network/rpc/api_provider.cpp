#include "api_provider.h"

namespace sn{
namespace corelib{
namespace network{

ApiProvider* ApiProvider::sm_self = nullptr;

ApiProvider::ApiProvider()
{}

ApiProvider& ApiProvider::instance()
{
   if(nullptr == ApiProvider::sm_self){
      ApiProvider::sm_self = new ApiProvider;
   }
   return *ApiProvider::sm_self;
}

ApiProvider& ApiProvider::setUnderlineSocket(QTcpSocket* socket)
{
   m_socket = socket;
   return *this;
}

ApiProvider& ApiProvider::addApiToPool(const QString &key, ApiInitializerType initializerFn)
{
   if(!m_apiIntializerPool.contains(key)){
      m_apiIntializerPool.insert(key, initializerFn);
   }
   return *this;
}

}//network
}//corelib
}//sn