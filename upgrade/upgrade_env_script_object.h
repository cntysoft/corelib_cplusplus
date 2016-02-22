#ifndef SN_CORELIB_UPGRADE_UPGRADE_ENV_SCRIPT_OBJECT_H
#define SN_CORELIB_UPGRADE_UPGRADE_ENV_SCRIPT_OBJECT_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QtQml/QJSValue>

#include "db/engine/engine.h"

namespace sn{
namespace corelib{
namespace upgrade{

using sn::corelib::db::engine::Engine;
class UpgradeEnv;

class UpgradeEnvScriptObject : public QObject
{
   Q_OBJECT
public:
   UpgradeEnvScriptObject(QMap<QString, QString> &metaInfo, UpgradeEnv &env);
   Q_INVOKABLE void writeLogMsg(const QString &msg);
   Q_INVOKABLE void dbQuery(const QString &query);
   Q_INVOKABLE QJSValue doGetConfig(const QString &filename);
   Q_INVOKABLE QJSValue doSaveConfig(const QString &filename, const QString &config);
public:
   QString getUtilsFuncDefs();
protected:
   Engine m_dbEngine;
   UpgradeEnv &m_env;
   QMap<QString, QString> &m_metaInfo;
};

}//upgrade
}//corelib
}//sn

#endif // SN_CORELIB_UPGRADE_UPGRADE_ENV_SCRIPT_OBJECT_H
