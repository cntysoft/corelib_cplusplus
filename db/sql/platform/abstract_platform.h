#ifndef SN_CORELIB_DB_SQL_PLATFORM_ABSTRACT_PLATFORM_H 
#define SN_CORELIB_DB_SQL_PLATFORM_ABSTRACT_PLATFORM_H

#include <QMap>
#include <QString>
#include <QSharedPointer>

#include "db/sql/interface.h"
#include "global/global.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{

using sn::corelib::db::sql::PlatformDecoratorInterface;
using sn::corelib::db::sql::SqlInterface;
using sn::corelib::db::sql::PreparableSqlInterface;

class SN_CORELIB_EXPORT AbstractPlatform : public PlatformDecoratorInterface, public PreparableSqlInterface, public SqlInterface
{
public:
   virtual Platform& setSubject(const PreparableSqlInterface &subject);
   virtual Platform& setSubject(const SqlInterface &subject);
   Platform& setTypeDecorator(const QString& type, PlatformDecoratorInterface* decorator);
   QSharedPointer<PlatformDecoratorInterface> getTypeDecorator(QSharedPointer<PreparableSqlInterface> &subject);
   QSharedPointer<PlatformDecoratorInterface> getTypeDecorator(QSharedPointer<SqlInterface> &subject);
   QMap<QString, PlatformDecoratorInterface*>& getDecorators();
protected:
   QSharedPointer<SqlInterface> m_normalSubject;
   QSharedPointer<PreparableSqlInterface> m_prepareSubject;
   QMap<QString, PlatformDecoratorInterface*> m_decorators;
};

}//platform
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PLATFORM_ABSTRACT_PLATFORM_H

