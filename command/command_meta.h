#ifndef SN_CORELIB_COMMAND_META_H
#define SN_CORELIB_COMMAND_META_H

#include <QLatin1String>
#include <QVariant>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn 
{
namespace corelib 
{

class CommandMeta
{
public:
   using CmdArgType = QMap<QLatin1String, QVariant>;
public:
   CommandMeta(const QString& category, const QString& commandName, const CmdArgType& args);
   const QString& getCommandCategory()const;
   const QString& getCommandName()const;
   const CmdArgType& getCmdArgs()const;
private:
   const QString m_commandCategory;
   const QString m_command;
   const CmdArgType m_args;
};


}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_META_H