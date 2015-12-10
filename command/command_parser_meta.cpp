#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "command_parser_meta.h"

namespace sn 
{
namespace corelib 
{

CommandParserMeta::CommandParserMeta()
{}

CommandParserMeta::OptionMapType* CommandParserMeta::getCmdOptionsByName(const QString& name) const
{
   if(!m_cmdOptionsPool.contains(name)){
      return nullptr;
   }
   return m_cmdOptionsPool.value(name);
}

QCommandLineParser* CommandParserMeta::getCmdParserByName(const QString &name) const
{
   if(!m_cmdParserPool.contains(name)){
      return nullptr; 
   }
   return m_cmdParserPool.value(name);
}

CommandParserMeta::~CommandParserMeta()
{
}

}//corelib
}//sn