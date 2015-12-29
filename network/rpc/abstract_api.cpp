#include "abstract_api.h"
#include <QDebug>

namespace sn{
namespace corelib{
namespace network{

AbstractApi::AbstractApi(ApiProvider &provider)
{}

void AbstractApi::notifySocketDisconnect(int socketDescriptor)
{
   qDebug() << "asdas";
}

}//network
}//corelib
}//sn