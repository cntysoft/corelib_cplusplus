#include <QStringList>

#include "route_matcher.h"
#include "simple_route_item.h"

namespace sn 
{
namespace corelib 
{

SimpleRouteItem::SimpleRouteItem(const QString& route)
{
   m_matcher = new RouteMatcher(route);
}

RouteMatchResult SimpleRouteItem::match(QStringList cmdArgs)
{
   RouteMatcher::MatchResult matchResult = m_matcher->match(cmdArgs);
   return RouteMatchResult(matchResult.status, matchResult.matches);
}

SimpleRouteItem::~SimpleRouteItem()
{
   delete m_matcher;
}

}//corelib
}//sn