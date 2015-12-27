#include <QDir>
#include <QFile>

#include <csignal>
#include <cerrno>

#include "application.h"
#include "errorinfo.h"
#include "io/terminal.h"
#include "io/filesystem.h"

namespace sn{
namespace corelib{

static volatile std::sig_atomic_t unixSignal = -1;

static void default_signal_handler(int signal)
{
   unixSignal = signal;
}

Application::Application(int &argc, char **argv)
   :QCoreApplication(argc, argv)
{}

Application* Application::instance()
{
   return qobject_cast<Application*>(QCoreApplication::instance());
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

void Application::watchUnixSignal(int sig, bool watch)
{
   if(sig < NSIG){
      struct sigaction sa;
      memset(&sa, 0, sizeof(sa));
      sa.sa_flags = SA_RESTART;
      if(watch){
         sa.sa_handler = default_signal_handler;
         m_timer.start(500, this);
      }else{
         sa.sa_handler = SIG_IGN;
      }
      if(sigaction(sig, &sa, 0) != 0){
         throw ErrorInfo(QString("sigaction failed errno : %1").arg(errno));
      }
   }
}

void Application::ignoreUnixSignal(int sig, bool ignore)
{
   if(sig < NSIG){
      struct sigaction sa;
      memset(&sa, 0, sizeof(sa));
      sa.sa_flags = SA_RESTART;
      sa.sa_handler = ignore ? SIG_IGN : SIG_DFL;
      if(sigaction(sig, &sa, 0) != 0){
         throw ErrorInfo(QString("sigaction failed errno : %1").arg(errno));
      }
   }
}

int Application::getCatchedSignalNumber()
{
   return sn::corelib::unixSignal;
}

void Application::resetCatchedSignalNumber()
{
   sn::corelib::unixSignal = -1;
}

void Application::setCatchedSignalNumber(int sig)
{
   sn::corelib::unixSignal = sig;
}

void Application::timerEvent(QTimerEvent *event)
{
   if (event->timerId() == m_timer.timerId()) {
      if (getCatchedSignalNumber() >= 0) {
         Terminal::writeText(QString("system trapped signal  number: %1").arg(getCatchedSignalNumber()).toLocal8Bit());
         exit(getCatchedSignalNumber());
      }
   } else {
      QCoreApplication::timerEvent(event);
   }
}

Application::~Application()
{
   if(nullptr != m_settings){
      delete m_settings;
   }
}

}//corelib
}//sn