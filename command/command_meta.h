#ifndef SN_CORELIB_COMMAND_COMMAND_META_H
#define SN_CORELIB_COMMAND_COMMAND_META_H

#include <QLatin1String>
#include <QVariant>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn 
{
namespace corelib 
{

class SN_CORELIB_EXPORT CommandMeta
{
public:
   using CmdArgType = QMap<QString, QString>;
public:
   CommandMeta();
   CommandMeta(const QString& category, const QString& commandName, const CmdArgType& args);
   const QString& getCommandCategory()const;
   const QString& getCommandName()const;
   const CmdArgType& getCmdArgs()const;
   CommandMeta& setCommandCategory(const QString& category);
   CommandMeta& setCommandName(const QString& name);
   CommandMeta& setCmdArgs(const CmdArgType& args);
private:
   QString m_commandCategory;
   QString m_command;
   CmdArgType m_args;
};


}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_COMMAND_META_H