#ifndef SN_CORELIB_DB_SQL_DDL_CONSTRAINT_SIMPLE_CONSTRAINT_H
#define SN_CORELIB_DB_SQL_DDL_CONSTRAINT_SIMPLE_CONSTRAINT_H

#include <QVariant>
#include <QString>
#include <QSharedPointer>

#include "global/global.h"
#include "abstract_constraint.h"
#include "db/sql/abstract_expression.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{
namespace constraint{

using sn::corelib::db::sql::AbstractExpression;

class SN_CORELIB_EXPORT Check : public AbstractConstraint
{
public:
   Check(const QString &expression, const QString &name);
   Check(const QSharedPointer<AbstractExpression> expression, const QString &name);
protected:
   Check(const QVariant &expression, const QString &name);
public:
   virtual AbstractExpression::ExpressionDataType getExpressionData()const;
protected:
   QVariant m_expression;
};

class SN_CORELIB_EXPORT PrimaryKey : public AbstractConstraint
{
public:
   PrimaryKey(const QStringList &columns = QStringList(), const QString &name = QString());
};

class SN_CORELIB_EXPORT UniqueKey : public AbstractConstraint
{
public:
   UniqueKey(const QStringList &columns = QStringList(), const QString &name = QString());
};

class SN_CORELIB_EXPORT ForeignKey : public AbstractConstraint
{
public:
   ForeignKey(const QString &name, const QStringList columns, 
              const QString &referenceTable, const QStringList &referenceColumn,
              const QString &onDeleteRule = QString(), const QString &onUpdateRule = QString());
   ForeignKey& setReferenceTable(const QString &referenceTable);
   const QString& getReferenceTable()const;
   ForeignKey& setReferenceColumn(const QStringList &referenceColumn);
   const QStringList& getReferenceColumn()const;
   ForeignKey& setOnDeleteRule(const QString &onDeleteRule);
   const QString& getOnDeleteRule()const;
   ForeignKey& setOnUpdateRule(const QString &onUpdateRule);
   const QString& getOnUpdateRule()const;
public:
   virtual ExpressionDataType getExpressionData()const;
protected:
   QString m_onDeleteRule;
   QString m_onUpdateRule;
   QStringList m_referenceColumn;
   QString m_referenceTable;
   QStringList m_referenceSpecification;
   
};


}//constraint
}//ddl
}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DDL_CONSTRAINT_SIMPLE_CONSTRAINT_H
