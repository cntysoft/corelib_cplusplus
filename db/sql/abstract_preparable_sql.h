#ifndef SN_CORELIB_DB_SQL_ABSTRACT_PREPARABLE_SQL_H
#define SN_CORELIB_DB_SQL_ABSTRACT_PREPARABLE_SQL_H

#include "global/global.h"
#include "abstract_sql.h"
#include "db/engine/interface.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::engine::StatementContainerInterface;

class SN_CORELIB_EXPORT AbstractPreparableSql : public AbstractSql
{
public:
   StatementContainerInterface& prepareStatement(const Engine &engine, const StatementContainerInterface &statementContainer);
   virtual ~AbstractPreparableSql();
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_ABSTRACT_PREPARABLE_SQL_H
