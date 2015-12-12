#ifndef SN_CORELIB_COMMAND_ROUTE_MATCHER_H
#define SN_CORELIB_COMMAND_ROUTE_MATCHER_H

#include <QList>
#include <QMap>
#include <QString>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

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
   struct MatchResult
   {
      bool status = true;
      QMap<QString, QString> matches;
   };

   using SyntaxPartListType = QList<SyntaxPart>;
public:
   RouteMatcher(const QString& route);
   RouteMatcher::MatchResult match(QStringList cmdArgs);
protected:
   void parseDefinition(const QString& route);
   QList<QString> getAliases(const QString& name);
   QString getCanonicalName(const QString& name);
   
protected:
   SyntaxPartListType m_parts;
   QMap<QString, bool> m_defaults;
   QMap<QString, QString> m_aliases;
};

}//corelib
}//sn

#endif // SN_CORELIB_COMMAND_ROUTE_MATCHER_H
