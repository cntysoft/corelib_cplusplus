#ifndef SN_CORELIB_APPLICATION_H
#define SN_CORELIB_APPLICATION_H

#include <QCoreApplication>

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
   void ensureImportantDir();
public:
   virtual ~Application();
protected:
   virtual QString getCfgFilename();
   virtual Settings::CfgInitializerFnType getDefaultCfgInitializerFn();
protected:
   Settings* m_settings = nullptr;
   QString m_runtimeDir;
   QString m_pidFilename;
};

}//corelib
}//sn

#endif //SN_CORELIB_APPLICATION_H
