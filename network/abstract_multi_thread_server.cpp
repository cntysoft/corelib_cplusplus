#include "abstract_multi_thread_server.h"

#include "kernel/application.h"

namespace sn{
namespace network{

AbstractMultiThreadServer::AbstractMultiThreadServer(Application &app, QObject *parent)
   : QTcpServer(parent),
     m_app(app)
{}

}//network
}//sn