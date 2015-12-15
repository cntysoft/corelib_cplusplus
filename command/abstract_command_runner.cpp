#include <QListIterator>
#include <QString>
#include <QDebug>

#include "abstract_command_runner.h"
#include "kernel/application.h"
#include "abstract_command.h"
#include "kernel/errorinfo.h"
#include "command_meta.h"
#include "command/route_item.h"
#include "command/route_match_result.h"

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
   AbstractCommand* (*initializer)(AbstractCommandRunner&, const CommandMeta&) = m_cmdRegisterPool[cmdKey];
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

void AbstractCommandRunner::parseCmdInvokeArgs(CommandMeta& meta)
{
   QStringList invokeArgs = m_app.arguments();
   invokeArgs.removeFirst();
   RouteMatchResult routeMatch = m_router.match(invokeArgs);
   if(!routeMatch.getStatus()){
      printUsage();
      throw ErrorInfo();
   }
   meta.setCommandCategory(routeMatch.getParam("category"));
   meta.setCommandName(routeMatch.getParam("name"));
   meta.setCmdArgs(routeMatch.getParams());
}

void AbstractCommandRunner::addCmdRoute(const QString& name, const QString& route, int priority, const QMap<QString, QString>& defaultParams)
{
   Q_ASSERT_X(defaultParams.contains("category") && defaultParams.contains("name"), 
              "AbstractCommandRunner::addCmdRoute()", "must contain key category and key name");
   m_router.addRoute(name, RouteItem(route, defaultParams), priority);
}

void AbstractCommandRunner::run()
{
   CommandMeta meta;
   parseCmdInvokeArgs(meta);
   runCmd(meta);
}

AbstractCommandRunner::~AbstractCommandRunner()
{
}

}//corelib
}//sn