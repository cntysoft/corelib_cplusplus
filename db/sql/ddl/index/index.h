#ifndef SN_CORELIB_DB_SQL_DDL_INDEX_INDEX_H
#define SN_CORELIB_DB_SQL_DDL_INDEX_INDEX_H

#include "global/global.h"
#include "db/sql/ddl/constraint/abstract_constraint.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace index{

using sn::corelib::db::sql::ddl::constraint::AbstractConstraint;

class SN_CORELIB_EXPORT Index : public AbstractConstraint
{
public:
   Index(const QString &column, const QString &name = QString(), 
         const QStringList &lengths = QStringList());
public:
   virtual AbstractConstraint::ExpressionDataType getExpressionData()const;
protected:
   QStringList m_lengths;
};

}//index
}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_INDEX_INDEX_H

