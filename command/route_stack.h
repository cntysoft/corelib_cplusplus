#ifndef SN_CORELIB_COMMAND_ROUTE_STACK_H
#define SN_CORELIB_COMMAND_ROUTE_STACK_H

#include <QMap>

#include "global/global.h"
#include "ds/priority_list.h"
#include "route_item.h"
#include "route_match_result.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

namespace sn{
namespace corelib{

class SN_CORELIB_EXPORT RouteStack
{
public:
   RouteStack& addRoute(const QString& name, const RouteItem& route, int priority = 1);
   RouteItem& getRoute(const QString& name);
   PriorityList<RouteItem>& getRoutes();
   bool hasRoute(const QString& name);
   RouteStack& removeRoute(const QString& name);
   RouteStack& setDefaultParams(const QMap<QString, QString>& params);
   RouteStack& setDefaultParam(const QString& name, const QString& value);
   RouteMatchResult match(const QStringList& cmdArgs);
protected:
   PriorityList<RouteItem> m_routes;
   QMap<QString, QString> m_defaultParams;
};

}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_ROUTE_STACK_H
