#include <QRegularExpression>
#include <QDebug>

#include "route_matcher.h"
#include "kernel/errorinfo.h"

namespace sn 
{
namespace corelib 
{

RouteMatcher::RouteMatcher(const QString &route)
{
   parseDefinition(route);
}

void RouteMatcher::parseDefinition(const QString &route)
{
   QString def(route.trimmed());
   int pos = 0;
   int length = route.size();
   int unnamedGroupCounter = 1;
   QRegularExpression regex1("\\G\\[(?P<name>[A-Z][A-Z0-9\\_\\-]*?)\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex2("\\G(?P<name>[A-Z][A-Z0-9\_\-]*?)(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex3("\\G\\[ *?(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-\\:]*?) *?\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex4("\\G\\[ *\\<(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-]*?)\\> *\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex5("\\G\\< *(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-]*?) *\\>(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex6("\\G(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-\\:]*?)(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex7("\\G--(?P<name>[a-zA-Z0-9][a-zA-Z0-9\\_\\-]+)(?P<hasValue>=\\S*?)?(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   while(pos < length){
      SyntaxPart item;
      /**
        * Optional value param, i.e.
        *    [SOMETHING]
       */
      QRegularExpressionMatch match;
      if((match = regex1.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isLiteral = false;
         item.required = false;
         item.positional = true;
         item.hasValue = true;
      }
      /**
       * Mandatory value param, i.e.
       *   SOMETHING
       */
      else if((match = regex2.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isLiteral = false;
         item.required = true;
         item.positional = true;
         item.hasValue = true;
      }
      /**
        * Optional literal param, i.e.
        *    [something]
        */
      else if((match = regex3.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isLiteral = true;
         item.required = false;
         item.positional = true;
         item.hasValue = false;
      }
      /**
        * Optional value param, syntax 2, i.e.
        *    [<something>]
        */
      else if((match = regex4.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isLiteral = false;
         item.required = false;
         item.positional = true;
         item.hasValue = true;
      }
      /**
        * Mandatory value param, i.e.
        *    <something>
        */
      else if((match = regex5.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isLiteral = false;
         item.required = true;
         item.positional = true;
         item.hasValue = true;
      }
      /**
        * Mandatory literal param, i.e.
        *   something
        */
      else if((match = regex6.match(def, pos)).hasMatch())
      {
         item.name = match.captured("name").toLower();
         item.isLiteral = true;
         item.required = true;
         item.positional = true;
         item.hasValue = false;
         
      }
      /**
        * Mandatory long param
        *    --param=
        *    --param=whatever
        */
      else if((match = regex6.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isShort = false;
         item.isLiteral = false;
         item.required = true;
         item.positional = false;
         item.hasValue = !match.captured("hasValue").isEmpty();
      }
      else{
         throw ErrorInfo(QString("Cannot understand Console route at \"%1\"").arg(def.mid(pos)));
      }
      pos += match.capturedTexts().first().size();
      m_parts.push_back(item);
   }
   
}

}//corelib
}//sn