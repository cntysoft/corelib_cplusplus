#include <QStringList>

#include "engine.h"
#include "kernel/errorinfo.h"
#include <QDebug>

namespace sn{
namespace corelib{
namespace db{
namespace engine{

using sn::corelib::ErrorInfo;

const QLatin1String Engine::PREPARE_TYPE_POSITIONAL("positional");
const QLatin1String Engine::PREPARE_TYPE_NAMED("named");

const QString Engine::QMYSQL("QMYSQL");

int Engine::DB_CONN_NAME_SEED = 0;

const QMap<QString, Engine::PlatformType> Engine::sm_driverToPlatformMap{
   {Engine::QMYSQL, Engine::PlatformType::Mysql}
};

Engine::Engine(const QString &driverType, QMap<QString, QString> connectionParams)
{
   if(!QSqlDatabase::isDriverAvailable(driverType)){
      throw ErrorInfo(QString("driver : %s is not supports").arg(driverType));
   }
   QString name = QString("EngineConn_%1").arg(Engine::DB_CONN_NAME_SEED++);
   m_database = QSqlDatabase::addDatabase(driverType, name);
   if(connectionParams.contains("host")){
      m_database.setHostName(connectionParams.value("host"));
   }
   if(connectionParams.contains("port")){
      m_database.setPort(connectionParams.value("port").toInt());
   }
   if(connectionParams.contains("username")){
      m_database.setUserName(connectionParams.value("username"));
   }
   if(connectionParams.contains("password")){
      m_database.setPassword(connectionParams.value("password"));
   }
   if(connectionParams.contains("dbname")){
      m_database.setDatabaseName(connectionParams.value("dbname"));
      m_dbname = connectionParams.value("dbname");
   }
   m_platformType = sm_driverToPlatformMap.value(driverType);
}

QSqlDatabase& Engine::getDbConnection()
{
   return m_database;
}

QString& Engine::getCurrentSchema()
{
   return m_dbname;
}

QString Engine::quoteValue(const QSqlField &field) const
{
   QSqlDriver *driver = m_database.driver();
   return driver->formatValue(field, true);
}

QString Engine::quoteIdentifier(const QString &identifier, IdentifierType type) const
{
   QSqlDriver *driver = m_database.driver();
   return driver->escapeIdentifier(identifier, type);
}

QString Engine::quoteTableName(const QString &tableName)const
{
   QSqlDriver *driver = m_database.driver();
   return driver->escapeIdentifier(tableName, IdentifierType::TableName);
}

QString Engine::quoteFieldName(const QString &fieldName) const
{
   QSqlDriver *driver = m_database.driver();
   return driver->escapeIdentifier(fieldName, IdentifierType::FieldName);
}

QChar Engine::getIdentifierSeparator()const
{
   return '.';
}

void Engine::query(const QString &sql, QueryMode queryMode)
{
   
}

Engine::PlatformType Engine::getPlatformType()
{
   return m_platformType;
}

}//engine
}//db
}//corelib
}//sn