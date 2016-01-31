#include "drop_table.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace ddl{

const QString DropTable::TABLE = "table";

AbstractSql::ProcessResultPointerType droptable_process_table(AbstractSql *self,const Engine &engine, 
                                                              ParameterContainer*, QMap<QString, QString>&, 
                                                              QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   DropTable* dropTableSql = dynamic_cast<DropTable*>(self);
   Q_ASSERT_X(dropTableSql != 0, "friend function droptable_process_table", "self pointer cast failure");
   if(0 == dropTableSql){
      throw ErrorInfo(QString("friend function droptable_process_table self pointer cast failure"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   result->value.setValue(QList<QVariant>{
                               engine.quoteIdentifierInFragment(dropTableSql->m_table.toString())
                            });
   return result;
}

DropTable::DropTable(const QString &table, const QString &schema)
   : DropTable(TableIdentifier(table, schema))
{
}

DropTable::DropTable(const TableIdentifier &table)
   : m_table(table)
{
   m_specifications.insert(DropTable::TABLE, "DROP TABLE %s");
   m_specificationFnPtrs.insert(DropTable::TABLE, droptable_process_table);
   m_specKeys.append(DropTable::TABLE);
}


}//ddl
}//sql
}//db
}//corelib
}//sn
