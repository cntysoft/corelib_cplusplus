#ifndef SN_CORELIB_DB_SQL_INSERT_H
#define SN_CORELIB_DB_SQL_INSERT_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QSharedPointer>

#include "global/global.h"
#include "kernel/errorinfo.h"

#include "db/sql/abstract_preparable_sql.h"
#include "db/sql/table_identifier.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::ErrorInfo;

class SN_CORELIB_EXPORT Insert : public AbstractPreparableSql
{
public:
   const static QString SPECIFICATION_INSERT;
   const static QString SPECIFICATION_SELECT;
   const static QString VALUES_MERGE;
   const static QString VALUES_SET;
   struct RawState
   {
      TableIdentifier table;
      QStringList columns;
      QList<QVariant> values;
   };
   
public:
public:
   friend ProcessResultPointerType insert_process_select(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType insert_process_insert(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   Insert(const TableIdentifier &table = TableIdentifier());
   Insert(const QString &table, const QString &schema = QString());
   Insert& into(const TableIdentifier &table = TableIdentifier());
   Insert& into(const QString &table, const QString &schema = QString());
   Insert& addColumn(const QString &name, const QVariant &value);
   Insert& removeColumn(const QString &name);
   QVariant getColumn(const QString &name);
   bool hasColumn(const QString &name);
   Insert& columns(const QMap<QString, QVariant> &columns);
   Insert& select(QSharedPointer<Select> select);
   Insert& values(const QMap<QString, QVariant> &values, const QString flag = Insert::VALUES_SET)throw(ErrorInfo);
   Insert& values(const QSharedPointer<Select> &select, const QString flag = Insert::VALUES_SET)throw(ErrorInfo);
   RawState getRawState()const;
protected:
   TableIdentifier m_table;
   QSharedPointer<Select> m_select;
   QMap<QString, QVariant> m_columns;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_INSERT_H