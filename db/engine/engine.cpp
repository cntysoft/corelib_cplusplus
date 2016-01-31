#include <QStringList>
#include <QSqlField>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QRegularExpressionMatch>

#include <algorithm>

#include "engine.h"
#include "kernel/errorinfo.h"


namespace sn{
namespace corelib{
namespace db{
namespace engine{

using sn::corelib::ErrorInfo;

const QLatin1String Engine::PREPARE_TYPE_POSITIONAL("positional");
const QLatin1String Engine::PREPARE_TYPE_NAMED("named");

const QString Engine::QMYSQL("QMYSQL");
const QString Engine::QDB2("QDB2");
const QString Engine::QODBC("QODBC");
const QString Engine::QSQLITE("QSQLITE");
const QString Engine::QSQLITE2("QSQLITE2");
const QString Engine::QOCI("QOCI");
const QString Engine::QPSQL("QPSQL");

int Engine::DB_CONN_NAME_SEED = 0;

const QMap<QString, Engine::PlatformType> Engine::sm_driverToPlatformMap{
   {Engine::QMYSQL, Engine::PlatformType::Mysql},
   {Engine::QDB2, Engine::PlatformType::IbmDb2},
   {Engine::QODBC, Engine::PlatformType::Mssql},
   {Engine::QSQLITE, Engine::PlatformType::SqlLite},
   {Engine::QSQLITE2, Engine::PlatformType::SqlLite},
   {Engine::QOCI, Engine::PlatformType::Oracle},
   {Engine::QPSQL, Engine::PlatformType::PostgreSql}
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

const QString& Engine::getCurrentSchema()
{
   return m_dbname;
}

QString Engine::quoteValue(const QVariant &value) const
{
   QSqlDriver *driver = m_database.driver();
   QSqlField field;
   field.setType(value.type());
   field.setValue(value);
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

bool Engine::getQuoteIdentifiersFlag() const
{
   return m_quoteIdentifiers;   
}

Engine& Engine::setQuoteIdentifiersFlag(bool flag)
{
   m_quoteIdentifiers = flag;
   return *this;
}

QString Engine::quoteIdentifierInFragment(const QString &identifier, const QStringList &safeWords)const
{
   if(!m_quoteIdentifiers){
      return identifier;
   }
   
   QMap<QString, bool> safeWordsInt{
      {"*", true},
      {" ", true},
      {".", true},
      {"as", true}
   };
   std::for_each(safeWords.cbegin(), safeWords.cend(), [&safeWordsInt](const QString &item){
      safeWordsInt.insert(item, true);
   });
   QString result(identifier);
   if(result == "*"){
      return result;
   }
   QRegularExpression regex("[^a-zA-Z0-9$_:]", QRegularExpression::CaseInsensitiveOption);
   QStringList parts = result.split(regex, QString::SkipEmptyParts);
   QRegularExpressionMatchIterator matchIterator = regex.globalMatch(result);
   int insertPos = 1;
   while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      QString delimeter = match.capturedTexts().first();
      delimeter = delimeter.trimmed();
      if(!delimeter.isEmpty()){
         parts.insert(insertPos, delimeter);
         insertPos += 2;
      }
   }
   result = "";
   std::for_each(parts.cbegin(), parts.cend(), [&result, &safeWordsInt, this](QString part){
      if(safeWordsInt.contains(part.toLower())){
         if("=" == part){
            part = " = ";
         }
         result += part;
      }else{
         QPair<QString, QString> quoteIdentifier = this->getQuoteIdentifier();
         result += quoteIdentifier.first + part.replace(quoteIdentifier.first, this->getQuoteIdentifierTo()) + quoteIdentifier.second;
      }
   });
   return result;
}

QPair<QChar, QChar> Engine::getQuoteIdentifier()const
{
   if(m_platformType == PlatformType::Mysql){
      return {'`','`'};
   }else if(m_platformType == PlatformType::Mssql){
      return {'[', ']'};
   }else{
      return {'"', '"'};
   }
}

QString Engine::getQuoteIdentifierTo() const
{
   if(m_platformType == PlatformType::Mysql){
      return "``";
   }else if(m_platformType == PlatformType::PostgreSql){
      return "\"\"";
   }else if(m_platformType == PlatformType::Mssql){
      return "\\";
   }else{
      return "\'";
   }
}

QString Engine::formatParameterName(const QString &name)const
{
   if(m_platformType == PlatformType::Mysql || m_platformType == PlatformType::IbmDb2 || m_platformType == PlatformType::Mssql){
      return "?";
   }else if(m_platformType == PlatformType::Oracle){
      return QString(":%1").arg(name);
   }else if(m_platformType == PlatformType::PostgreSql){
      return "$#";
   }
   return "?";
}

}//engine
}//db
}//corelib
}//sn