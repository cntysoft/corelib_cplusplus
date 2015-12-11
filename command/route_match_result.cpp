#include "route_match_result.h"

namespace sn 
{
namespace corelib 
{

RouteMatchResult::RouteMatchResult(const RouteArgsType &params)
   :m_params(params)
{}

QString& RouteMatchResult::getMatchedRouteName()
{
   return m_matchedRouteName;
}

RouteMatchResult& RouteMatchResult::setMatchedRouteName(const QString &name)
{
   m_matchedRouteName = name;
   return *this;
}

RouteMatchResult& RouteMatchResult::setParam(const QString &name, const QString &value)
{
   m_params[name] = value;
   return *this;
}

QString& RouteMatchResult::getParam(const QString &name, const QString &defaultValue)
{
   if(m_params.contains(name)){
      return m_params[name];
   }
   return defaultValue;
}

}//corelib
}//sn