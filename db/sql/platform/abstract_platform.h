#ifndef SN_CORELIB_DB_SQL_PLATFORM_ABSTRACT_PLATFORM_H 
#define SN_CORELIB_DB_SQL_PLATFORM_ABSTRACT_PLATFORM_H

#include <QMap>
#include <QString>
#include <QSharedPointer>
#include "global/global.h"
#include "db/engine/engine.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/abstract_preparable_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

using sn::corelib::db::sql::AbstractSql;
using sn::corelib::db::sql::AbstractPreparableSql;
using sn::corelib::db::engine::Engine;

class SN_CORELIB_EXPORT AbstractPlatform
{
public:
   using DecoratorPoolType = QMap<QString, QSharedPointer<AbstractSql>>;
public:
   virtual AbstractPlatform& setSubject(QSharedPointer<AbstractSql> subject);
   AbstractPlatform& setTypeDecorator(const QString& type, QSharedPointer<AbstractSql> decorator);
   QSharedPointer<AbstractSql> getTypeDecorator(QSharedPointer<AbstractSql> subject);
   QMap<QString, QSharedPointer<AbstractSql>>& getDecorators();
   virtual QString getSqlString(Engine &engine);
protected:
   QSharedPointer<AbstractSql> m_subject;
   DecoratorPoolType m_decorators;
};

class SN_CORELIB_EXPORT PlatformDecoratorInterface
{
};

}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_ABSTRACT_PLATFORM_H

