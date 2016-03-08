#ifndef SN_CORELIB_COMMAND_ABSTRACT_COMMAND_H
#define SN_CORELIB_COMMAND_ABSTRACT_COMMAND_H

#include <QObject>

#include "global/global.h"
#include "io/terminal.h"
#include "command/abstract_command_runner.h"
#include "command/command_meta.h"

namespace sn{
namespace corelib{

class SN_CORELIB_EXPORT AbstractCommand : public QObject
{
   Q_OBJECT
   Q_DISABLE_COPY(AbstractCommand)
public:
   AbstractCommand(AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
   void exit(int exitCode = 0) const;
   void printConsoleMsg(const char* str, TerminalColor color = TerminalColor::Default, bool underline = false, bool blink = false) const;
   void printConsoleMsg(const QString &str, TerminalColor color = TerminalColor::Default, bool underline = false, bool blink = false) const;
public:
   virtual void exec() = 0;
   virtual ~AbstractCommand();
protected:
   AbstractCommandRunner& m_cmdRunner;
   const CommandMeta& m_invokeMeta;
};

}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_ABSTRACT_COMMAND_H
