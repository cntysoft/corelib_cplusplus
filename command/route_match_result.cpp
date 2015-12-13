#include <QMapIterator>

#include "route_match_result.h"

namespace sn 
{
namespace corelib 
{

RouteMatchResult::RouteMatchResult(bool status, const RouteArgsType &params, int length)
   : m_status(status),
     m_length(length),
     m_params(params)     
{}

QString RouteMatchResult::getMatchedRouteName()const
{
   return m_matchedRouteName;
}

RouteMatchResult& RouteMatchResult::setMatchedRouteName(const QString &name)
{
   if(m_matchedRouteName.isEmpty()){
      m_matchedRouteName = name;
   }else{
      m_matchedRouteName = name+'/'+m_matchedRouteName;
   }
   return *this;
}

RouteMatchResult& RouteMatchResult::setParam(const QString& name, const QString& value)
{
   m_params[name] = value;
   return *this;
}

QString RouteMatchResult::getParam(const QString& name, QString defaultValue)const
{
   if(m_params.contains(name)){
      return m_params[name];
   }
   return defaultValue;
}

RouteMatchResult::RouteArgsType RouteMatchResult::getParams()const
{
   return m_params;
}

RouteMatchResult& RouteMatchResult::merge(RouteMatchResult& routeMatchResult)
{
   RouteArgsType otherParams(routeMatchResult.getParams());
   QMapIterator<QString, QString> iterator(otherParams);
   while(iterator.hasNext()){
      iterator.next();
      if(m_params.contains(iterator.key())){
         m_params[iterator.key()] = iterator.value();
      }else{
         m_params.insert(iterator.key(), iterator.value());
      }
   }
   m_length += routeMatchResult.getLength();
   m_matchedRouteName = routeMatchResult.getMatchedRouteName();
   return *this;
}

int RouteMatchResult::getLength()const
{
   return m_length;
}

bool RouteMatchResult::getStatus()const
{
   return m_status;
}

}//corelib
}//sn