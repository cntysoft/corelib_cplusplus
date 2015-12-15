#include <QCoreApplication>

#include "abstract_command.h"
#include "global/common_funcs.h"

namespace sn 
{
namespace corelib 
{

AbstractCommand::AbstractCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta)
   : m_cmdRunner(runner), m_invokeMeta(invokeMeta)
{
}

AbstractCommand::~AbstractCommand()
{
}

void AbstractCommand::exit(int exitCode) const
{
   const QCoreApplication& app = get_core_application_ref();
   app.exit(exitCode);
}

void AbstractCommand::printConsoleMsg(const char *str, TerminalColor color, bool underline, bool blink) const
{
   Terminal::writeText(str, color, underline, blink);
}

}//corelib
}//sn