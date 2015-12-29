#include <QDataStream>
#include <QDebug>

#include "api_invoker.h"
#include "kernel/errorinfo.h"
#include <QThread>
namespace sn{
namespace corelib{
namespace network{
using sn::corelib::ErrorInfo;

ApiInvoker::ApiInvoker(const QSharedPointer<QTcpSocket> &socket)
   :m_socket(socket)
{
   if(!m_socket->isValid()){
      throw ErrorInfo("socket is invalid");
   }
//   qDebug() << "api invoker" <<QThread::currentThreadId();
   
}

bool ApiInvoker::request(ApiInvokeRequest &request, RequestCallbackType callback, void *callbackArgs)
{
   int serial = ++ApiInvoker::sm_serial;
   request.setSerial(serial);
   m_callbackPool.insert(serial, CallbackUnitType(callback, callbackArgs));
   writeRequestToSocket(request);
   //qDebug() << m_socket->thread()->currentThreadId();
   return true;
}

void ApiInvoker::responseArriveHandler()
{
   qDebug() << "xiuxiuuix";
}

void ApiInvoker::writeRequestToSocket(const ApiInvokeRequest &request)
{
   QDataStream out(m_socket.data());
   out.setVersion(QDataStream::Qt_5_5);
   out << request;
   m_socket->write("\r\n");
   m_socket->flush();
}

ApiInvoker::~ApiInvoker()
{
   QObject::disconnect(m_socket.data(), &QTcpSocket::readyRead, this, &ApiInvoker::responseArriveHandler);
}

QAtomicInt ApiInvoker::sm_serial = 0;

}//network
}//corelib
}//sn