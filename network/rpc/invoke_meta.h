#ifndef SN_CORELIB_NETWORK_RPC_INVOKE_META_H
#define SN_CORELIB_NETWORK_RPC_INVOKE_META_H

#include <QString>
#include <QStringList>
#include <QByteArray>

#include "global/global.h"

namespace sn{
namespace network{

class SN_CORELIB_EXPORT InvokeMeta
{
protected:
   QString m_name;
   QString m_method;
   QStringList m_args;
   QByteArray m_extraData;
};

}//network
}//sn

#endif // SN_CORELIB_NETWORK_RPC_INVOKE_META_H
