#include "abstract_multi_thread_server.h"
#include <QNetworkAccessManager>

namespace sn{
namespace corelib{
namespace network{

AbstractMultiThreadServer::AbstractMultiThreadServer(Application& app, QObject *parent)
   : QTcpServer(parent),
     m_app(app)
{}

bool AbstractMultiThreadServer::run()
{
   //暂时简单的监听
   bool ret = listen(m_host, m_port);
   return ret;
}
void AbstractMultiThreadServer::slotReadyRead()
{
   qDebug() << "asdasdas";
}

AbstractMultiThreadServer& AbstractMultiThreadServer::setHost(const QHostAddress &host)
{
   m_host = host;
   return *this;
}

QHostAddress& AbstractMultiThreadServer::getHost()
{
   return m_host;
}

AbstractMultiThreadServer& AbstractMultiThreadServer::setPort(quint16 port)
{
   m_port = port;
   return *this;
}

quint16 AbstractMultiThreadServer::getPort()
{
   return m_port;
}

AbstractMultiThreadServer& AbstractMultiThreadServer::setEnableSsl(bool flag)
{
   m_enableSsl = flag;
   return *this;
}

bool AbstractMultiThreadServer::getEnableSsl()
{
   return m_enableSsl;
}

}//network
}//corelib
}//sn