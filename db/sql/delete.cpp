#include <QVariant>
#include <QSharedPointer>

#include "delete.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

const QString Delete::SPECIFICATION_WHERE = "where";
const QString Delete::SPECIFICATION_DELETE = "delete";

AbstractSql::ProcessResultPointerType process_delete(AbstractSql *self, const Engine &engine, const ParameterContainer &parameterContainer, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   Delete* deleteSql = qobject_cast<Delete*>(self);
   Q_ASSERT_X(deleteSql != 0, "delete friend function process_delete", "self pointer cast fail");
   if(0 == deleteSql){
      throw ErrorInfo(QString("delete friend function process_delete self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->type = AbstractSql::ProcessResultType::String;
   result->value = QVariant(QString(deleteSql->m_specifications.value(Delete::SPECIFICATION_DELETE).toString()).arg(deleteSql->resolveTable(deleteSql->m_table, engine, parameterContainer)));
   return result;
}

AbstractSql::ProcessResultPointerType process_where(AbstractSql *self, const Engine &engine, const ParameterContainer &parameterContainer, QMap<QString, QString>&, QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   
}

Delete::Delete(const QString &table)
   : Delete(TableIdentifier(table))
{
}

Delete::Delete(const TableIdentifier &table)
   :m_table(table)
{
   m_specifications.insert(Delete::SPECIFICATION_DELETE, "DELETE FROM %1$s");
   m_specifications.insert(Delete::SPECIFICATION_WHERE, "WHERE %1$s");
   m_specificationFnPtrs.insert("where", process_where);
   m_specificationFnPtrs.insert("delete", process_delete);
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

Delete::~Delete()
{}

}//sql
}//db
}//corelib
}//sn