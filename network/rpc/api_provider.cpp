#include "api_provider.h"

namespace sn{
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

}//network
}//sn