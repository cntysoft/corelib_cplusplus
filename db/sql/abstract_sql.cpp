#include "abstract_sql.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

AbstractSql& AbstractSql::setSpecificationFn(const QString &name, SpecificationFuncPtr fn)
{
   if(!m_specificationFnPtrs.contains(name)){
      m_specificationFnPtrs.insert(name, fn);
   }
   return *this;
}

QString AbstractSql::buildSqlString(const Engine &engine, const ParameterContainer &parameterContainer)
{
   QMap<QString, QString> sqls;
   QMap<QString, ProcessResult> parameters;
   QMap<QString, QString>::const_iterator cit = m_specifications.cbegin();
   while(cit != m_specifications.cend()){
      SpecificationFuncPtr fn = nullptr;
      QString key(cit.key());
      QString specification(cit.value());
      if(m_specificationFnPtrs.contains(key)){
         fn = m_specificationFnPtrs.value(key);
      }
      Q_ASSERT_X(fn != nullptr, "AbstractSql::buildSqlString", "specification function can not be nullptr");
      parameters[key] = fn(engine, parameterContainer, sqls, parameters);
      if(!specification.isEmpty() && parameters.value(key).type == ProcessResultType::Array){
         sqls[key] = createSqlFromSpecificationAndParameters(specification, parameters);
         continue;
      }
      if(ProcessResultType::String == parameters.value(key).type){
         sqls.insert(key, parameters.value(key).stringValue);
      }
      cit++;
   }
   return sqls.values().join(' ');
}

QString AbstractSql::createSqlFromSpecificationAndParameters(const QString &specification, QMap<QString, ProcessResult> &parameters)
{
   return QString();
}

}//sql
}//db
}//corelib
}//sn
