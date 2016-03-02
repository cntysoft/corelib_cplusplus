#include <QCoreApplication>

#include "abstract_command.h"
#include "global/global.h"

namespace sn{
namespace corelib{

AbstractCommand::AbstractCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : m_cmdRunner(runner), m_invokeMeta(invokeMeta)
{
}

void AbstractCommand::exit(int exitCode) const
{
   QCoreApplication::instance()->exit(exitCode);
}

void AbstractCommand::printConsoleMsg(const char *str, TerminalColor color, bool underline, bool blink) const
{
   Terminal::writeText(str, color, underline, blink);
}

void AbstractCommand::printConsoleMsg(const QString &str, TerminalColor color, bool underline, bool blink) const
{
   printConsoleMsg(str.toStdString().c_str(), color, underline, blink);
}

AbstractCommand::~AbstractCommand()
{
}

}//corelib
}//sn