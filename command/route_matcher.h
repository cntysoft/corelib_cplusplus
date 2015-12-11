#ifndef SN_CORELIB_COMMAND_ROUTE_MATCHER_H
#define SN_CORELIB_COMMAND_ROUTE_MATCHER_H

#include <QList>
#include <QMap>

#include "global/global.h"

namespace sn 
{
namespace corelib 
{

class SN_CORELIB_EXPORT RouteMatcher
{
public:
   struct SyntaxPart
   {
      QString name;
      bool isShort;
      bool isLiteral;
      bool required;
      bool positional;
      bool hasValue;
      QList<QString> alternatives;
   };
public:
   RouteMatcher(const QString& route);
protected:
   void parseDefinition(const QString& route);
protected:
   QList<SyntaxPart> m_parts;
   QMap<QString, bool> m_defaults;
   QMap<QString, QString> m_aliases;
   
};

}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_ROUTE_MATCHER_H
