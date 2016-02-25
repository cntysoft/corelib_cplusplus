#ifndef SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_H
#define SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_H

#include <QObject>
#include <QVariant>
#include <QByteArray>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QEventLoop>

#include "global/global.h"
#include "network/rpc/service_provider.h"
#include "kernel/errorinfo.h"
#include "global/common_funcs.h"
#include "kernel/std_error_type.h"
#include "network/rpc/service_invoker.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QWebSocket;
QT_END_NAMESPACE

namespace sn{
namespace corelib{
namespace network{

using sn::corelib::ErrorInfo;
using sn::corelib::map_has_requires;
using sn::corelib::throw_exception;
using sn::corelib::StdErrorType;

class SN_CORELIB_EXPORT AbstractService : public QObject
{
   Q_OBJECT
friend class ServiceProvider;
public:
   AbstractService(ServiceProvider &provider);
   virtual ~AbstractService();
protected:
   void writeInterResponse(const ServiceInvokeRequest &request, ServiceInvokeResponse &response);
   QByteArray encodeJsonObject(const QVariant &data);
   template<typename T>
   void checkRequireFields(const QMap<QString, T> &map, const QStringList& requires = QStringList())throw(ErrorInfo);
   QSharedPointer<ServiceInvoker> getServiceInvoker(const QString &host, quint16 port);
   QString getErrorContext();
protected:
   virtual void notifySocketDisconnect(QTcpSocket *socket);
   virtual void notifySocketDisconnect(QWebSocket *socket);
protected:
   ServiceProvider& m_serviceProvider;
   QEventLoop m_eventLoop;
   QSharedPointer<ServiceInvoker> m_serviceInvoker;
};

template<typename T>
void AbstractService::checkRequireFields(const QMap<QString, T> &map, const QStringList& requires)throw(ErrorInfo)
{
   QStringList leak;
   map_has_requires(map, requires, leak);
   if(!leak.isEmpty()){
      throw_exception(ErrorInfo(StdErrorType::msg(SN_E_API_INVOKE_LEAK_ARGS, leak), SN_E_API_INVOKE_LEAK_ARGS), 
                      STD_EXCEPTION_CONTEXT);
   }
}

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_RPC_ABSTRACT_SERVICE_H
