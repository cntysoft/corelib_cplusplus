#include "route_stack.h"

namespace sn 
{
namespace corelib 
{

RouteStack& RouteStack::addRoute(const QString& name, const RouteItem& route, int priority)
{
   m_routes.insert(name, route, priority);
   return *this;
}

RouteItem& RouteStack::getRoute(const QString& name)
{
   return m_routes.get(name);
}

RouteStack& RouteStack::removeRoute(const QString &name)
{
   m_routes.remove(name);
   return *this;
}

PriorityList<RouteItem>& RouteStack::getRoutes()
{
   return m_routes;
}

bool RouteStack::hasRoute(const QString &name)
{
   return m_routes.hasItem(name);
}

RouteStack& RouteStack::setDefaultParams(const QMap<QString, QString>& params)
{
   m_defaultParams = params;
   return *this;
}

RouteStack& RouteStack::setDefaultParam(const QString& name, const QString& value)
{
   m_defaultParams[name] = value;
   return *this;
}

RouteMatchResult RouteStack::match(const QStringList& cmdArgs)
{
   m_routes.sort();
   RouteMatchResult matchResult(false);
   for(int i = 0; i < m_routes.count(); i++){
      QPair<QString, RouteItem&> routePair = m_routes.getDataPairByIndex(i);
      QStringList args(cmdArgs);
      matchResult = routePair.second.match(args);
      if(matchResult.getStatus()){
         matchResult.setMatchedRouteName(routePair.first);
         return matchResult;
      }
   }
   return matchResult;
}

}//corelib
}//sn