#include <QListIterator>
#include <QString>

#include "abstract_command_runner.h"
#include "kernel/application.h"
#include "abstract_command.h"
#include "kernel/errorinfo.h"
#include "command_meta.h"

namespace sn 
{
namespace corelib 
{

AbstractCommandRunner::AbstractCommandRunner(Application &app)
   : m_app(app)
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

AbstractCommandRunner::~AbstractCommandRunner()
{
}

}//corelib
}//sn