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

void AbstractService::writeInterResponse(const ServiceInvokeRequest &request, ServiceInvokeResponse &response)
{
   response.setIsFinal(false);
   m_serviceProvider.writeResponseToSocket(request, response);
}

AbstractService::~AbstractService()
{
}

}//network
}//corelib
}//sn