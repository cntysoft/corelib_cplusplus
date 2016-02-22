#include <QJsonDocument>

#include "upgrade_env_script_object.h"
#include <QDebug>
#include "kernel/errorinfo.h"
#include "upgradeenv.h"
#include "io/filesystem.h"

namespace sn{
namespace corelib{
namespace upgrade{

using sn::corelib::ErrorInfo;
using sn::corelib::Filesystem;

UpgradeEnvScriptObject::UpgradeEnvScriptObject(QMap<QString, QString> &metaInfo, UpgradeEnv &env)
   : m_dbEngine(Engine::QMYSQL, metaInfo),
     m_env(env),
     m_metaInfo(metaInfo)
{}

void UpgradeEnvScriptObject::writeLogMsg(const QString &msg)
{
   m_env.notifyLog(msg);
}

void UpgradeEnvScriptObject::dbQuery(const QString &query)
{
   try{
      m_dbEngine.query(query);
   }catch(ErrorInfo errorInfo){
      m_env.notifyException(errorInfo);
   }
}

QJSValue UpgradeEnvScriptObject::doGetConfig(const QString &filename)
{
   if(!m_metaInfo.contains("ConfigBaseDir")){
      return QJSValue();
   }
   QString base = m_metaInfo.value("ConfigBaseDir");
   QString targetFilename = base + "/" + filename;
   if(!Filesystem::fileExist(targetFilename)){
      return QJSValue();
   }else{
      return QJSValue(QString(Filesystem::fileGetContents(targetFilename)));
   }
}

QJSValue UpgradeEnvScriptObject::doSaveConfig(const QString &filename, const QString &config)
{
   if(!m_metaInfo.contains("ConfigBaseDir")){
      return QJSValue(false);
   }
   QString base = m_metaInfo.value("ConfigBaseDir");
   QString targetFilename = base + "/" + filename;
   if(-1 == Filesystem::filePutContents(targetFilename, config)){
      return QJSValue(false);
   }else{
      return QJSValue(true);
   }
}

QString UpgradeEnvScriptObject::getUtilsFuncDefs()
{
   return "UpgradeEnv.getConfig = function(filename)"
          "{"
             "var data = this.doGetConfig(filename);"
             "return JSON.parse(data)"
          "}"
          "UpgradeEnv.saveConfig = function(filename, data)"
          "{"
             "return this.doSaveConfig(filename, JSON.stringify(data, null, 3));"
          "}";
}

}//upgrade
}//corelib
}//sn
