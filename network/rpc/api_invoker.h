#ifndef SN_CORELIB_NETWORK_RPC_API_INVOKER_H
#define SN_CORELIB_NETWORK_RPC_API_INVOKER_H

#include <QObject>

#include "invoke_meta.h"
#include "global/global.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace sn{
namespace network{

class SN_CORELIB_EXPORT ApiInvoker : public QObject
{
   Q_OBJECT
public:
   ApiInvoker(const QTcpSocket& socket);
   bool request(const ApiInvokeRequest& request);
   ApiInvokeResponse requestSync(const ApiInvokeRequest& request);
protected:
   const QTcpSocket &m_socket;
};

}//network
}//sn

#endif // SN_CORELIB_NETWORK_RPC_API_INVOKER_H
