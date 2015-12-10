#include <QListIterator>

#include "abstract_command_runner.h"
#include "kernel/application.h"
#include "command_parser_meta.h"
#include "abstract_command.h"

namespace sn 
{
namespace corelib 
{

AbstractCommandRunner::AbstractCommandRunner(const Application &app, const CommandParserMeta &commandParserMeta)
   : m_app(app),
     m_cmdParserMeta(commandParserMeta)
{}

void AbstractCommandRunner::addUsageText(const QString& text, TerminalColor color)
{
   m_usageTextPool.append({
      {text, color}                            
   });
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