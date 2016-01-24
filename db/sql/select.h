#ifndef SN_CORELIB_DB_SQL_SELECT_H
#define SN_CORELIB_DB_SQL_SELECT_H

#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QPair>
#include <QMap>

#include "global/global.h"
#include "kernel/errorinfo.h"

#include "db/sql/abstract_preparable_sql.h"
#include "db/sql/abstract_expression.h"
#include "db/sql/simple_sql.h"
#include "table_identifier.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::sql::AbstractSql;

class SN_CORELIB_EXPORT Select : public AbstractPreparableSql
{
   Q_OBJECT
public:
   const static QString SELECT;
   const static QString QUANTIFIER;
   const static QString COLUMNS;
   const static QString TABLE;
   const static QString JOINS;
   const static QString WHERE;
   const static QString GROUP;
   const static QString HAVING;
   const static QString ORDER;
   const static QString LIMIT;
   const static QString OFFSET;
   const static QString QUANTIFIER_DISTINCT;
   const static QString QUANTIFIER_ALL;
   const static QString JOIN_INNER;
   const static QString JOIN_OUTER;
   const static QString JOIN_LEFT;
   const static QString JOIN_RIGHT;
   const static QString JOIN_OUTER_RIGHT;
   const static QString JOIN_OUTER_LEFT;
   const static QString SQL_STAR;
   const static QString ORDER_ASCENDING;
   const static QString ORDER_DESCENDING;
   const static QString COMBINE;
   const static QString COMBINE_UNION;
   const static QString COMBINE_EXCEPT;
   const static QString COMBINE_INTERSECT;
public:
   friend ProcessResultPointerType process_select(AbstractSql *self,const Engine &engine, 
                                                  ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                  QMap<QString, ProcessResultPointerType> &parameters);
public:
   Select(const TableIdentifier &table = TableIdentifier());
   Select(const QString &table, const QString &schema = QString());
   Select& from(const QString &tableName, const QString &schema = QString())throw(ErrorInfo);
   Select& from(const TableIdentifier &table)throw(ErrorInfo);
   Select& setQuantifier(const QString &quantifier);
   Select& setQuantifier(const QSharedPointer<AbstractExpression> &quantifier);
   /*
     Specify columns from which to select
     Possible valid states:
     array()
     array(value, ...)
     value can be strings or Expression objects
     array(string => value, ...)
     key string will be use as alias,
     value can be string or Expression objects
   */
   Select& setColumns(QList<QVariant> &columns, bool prefixColumnsWithTable = true);
   Select& addColumn(const QVariant &column, int index);
   Select& addColumn(const QVariant &column, const QString &alias);
   Select& setPrefixColumnsWithTable(bool flag);
protected:
   QPair<QString, QString> resolveTable(const TableIdentifier &table, const Engine &engine, 
                                        ParameterContainer *parameterContainer = nullptr);
   QPair<QString, QString> resolveTable(const QString &table, const Engine &engine, 
                                        ParameterContainer *parameterContainer = nullptr);
   QPair<QString, QString> resolveTable(const QString &table, const QString &alias, 
                                        const Engine &engine, ParameterContainer *parameterContainer = nullptr);
   QString renderTable(const QString &table, const QString &alias = QString());
   
protected:
   TableIdentifier m_table;
   bool m_tableReadOnly = false;
   bool m_prefixColumnsWithTable = true;
   QVariant m_quantifier;
   QList<QVariant> m_columns;
   QSharedPointer<Where> m_where;
   QSharedPointer<Having> m_having;
};

}//sql
}//db
}//corelib
}//sn
Q_DECLARE_METATYPE(QSharedPointer<sn::corelib::db::sql::Select>)
#endif // SN_CORELIB_DB_SQL_SELECT_H
