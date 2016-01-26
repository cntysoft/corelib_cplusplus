#include <QVariant>
#include <QSharedPointer>
#include <QDebug>

#include "delete.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const QString Delete::SPECIFICATION_WHERE = "where";
const QString Delete::SPECIFICATION_DELETE = "delete";

AbstractSql::ProcessResultPointerType delete_process_delete(AbstractSql *self, const Engine &engine, ParameterContainer *parameterContainer, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Delete* deleteSql = dynamic_cast<Delete*>(self);
   Q_ASSERT_X(deleteSql != 0, "delete friend function process_delete", "self pointer cast fail");
   if(0 == deleteSql){
      throw ErrorInfo(QString("delete friend function process_delete self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString(deleteSql->m_specifications.value(Delete::SPECIFICATION_DELETE).toString()).arg(deleteSql->resolveTable(deleteSql->m_table, engine, parameterContainer)));
   return result;
}

AbstractSql::ProcessResultPointerType delete_process_where(AbstractSql *self, const Engine &engine, ParameterContainer *parameterContainer, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Delete* deleteSql = dynamic_cast<Delete*>(self);
   Q_ASSERT_X(deleteSql != 0, "delete friend function process_where", "self pointer cast fail");
   if(0 == deleteSql){
      throw ErrorInfo(QString("delete friend function process_where self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   if(deleteSql->m_where->count() == 0){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString(deleteSql->m_specifications.value(Delete::SPECIFICATION_WHERE).toString()).arg(deleteSql->processExpression(deleteSql->m_where, engine, parameterContainer, "where")));
   return result;
}

Delete::Delete(const QString &table)
   : Delete(TableIdentifier(table))
{
}

Delete::Delete(const TableIdentifier &table)
   : m_table(table),
     m_where(new Where)
{
   m_specifications.insert(Delete::SPECIFICATION_DELETE, "DELETE FROM %1");
   m_specifications.insert(Delete::SPECIFICATION_WHERE, "WHERE %1");
   m_specificationFnPtrs.insert("where", delete_process_where);
   m_specificationFnPtrs.insert("delete", delete_process_delete);
   m_specKeys.append("where");
   m_specKeys.append("delete");
}

Delete::RawState Delete::getRawState()
{
   return {
      m_emptyWhereProtection,
            m_table,
            m_where
   };
}

TableIdentifier& Delete::getTable()
{
   return m_table;
}

Delete& Delete::where(const QSharedPointer<Where> &where)
{
   m_where = where;
   return *this;
}

Delete& Delete::where(const QString &where, const QString &combination)
{
   m_where->addPredicate(where, combination);
   return *this;
}

Delete::~Delete()
{
}

}//sql1
}//db
}//corelib
}//sn