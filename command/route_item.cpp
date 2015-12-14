#include <QStringList>

#include "route_matcher.h"
#include "route_item.h"

namespace sn 
{
namespace corelib 
{
RouteItem::RouteItem()
{
   m_matcher = new RouteMatcher;
}

RouteItem::RouteItem(const QString& route, QMap<QString, QString> defaults, QMap<QString, QString> aliases, 
                     QMap<QString, QString> constraints)
{
   m_matcher = new RouteMatcher(route, defaults, aliases, constraints);
}

RouteItem::RouteItem(const RouteItem &other)
{
   if(this != &other){
      m_matcher = new RouteMatcher(*other.m_matcher);
   }
}

RouteMatchResult RouteItem::match(QStringList& cmdArgs)
{
   RouteMatcher::MatchResult matchResult = m_matcher->match(cmdArgs);
   return RouteMatchResult(matchResult.status, matchResult.matches);
}

RouteItem::~RouteItem()
{
   delete m_matcher;
}

}//corelib
}//sn