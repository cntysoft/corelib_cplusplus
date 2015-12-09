#include "application.h"
#include "errorinfo.h"
#include "io/terminal.h"

namespace sn 
{
namespace corelib 
{

Application::Application(int &argc, char **argv)
   :QCoreApplication(argc, argv)
{}

Application *Application::instance()
{
   return qobject_cast<Application *>(QCoreApplication::instance());
}

bool Application::notify(QObject *receiver, QEvent *event)
{
   try{
      return QCoreApplication::notify(receiver, event);
   }catch(const ErrorInfo& errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLatin1(), TerminalColor::Red);
      }
      exit(EXIT_FAILURE);
      return false;
   }
}

Settings& Application::getSettings()
{
   if(nullptr == m_settings){
      m_settings = new Settings(getCfgFilename(), getDefaultCfgInitializerFn());
   }
   return *m_settings;
}

QString Application::getCfgFilename()
{
   QString cfgName(applicationName());
   return cfgName+"/"+cfgName+".ini";
}

Settings::CfgInitializerFnType Application::getDefaultCfgInitializerFn()
{
   return nullptr;
}

Application::~Application()
{
   if(nullptr != m_settings){
      delete m_settings;
   }
}

}//corelib
}//sn