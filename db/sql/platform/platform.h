#ifndef SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H
#define SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H

#include <QSharedPointer>
#include <QMap>

#include "abstract_platform.h"
#include "global/global.h"
#include "db/engine/engine.h"
#include "db/sql/abstract_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

using sn::corelib::db::engine::Engine;
using sn::corelib::db::sql::AbstractSql;

class SN_CORELIB_EXPORT Platform : public AbstractPlatform
{
public:
   Platform(Engine &engine);
   QSharedPointer<AbstractSql> getTypeDecorator(QSharedPointer<AbstractSql> subject, Engine::PlatformType platformType);
   virtual QString getSqlString(Engine &engine);
   Platform& setTypeDecorator(const QString& type, QSharedPointer<AbstractSql> decorator, Engine::PlatformType platformType);
   Platform& setTypeDecorator(const QString& type, QSharedPointer<AbstractSql> decorator);
public:
   virtual DecoratorPoolType getDecorators()const;
protected:
   Engine& m_engine;
   QMap<Engine::PlatformType, DecoratorPoolType> m_decorators;
   Engine::PlatformType m_defaultPlatform;
};


}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_PLATFORM_H
