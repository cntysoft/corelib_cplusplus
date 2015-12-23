#include <QDir>
#include <QFile>

#include "application.h"
#include "errorinfo.h"
#include "io/terminal.h"
#include "io/filesystem.h"

namespace sn{
namespace corelib{

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

QString& Application::getRuntimeDir()
{
   if(m_runtimeDir.isEmpty()){
      //为空这里进行尝试配置文件是否合适
      Settings& settings = getSettings();
      QString runtimeDir = settings.getValue("runtimeDir").toString();
      if(!runtimeDir.isEmpty()){
         m_runtimeDir = runtimeDir;
      }else{
         m_runtimeDir = QDir::tempPath() +QDir::separator() +applicationName();
      }
   }
   return m_runtimeDir;
}

QString& Application::getPidFilename()
{
   if(m_pidFilename.isEmpty()){
      m_pidFilename = getRuntimeDir() + QDir::separator() + applicationName()+".pid";
   }
   return m_pidFilename;
}

QString Application::getCfgFilename()
{
   QString cfgName(applicationName());
   return cfgName+"/"+cfgName+".ini";
}

bool Application::createPidFile()
{
   QFile pidFile(getPidFilename());
   if (pidFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
      qint64 pid = QCoreApplication::applicationPid();
      pidFile.write( QString::number(pid).toLatin1() );
      pidFile.close();
      return true;
   }
   return false;
}

void Application::deletePidFile()
{
   QFile(getPidFilename()).remove();
   
}

void Application::ensureImportantDir()
{
   QString runtimeDir(getRuntimeDir());
   if(!runtimeDir.isEmpty()){
      Filesystem::createPath(runtimeDir);
   }
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