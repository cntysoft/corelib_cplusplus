#ifndef SN_CORELIB_DB_SQL_PREDICATE_INTERFACE_H
#define SN_CORELIB_DB_SQL_PREDICATE_INTERFACE_H

#include "db/sql/interface.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace predicate{

using sn::corelib::db::sql::ExpressionInterface;

class SN_CORELIB_EXPORT PredicateInterface : public ExpressionInterface
{};

}//predicate
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_PREDICATE_INTERFACE_H
