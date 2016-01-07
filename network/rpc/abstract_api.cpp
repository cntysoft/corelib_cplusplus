#include "abstract_api.h"
#include <QDebug>

namespace sn{
namespace corelib{
namespace network{

AbstractApi::AbstractApi(ApiProvider &provider)
   : m_apiProvider(provider)
{}

void AbstractApi::notifySocketDisconnect(int socketDescriptor)
{
}

AbstractApi::~AbstractApi()
{
   
}

}//network
}//corelib
}//sn