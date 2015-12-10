#ifndef SN_CORELIB_COMMAND_ABSTRACT_COMMAND_RUNNER_H
#define SN_CORELIB_COMMAND_ABSTRACT_COMMAND_RUNNER_H

#include <QList>
#include <QPair>
#include <QString>

#include "global/global.h"
#include "command/command_parser_meta.h"
#include "command_meta.h"
#include "io/terminal.h"

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
   using CmdPoolType = QMap<QString, AbstractCommand* (*)(AbstractCommandRunner*, const CommandMeta& meta)>;
   using UsageTextItemType = QPair<QString, TerminalColor>;
public:
   AbstractCommandRunner(const Application &app, const CommandParserMeta& commandParserMeta);
   void printUsage()const;
   Settings& getSysSettings();
   QCommandLineParser* getCmdParserByCmdName(const QString& cmdName);
   QList<QString> getSupportSubCommands() const;
   void runCmd(const CommandMeta& meta);
   bool isSubCmdSupported(const QString& cmd) const;
public:
   virtual ~AbstractCommandRunner();
   virtual void run() = 0;
protected:
   CommandMeta::CmdArgType parseSubCmdArgs(const QString& category, const QString& cmd, const QStringList& invokeArgs);
   void addUsageText(const QString& text, TerminalColor color = TerminalColor::Default);
protected:
   static const CmdPoolType m_cmdRegisterPool;
   QList<UsageTextItemType> m_usageTextPool;
   const Application& m_app;
   const CommandParserMeta& m_cmdParserMeta;
};

}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_ABSTRACT_COMMAND_RUNNER_H
