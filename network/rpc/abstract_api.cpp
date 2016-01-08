#include "abstract_api.h"
#include "invoke_meta.h"

#include <QDebug>

namespace sn{
namespace corelib{
namespace network{

AbstractApi::AbstractApi(ApiProvider &provider)
   : m_apiProvider(provider)
{}

void AbstractApi::notifySocketDisconnect(int)
{
}

void AbstractApi::writeInterResponse(int socketNum, ApiInvokeResponse &response)
{
   response.setIsFinal(false);
   m_apiProvider.writeResponseToSocket(socketNum, response);
}

AbstractApi::~AbstractApi()
{
}

}//network
}//corelib
}//sn