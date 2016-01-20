#ifndef SN_CORELIB_DB_ENGINE_ENGINE_H
#define SN_CORELIB_DB_ENGINE_ENGINE_H

#include <QSqlDatabase>
#include <QSqlField>
#include <QString>
#include <QLatin1String>
#include <QMap>
#include <QSqlDriver>

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
      IbmDb2
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
   static int DB_CONN_NAME_SEED;
   const static QMap<QString, PlatformType> sm_driverToPlatformMap;
public:
   Engine(const QString &driverType, QMap<QString, QString> connectionParams);
   QSqlDatabase& getDbConnection();
   QString& getCurrentSchema();
   void query(const QString& sql, QueryMode queryMode = QueryMode::Prepare);
   QString quoteValue(const QSqlField &field) const;
   QString quoteIdentifier(const QString &identifier, IdentifierType type)const;
   QString quoteTableName(const QString &tableName);
   QString quoteFieldName(const QString &fieldName);
   PlatformType getPlatformType();
protected:
   QSqlDatabase m_database;
   QString m_dbname;
   PlatformType m_platformType;
};

}//engine
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_ENGINE_ENGINE_H

