#include "abstract_service.h"
#include "invoke_meta.h"

#include <QDebug>

namespace sn{
namespace corelib{
namespace network{

AbstractService::AbstractService(ServiceProvider &provider)
   : m_serviceProvider(provider)
{}

void AbstractService::notifySocketDisconnect(int)
{
}

void AbstractService::writeInterResponse(int socketNum, ServiceInvokeResponse &response)
{
   response.setIsFinal(false);
   m_serviceProvider.writeResponseToSocket(socketNum, response);
}

AbstractService::~AbstractService()
{
}

}//network
}//corelib
}//sn