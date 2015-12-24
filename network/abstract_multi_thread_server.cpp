#include "abstract_multi_thread_server.h"
#include <QNetworkAccessManager>

namespace sn{
namespace network{



AbstractMultiThreadServer::AbstractMultiThreadServer( QObject *parent)
   : QTcpServer(parent)
{}

bool AbstractMultiThreadServer::run()
{
   QNetworkAccessManager *manager = new QNetworkAccessManager(this);
   QNetworkRequest request;
   request.setUrl(QUrl("http://qt-project.org"));
   request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");
   
   QNetworkReply *reply = manager->get(request);
   connect(reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
   
//   //暂时简单的监听
//   bool ret = listen(m_host, m_port);
   
//   return ret;
   return true;
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
}//sn