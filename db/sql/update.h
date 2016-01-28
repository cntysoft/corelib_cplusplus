#ifndef SN_CORELIB_DB_SQL_UPDATE_H
#define SN_CORELIB_DB_SQL_UPDATE_H

#include <QString>
#include <QVariant>
#include <QMap>
#include <QSharedPointer>

#include "global/global.h"
#include "kernel/errorinfo.h"

#include "db/sql/abstract_preparable_sql.h"
#include "db/sql/table_identifier.h"
#include "db/sql/simple_sql.h"
#include "db/sql/predicate/predicateset.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::sql::predicate::PredicateSet;

class SN_CORELIB_EXPORT Update : public AbstractPreparableSql
{
public:
   const static QString SPECIFICATION_UPDATE;
   const static QString SPECIFICATION_WHERE;
   const static QString VALUES_MERGE;
   const static QString VALUES_SET;
   
   struct RawState
   {
      TableIdentifier table;
      bool emptyWhereProtection;
      QSharedPointer<Where> where;
      QMap<QString, QVariant> set;
   };

public:
   friend ProcessResultPointerType update_process_update(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType update_process_where(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   Update(const TableIdentifier &table = TableIdentifier());
   Update(const QString &tableName, const QString &schema = QString());
   Update& table(const TableIdentifier &tableInfo = TableIdentifier());
   Update& table(const QString &tableName, const QString &schema = QString());
   Update& set(const QMap<QString, QVariant> &values, const QString &flag = Update::VALUES_SET);
   Update& where(const QSharedPointer<Where> &where);
   Update& where(const QString &where, const QString &combination = PredicateSet::OP_AND);
   RawState getRawState()const;
protected:
   TableIdentifier m_table;
   bool m_emptyWhereProtection = true;
   QSharedPointer<Where> m_where;
   QMap<QString, QVariant> m_set;
};

}//sql
}//db
}//corelib
}//sn


#endif // SN_CORELIB_DB_SQL_UPDATE_H
