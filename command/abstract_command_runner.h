#ifndef SN_CORELIB_COMMAND_ABSTRACT_COMMAND_RUNNER_H
#define SN_CORELIB_COMMAND_ABSTRACT_COMMAND_RUNNER_H

#include <QList>
#include <QPair>
#include <QString>

#include "global/global.h"
#include "command_meta.h"
#include "io/terminal.h"
#include "route_stack.h"

namespace sn 
{
namespace corelib 
{

class Application;
class Settings;
class AbstractCommand;
class CommandMeta;

class SN_CORELIB_EXPORT AbstractCommandRunner
{
   Q_DISABLE_COPY(AbstractCommandRunner)
public:
   using CmdPoolType = QMap<QString, AbstractCommand* (*)(const AbstractCommandRunner&, const CommandMeta&)>;
   using UsageTextItemType = QPair<QString, TerminalColor>;
public:
   AbstractCommandRunner(Application &app);
   void printUsage()const;
   Settings& getSysSettings();
   void runCmd(const CommandMeta& meta);
public:
   virtual ~AbstractCommandRunner();
   virtual void run();
protected:
   void addUsageText(const QString& text, TerminalColor color = TerminalColor::Default);
   void addCmdRoute(const QString& name, const QString& route, int priority = 1, const QMap<QString, QString>& defaultParams = QMap<QString, QString>());
   void parseCmdInvokeArgs(CommandMeta& meta);
protected:
   CmdPoolType m_cmdRegisterPool;
   QList<UsageTextItemType> m_usageTextPool;
   Application& m_app;
   RouteStack m_router;
};

}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_ABSTRACT_COMMAND_RUNNER_H
