#include <QDataStream>

#include "api_invoker.h"
#include "kernel/errorinfo.h"

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
   QObject::connect(m_socket.data(), &QTcpSocket::readyRead, this, &ApiInvoker::responseArriveHandler);
}

bool ApiInvoker::request(ApiInvokeRequest &request, RequestCallbackType callback, void *callbackArgs)
{
   int serial = ++ApiInvoker::sm_serial;
   request.setSerial(serial);
   m_callbackPool.insert(serial, CallbackUnitType(callback, callbackArgs));
   writeRequestToSocket(request);
   return true;
}

void ApiInvoker::responseArriveHandler()
{
   
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