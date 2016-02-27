#include <QFile>
#include <QTextStream>
#include <QJSValue>
#include <QJsonDocument>

#include "upgradeenv.h"
#include "kernel/std_error_type.h"
#include <QDebug>

namespace sn{
namespace corelib{
namespace upgrade{

using sn::corelib::StdErrorType;

UpgradeEnv::UpgradeEnv(const QString &host, const QString &username, const QString &password, const QString &dbname)
{
   m_metaInfo.insert("host", host);
   m_metaInfo.insert("username", username);
   m_metaInfo.insert("password", password);
   m_metaInfo.insert("dbname", dbname);
   m_context.reset(new UpgradeEnvScriptObject(m_metaInfo, *this));
   QJSValue context = m_engine.newQObject(m_context.data());
   m_engine.globalObject().setProperty("UpgradeEnv", context);
   m_engine.evaluate(m_context->getUtilsFuncDefs());
}


void UpgradeEnv::exec(const QString &filename)throw(ErrorInfo)
{
   QFile scriptFile(filename);
   if (!scriptFile.open(QIODevice::ReadOnly)){
      throw_exception(ErrorInfo(StdErrorType::msg(SN_E_FILE_NOT_EXIST, {filename}), SN_E_FILE_NOT_EXIST), STD_EXCEPTION_CONTEXT);
   }
   QTextStream stream(&scriptFile);
   QString contents = stream.readAll();
   scriptFile.close();
   QJSValue ret = m_engine.evaluate(contents, filename);
   if(ret.isError()){
//      throw_exception(ErrorInfo(StdErrorType::msg(SN_E_EXEC_JS, {ret.toString()}), SN_E_EXEC_JS), STD_EXCEPTION_CONTEXT);
      notifyException(ErrorInfo(StdErrorType::msg(SN_E_EXEC_JS, {
                                                     QString("%1  %2  %3 line number : %4")
                                                     .arg(ret.property("name").toString())
                                                     .arg(ret.property("message").toString())
                                                     .arg(ret.property("fileName").toString())
                                                     .arg(ret.property("lineNumber").toString())
                                                  }), SN_E_EXEC_JS));
   }
}

QJSEngine& UpgradeEnv::getJsEngine()
{
   return m_engine;
}

UpgradeEnv& UpgradeEnv::setMetaInfo(const QString &name, const QString &value)
{
   m_metaInfo.insert(name, value);
   return *this;
}

QString UpgradeEnv::getMetaInfo(const QString &name, const QString &defaultValue)
{
   if(!m_metaInfo.contains(name)){
      return defaultValue;
   }
   return m_metaInfo.value(name);
}

bool UpgradeEnv::registerContextObject(const QString &name, QObject *object, bool force)
{
   QJSValue globalObject = m_engine.globalObject();
   if(globalObject.hasProperty(name) && !force){
      return false;
   }
   QJSValue jsObject = m_engine.newQObject(object);
   globalObject.setProperty(name, jsObject);
   return true;
}

bool UpgradeEnv::registerContextObject(const QString &name, QJSValue value, bool force)
{
   QJSValue globalObject = m_engine.globalObject();
   if(globalObject.hasProperty(name) && !force){
      return false;
   }
   globalObject.setProperty(name, value);
   return true;
}

void UpgradeEnv::notifyException(const ErrorInfo error)
{
   emit excpetionSignal(error);
}

void UpgradeEnv::notifyLog(const QString &msg)
{
   emit logMsgSignal(msg);
}

UpgradeEnv::~UpgradeEnv()
{}

}//upgrade
}//corelib
}//sn