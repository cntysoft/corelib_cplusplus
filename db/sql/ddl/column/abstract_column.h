#ifndef SN_CORELIB_DB_SQL_DDL_COLUMN_ABSTRACT_COLUMN_H
#define SN_CORELIB_DB_SQL_DDL_COLUMN_ABSTRACT_COLUMN_H

#include <QVariant>
#include <QString>
#include <QMap>
#include <QSharedPointer>

#include "global/global.h"
#include "db/sql/ddl/constraint/abstract_constraint.h"
#include "db/sql/abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace column {

using sn::corelib::db::sql::ddl::constraint::AbstractConstraint;
using sn::corelib::db::sql::AbstractExpression;

class SN_CORELIB_EXPORT AbstractColumn : public AbstractExpression
{
public:
   AbstractColumn(const QString &name = QString(), bool nullable = false,
                  const QVariant &defaultValue = QVariant(), const QMap<QString, QVariant> &options = QMap<QString, QVariant>());

   AbstractColumn& setName(const QString &name);
   const QString& getName()const;
   AbstractColumn& setNullable(bool flag);
   bool isNullable()const;
   AbstractColumn& setDefault(const QVariant &defaultValue);
   const QVariant& getDefault()const;
   AbstractColumn& setOptions(const QMap<QString, QVariant> &options);
   const QMap<QString, QVariant>& getOptions()const;
   AbstractColumn& setOption(const QString &name);
   const QString& getOption()const;
   AbstractColumn& addConstraint(const QSharedPointer<AbstractConstraint> &constraint);
   virtual ExpressionDataType getExpressionData()const;
protected:
   QVariant m_default;
   bool m_isNullable;
   QString m_name;
   QMap<QString, QVariant> m_options;
   QList<QSharedPointer<AbstractConstraint>> m_constraints;
   QString m_specification;
   QString m_type;
};

}//column
}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_COLUMN_ABSTRACT_COLUMN_H
