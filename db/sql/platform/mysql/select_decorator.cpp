#include "select_decorator.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{
namespace platform{
namespace mysql{

AbstractSql::ProcessResultPointerType selectdecorator_process_limit(AbstractSql *self,const Engine &engine, 
                                                           ParameterContainer* parameterContainer, QMap<QString, QString>&, 
                                                           QMap<QString, AbstractSql::ProcessResultPointerType>&)
{
   SelectDecorator* selectSql = dynamic_cast<SelectDecorator*>(self);
   Q_ASSERT_X(selectSql != 0, "limit friend function selectdecorator_process_limit", "self pointer cast fail");
   if(0 == selectSql){
      throw ErrorInfo(QString("limit friend function selectdecorator_process_limit self pointer cast fail"));
   }
   QSharedPointer<AbstractSql::ProcessResult> result(new AbstractSql::ProcessResult);
   
   if(selectSql->m_limit.isNull() && !selectSql->m_offset.isNull()){
      result->type = AbstractSql::ProcessResultType::Array;
      result->isNull = false;
      result->value.setValue(QList<QVariant>{""});
      return result;
   }
   if(selectSql->m_limit.isNull()){
      result->isNull = true;
      return result;
   }
   result->isNull = false;
   result->type = AbstractSql::ProcessResultType::Array;
   if(nullptr != parameterContainer){
      parameterContainer->offsetSet("limit", selectSql->m_limit.toInt(), ParameterContainer::TYPE_INTEGER);
      result->value = QVariant(QList<QVariant>{QVariant(engine.formatParameterName("limit"))});
   }else{
      result->value = QVariant(QList<QVariant>{QVariant(engine.quoteValue(selectSql->m_limit.toInt()))});
   }
   return result;
}

SelectDecorator::SelectDecorator(const QString &table, const QString &schema)
   : SelectDecorator(TableIdentifier(table, schema))
{
}

SelectDecorator::SelectDecorator(const TableIdentifier &table)
   : Select(table)
{
   m_specificationFnPtrs.insert(Select::LIMIT, selectdecorator_process_limit);
//   m_specificationFnPtrs.insert(Select::OFFSET, select_process_offset);
   m_isNeedLocalizeVariables = true;
}

void SelectDecorator::localizeVariables()
{
   if(!m_isNeedLocalizeVariables || m_subject.isNull()){
      return;
   }
   Select::localizeVariables();
   if(m_limit.isNull() && !m_offset.isNull()){
      m_specifications[Select::LIMIT] = "LIMIT 18446744073709551615";
   }
}

}//mysql
}//platform
}//sql
}//db
}//corelib
}//sn
