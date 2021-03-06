#ifndef SN_CORELIB_UPGRADE_UPGRADEENV_H
#define SN_CORELIB_UPGRADE_UPGRADEENV_H

#include <QtQml/QJSEngine>
#include <QMap>
#include <QString>
#include <QSharedPointer>
#include <QObject>

#include "global/global.h"
#include "kernel/errorinfo.h"
#include "upgrade_env_script_object.h"

namespace sn{
namespace corelib{
namespace upgrade{

using sn::corelib::ErrorInfo;

class SN_CORELIB_EXPORT UpgradeEnv : public QObject
{
   Q_OBJECT
   using ExceptionHandlerType = void (*)(ErrorInfo errorInfo);
public:
   UpgradeEnv(const QString &host, const QString &username, const QString &password, const QString &dbname);
   virtual ~UpgradeEnv();
   bool exec(const QString &filename)throw(ErrorInfo);
   void changeCurrentDd(const QString &name);
   bool registerContextObject(const QString &name, QObject *object, bool force = false);
   bool registerContextObject(const QString &name, QJSValue value, bool force = false);
public:
   void notifyException(const ErrorInfo error);
   void notifyLog(const QString &msg);
   UpgradeEnv& setMetaInfo(const QString &name, const QString &value);
   QString getMetaInfo(const QString &name, const QString &defaultValue = QString());
   QJSEngine& getJsEngine();
signals:
   void logMsgSignal(const QString &msg);
   void excpetionSignal(ErrorInfo errorInfo);
protected:
   QJSEngine m_engine;
   QSharedPointer<UpgradeEnvScriptObject> m_context;
   QMap<QString, QString> m_metaInfo;
   ExceptionHandlerType m_exceptionHandler = nullptr;
};

}//upgrade
}//corelib
}//sn

#endif // SN_CORELIB_UPGRADE_UPGRADEENV_H
