#ifndef SN_CORELIB_DB_SQL_DDL_CONSTRAINT_ABSTRACT_CONSTRAINT_H
#define SN_CORELIB_DB_SQL_DDL_CONSTRAINT_ABSTRACT_CONSTRAINT_H

#include "global/global.h"
#include "db/sql/abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace constraint{

using sn::corelib::db::sql::AbstractExpression;

class SN_CORELIB_EXPORT AbstractConstraint : public AbstractExpression
{
public:
   AbstractConstraint(const QStringList &columns = QStringList(), const QString &name = QString());
   AbstractConstraint& setName(const QString &name);
   const QString& getName()const;
   AbstractConstraint& setColumns(const QStringList &columns);
   const QStringList& getColumns()const;
   AbstractConstraint& addColumn(const QString &column);
public:
   virtual ExpressionDataType getExpressionData()const;
   virtual ~AbstractConstraint();
protected:
   QString m_columnSpecification;
   QString m_namedSpecification;
   QString m_specification;
   QString m_name;
   QStringList m_columns;
};

}//constraint
}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_CONSTRAINT_ABSTRACT_CONSTRAINT_H
