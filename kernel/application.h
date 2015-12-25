#ifndef SN_CORELIB_APPLICATION_H
#define SN_CORELIB_APPLICATION_H

#include <QCoreApplication>
#include <QBasicTimer>

#include "global/global.h"
#include "settings.h"

QT_BEGIN_NAMESPACE
class QObject;
class QEvent;
QT_END_NAMESPACE

namespace sn{
namespace corelib{

class Settings;

class SN_CORELIB_EXPORT Application : public QCoreApplication
{
   Q_OBJECT
public:
   Application(int &argc, char **argv);
   bool notify(QObject * receiver, QEvent * event);
   static Application *instance();
   Settings& getSettings();
   QString& getRuntimeDir();
   QString& getPidFilename();
   bool createPidFile();
   void deletePidFile();
   //几个信号处理
   static void watchUnixSignal(int sig, bool watch = true);
public:
   virtual void ensureImportantDir();
   virtual ~Application();
protected:
   virtual QString getCfgFilename();
   virtual Settings::CfgInitializerFnType getDefaultCfgInitializerFn();
protected:
   Settings* m_settings = nullptr;
   QString m_runtimeDir;
   QString m_pidFilename;
   QBasicTimer m_timer;
};

}//corelib
}//sn

#endif //SN_CORELIB_APPLICATION_H
