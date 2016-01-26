#ifndef SN_CORELIB_DB_ENGINE_ENGINE_H
#define SN_CORELIB_DB_ENGINE_ENGINE_H

#include <QSqlDatabase>
#include <QString>
#include <QLatin1String>
#include <QMap>
#include <QSqlDriver>
#include <QChar>
#include <QPair>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace engine{

class SN_CORELIB_EXPORT Engine
{
public:
   enum class PlatformType
   {
      Mysql,
      Mssql,
      Oracle,
      IbmDb2,
      SqlLite,
      PostgreSql
   };
   enum class QueryMode{
      Execute,
      Prepare
   };
   enum class PrepareType{
      Positional,
      Named
   };
   using IdentifierType = QSqlDriver::IdentifierType;
   const static QLatin1String PREPARE_TYPE_POSITIONAL;
   const static QLatin1String PREPARE_TYPE_NAMED;
   
   const static QString QMYSQL;
   const static QString QDB2;
   const static QString QODBC;
   const static QString QSQLITE;
   const static QString QSQLITE2;
   const static QString QOCI;
   const static QString QPSQL;
   
   static int DB_CONN_NAME_SEED;
   const static QMap<QString, PlatformType> sm_driverToPlatformMap;
public:
   Engine(const QString &driverType, QMap<QString, QString> connectionParams);
   QSqlDatabase& getDbConnection();
   QString& getCurrentSchema();
   void query(const QString& sql, QueryMode queryMode = QueryMode::Prepare);
   QString quoteValue(const QVariant &value) const;
   QString quoteIdentifier(const QString &identifier, IdentifierType type)const;
   QString quoteTableName(const QString &tableName)const;
   QString quoteFieldName(const QString &fieldName)const;
   QChar getIdentifierSeparator()const;
   QString quoteIdentifierInFragment(const QString &identifier, const QStringList &safeWords = QStringList())const;
   PlatformType getPlatformType();
   Engine& setQuoteIdentifiersFlag(bool flag);
   bool getQuoteIdentifiersFlag()const;
   QString formatParameterName(const QString &name)const;
protected:
   QPair<QChar, QChar> getQuoteIdentifier()const;
   QString getQuoteIdentifierTo()const;
protected:
   QSqlDatabase m_database;
   QString m_dbname;
   PlatformType m_platformType;
   bool m_quoteIdentifiers = true;
};

}//engine
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ENGINE_ENGINE_H

