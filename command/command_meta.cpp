#include <QString>

#include "command_meta.h"

namespace sn 
{
namespace corelib 
{

CommandMeta::CommandMeta(const QString& commandCategory, const QString& commandName, const CmdArgType& args)
   : m_commandCategory(commandCategory), m_command(commandName), m_args(args)
{}

const QString& CommandMeta::getCommandName() const
{
   return m_command;
}

const QString& CommandMeta::getCommandCategory()const
{
   return m_commandCategory;
}

const CommandMeta::CmdArgType& CommandMeta::getCmdArgs()const
{
   return m_args;
}

}//corelib
}//sn