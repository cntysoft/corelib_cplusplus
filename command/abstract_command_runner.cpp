#include <QListIterator>
#include <QString>

#include "abstract_command_runner.h"
#include "kernel/application.h"
#include "command_parser_meta.h"
#include "abstract_command.h"
#include "kernel/errorinfo.h"
#include "command_meta.h"

namespace sn 
{
namespace corelib 
{

AbstractCommandRunner::AbstractCommandRunner(Application &app, const CommandParserMeta &commandParserMeta)
   : m_app(app),
     m_cmdParserMeta(commandParserMeta)
{}

void AbstractCommandRunner::addUsageText(const QString& text, TerminalColor color)
{
   m_usageTextPool.append({
      {text, color}                            
   });
}

Settings& AbstractCommandRunner::getSysSettings()
{
   return m_app.getSettings();
}

void AbstractCommandRunner::runCmd(const CommandMeta& meta)
{
   QString cmdKey(meta.getCommandCategory()+"_"+meta.getCommandName());
   if(!m_cmdRegisterPool.contains(cmdKey)){
      throw ErrorInfo(QString("command : %1 is not exist").arg(cmdKey));
   }
   AbstractCommand* (*initializer)(const AbstractCommandRunner&, const CommandMeta&) = m_cmdRegisterPool[cmdKey];
   QScopedPointer<AbstractCommand> cmd(initializer(*this, meta));
   cmd->exec();
}

void AbstractCommandRunner::printUsage()const
{
   QListIterator<UsageTextItemType> iterator(m_usageTextPool);
   while(iterator.hasNext()){
      UsageTextItemType item(iterator.next());
      Terminal::writeText(item.first.toLocal8Bit(), item.second);
   }
}

QList<QString> AbstractCommandRunner::getSupportSubCommands()const
{
   return m_cmdRegisterPool.keys();
}

void AbstractCommandRunner::parseSubCmdArgs(CommandMeta::CmdArgType&, QString&, const QString&, const QStringList&)
{}

bool AbstractCommandRunner::isSubCmdSupported(const QString& cmd) const
{
   QList<QString> cmds = getSupportSubCommands();
   return cmds.contains(cmd);
}

AbstractCommandRunner::~AbstractCommandRunner()
{
}

}//corelib
}//sn