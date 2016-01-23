#ifndef SN_CORELIB_DB_SQL_DELETE_H
#define SN_CORELIB_DB_SQL_DELETE_H

#include <QString>
#include <QSharedPointer>

#include "global/global.h"
#include "abstract_preparable_sql.h"
#include "table_identifier.h"
#include "db/sql/predicate/simple_predicate.h"
#include "db/sql/simple_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::sql::Where;
using sn::corelib::db::sql::predicate::PredicateSet;

class SN_CORELIB_EXPORT Delete : public AbstractPreparableSql
{
   Q_OBJECT
public:
   friend ProcessResultPointerType process_delete(AbstractSql *self,const Engine &engine, 
                                                  ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                  QMap<QString, ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType process_where(AbstractSql *self, const Engine &engine, 
                                                 ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                 QMap<QString, ProcessResultPointerType> &parameters);
public:
   struct RawState{
      bool emptyWhereProtection;
      TableIdentifier table;
      QSharedPointer<Where> where;
   };
public:
   const static QString SPECIFICATION_DELETE;
   const static QString SPECIFICATION_WHERE;
public:
   Delete(const TableIdentifier &table = TableIdentifier());
   Delete(const QString &table);
   TableIdentifier& getTable();
   RawState getRawState();
   Delete& where(const QSharedPointer<Where> &where);
   Delete& where(const QString &where, const QString &combination = PredicateSet::OP_AND);
   ~Delete();
protected:
   TableIdentifier m_table;
   bool m_emptyWhereProtection = true;
   QSharedPointer<Where> m_where;
};

}//sql
}//db
}//corelib
}//sn

#endif // SN_CORELIB_DB_SQL_DELETE_H
