#ifndef SN_CORELIB_COMMAND_ROUTE_ITEM_H
#define SN_CORELIB_COMMAND_ROUTE_ITEM_H

#include "global/global.h"
#include "route_match_result.h"

namespace sn
{
namespace corelib
{

class RouteMatcher;

class SN_CORELIB_EXPORT RouteItem
{
public:
   RouteItem();
   RouteItem(const QString& route, 
             QMap<QString, QString> defaults = QMap<QString, QString>(), 
             QMap<QString, QString> aliases = QMap<QString, QString>(), 
             QMap<QString, QString> constraints = QMap<QString, QString>());
   RouteItem(const RouteItem& other);
   RouteMatchResult match(QStringList& cmdArgs);
   ~RouteItem();
protected:
   RouteMatcher* m_matcher;
};

}//corelib
}//sn
#endif // SN_CORELIB_COMMAND_ROUTE_ITEM_H
