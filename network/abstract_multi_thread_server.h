#ifndef SN_CORELIB_NETWORK_ABSTRACT_MULTI_THREAD_SERVER_H
#define SN_CORELIB_NETWORK_ABSTRACT_MULTI_THREAD_SERVER_H

#include "global/global.h"
#include "kernel/application.h"

#include <QTcpServer>
#include <QNetworkReply>

namespace sn{
namespace corelib{
namespace network{

using sn::corelib::Application;

class SN_CORELIB_EXPORT AbstractMultiThreadServer : public QTcpServer
{
   Q_OBJECT
public:
   AbstractMultiThreadServer(Application& app, QObject* parent = nullptr);
   bool run();
   AbstractMultiThreadServer& setHost(const QHostAddress& host);
   QHostAddress& getHost();
   AbstractMultiThreadServer& setPort(quint16 port);
   quint16 getPort();
   AbstractMultiThreadServer& setEnableSsl(bool flag);
   bool getEnableSsl();
protected:
   Application& m_app;
   bool m_enableSsl = false;
   QHostAddress m_host;
   quint16 m_port;
};

}//network
}//corelib
}//sn

#endif // SN_CORELIB_NETWORK_ABSTRACT_MULTI_THREAD_SERVER_H
