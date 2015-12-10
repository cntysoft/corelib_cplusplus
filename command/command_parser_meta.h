#ifndef SN_CORELIB_COMMAND_OPTION_POOL_H
#define SN_CORELIB_COMMAND_OPTION_POOL_H

#include <QMap>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QCommandLineOption;
class QCommandLineParser;
QT_END_NAMESPACE

namespace sn 
{
namespace corelib 
{

class SN_CORELIB_EXPORT CommandParserMeta
{
public:
   using OptionMapType = QMap<QString, QCommandLineOption*>;
public:
   CommandParserMeta();
   OptionMapType getCmdOptionsByName(const QString& name);
   QCommandLineParser* getCmdParserByName(const QString& name);
   ~CommandParserMeta();
protected:
   QMap<QString, QCommandLineParser*> m_cmdParserPool;
   QMap<QString, OptionMapType> m_cmdOptionsPool;
};

}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_OPTION_POOL_H
