#ifndef SN_CORELIB_NETWORK_ABSTRACT_MULTI_THREAD_SERVER_H
#define SN_CORELIB_NETWORK_ABSTRACT_MULTI_THREAD_SERVER_H

#include "global/global.h"

#include <QTcpServer>

namespace sn{
namespace network{

class Application;

class SN_CORELIB_EXPORT AbstractMultiThreadServer : public QTcpServer
{
public:
   AbstractMultiThreadServer(Application& app, QObject* parent = nullptr);
protected:
   Application& m_app;
};

}//network
}//sn

#endif // SN_CORELIB_NETWORK_ABSTRACT_MULTI_THREAD_SERVER_H
