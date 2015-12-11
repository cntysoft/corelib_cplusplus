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
   QRegularExpression regex2("\\G(?P<name>[A-Z][A-Z0-9\\_\\-]*?)(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex3("\\G\\[ *?(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-\\:]*?) *?\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex4("\\G\\[ *\\<(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-]*?)\\> *\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex5("\\G\\< *(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-]*?) *\\>(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex6("\\G(?P<name>[a-zA-Z][a-zA-Z0-9\\_\\-\\:]*?)(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex7("\\G--(?P<name>[a-zA-Z0-9][a-zA-Z0-9\\_\\-]+)(?P<hasValue>=\\S*?)?(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex8("\\G\\[ *?--(?P<name>[a-zA-Z0-9][a-zA-Z0-9\\_\\-]+) *?\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex9("\\G\\[ *?--(?P<name>[a-zA-Z0-9][a-zA-Z0-9\\_\\-]+)(?P<hasValue>=\\S*?)? *?\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex10("\\G-(?P<name>[a-zA-Z0-9])(?:=(?P<type>[ns]))?(?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex11("\\G\\[ *?-(?P<name>[a-zA-Z0-9])(?:=(?P<type>[ns]))? *?\\](?: +|$)", QRegularExpression::DotMatchesEverythingOption);
   QRegularExpression regex12("\\G"
                              "\\["
                                  "(?P<options>"
                                      "(?:"
                                          "\\ *?"
                                          "(?P<name>[a-zA-Z][a-zA-Z0-9_\\-]*?)"
                                          "\\ *?"
                                          "(?:\\||(?=\\]))"
                                          "\\ *?"
                                      ")+"
                                  ")"
                              "\\]"
                              "(?:\\:(?P<groupName>[a-zA-Z0-9]+))?"
                              "(?:\\ +|$)"
                              , QRegularExpression::DotMatchesEverythingOption|QRegularExpression::ExtendedPatternSyntaxOption);
   QRegularExpression regex13("\\G"
                              "\\("
                                  "(?P<options>"
                                      "(?:"
                                          "\\ *?"
                                          "(?P<name>[a-zA-Z][a-zA-Z0-9_\\-]+)"
                                          "\\ *?"
                                          "(?:\\||(?=\\)))"
                                          "\\ *?"
                                      ")+"
                                  ")"
                              "\\)"
                              "(?:\\:(?P<groupName>[a-zA-Z0-9]+))?"
                              "(?:\\ +|$)"
                              , QRegularExpression::DotMatchesEverythingOption|QRegularExpression::ExtendedPatternSyntaxOption);
   QRegularExpression regex14("\\G"
                              "\\("
                                  "(?P<options>"
                                      "(?:"
                                          "\\ *?"
                                          "\\-+(?P<name>[a-zA-Z0-9][a-zA-Z0-9_\\-]*?)"
                                          "\\ *?"
                                          "(?:\\||(?=\\)))"
                                          "\\ *?"
                                      ")+"
                                  ")"
                              "\\)"
                              "(?:\\:(?P<groupName>[a-zA-Z0-9]+))?"
                              "(?:\\ +|$)"
                              , QRegularExpression::DotMatchesEverythingOption|QRegularExpression::ExtendedPatternSyntaxOption);
   QRegularExpression regex15("\\G"
                              "\\["
                                  "(?P<options>"
                                      "(?:"
                                          "\\ *?"
                                          "\\-+(?P<name>[a-zA-Z0-9][a-zA-Z0-9_\\-]*?)"
                                          "\\ *?"
                                          "(?:\\||(?=\\]))"
                                          "\\ *?"
                                      ")+"
                                  ")"
                              "\\]"
                              "(?:\\:(?P<groupName>[a-zA-Z0-9]+))?"
                              "(?:\\ +|$)"
                              , QRegularExpression::DotMatchesEverythingOption|QRegularExpression::ExtendedPatternSyntaxOption);
   
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
      else if((match = regex7.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isShort = false;
         item.isLiteral = false;
         item.required = true;
         item.positional = false;
         item.hasValue = !match.captured("hasValue").isEmpty();
      }
      /**
        * Optional long flag
        *    [--param]
        */
      else if((match = regex8.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isShort = false;
         item.isLiteral = false;
         item.required = false;
         item.positional = false;
         item.hasValue = false;
      }
      /**
        * Optional long param
        *    [--param=]
        *    [--param=whatever]
        */
      else if((match = regex9.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isShort = false;
         item.isLiteral = false;
         item.required = false;
         item.positional = false;
         item.hasValue = !match.captured("hasValue").isEmpty();
      }
      /**
        * Mandatory short param
        *    -a
        *    -a=i
        *    -a=s
        *    -a=w
        */
      else if((match = regex10.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isShort = true;
         item.isLiteral = false;
         item.required = true;
         item.positional = false;
         item.hasValue = !match.captured("type").isEmpty() ? true : false;//这里是n还是s呢?
      }
      /**
        * Optional short param
        *    [-a]
        *    [-a=n]
        *    [-a=s]
        */
      else if((match = regex11.match(def, pos)).hasMatch()){
         item.name = match.captured("name").toLower();
         item.isShort = true;
         item.isLiteral = false;
         item.required = false;
         item.positional = false;
         item.hasValue = !match.captured("type").isEmpty() ? true : false;//这里是n还是s呢?
      }
      /**
        * Optional literal param alternative
        *    [ something | somethingElse | anotherOne ]
        *    [ something | somethingElse | anotherOne ]:namedGroup
        */
      else if((match = regex12.match(def, pos)).hasMatch()){
         QList<QString> options = match.captured("options").trimmed().split(QRegularExpression(" *\\| *"), QString::SkipEmptyParts);
         QList<QString>::const_iterator it = options.cbegin();
         while(it != options.cend()){
            if(!item.alternatives.contains(*it)){
               item.alternatives.push_back(*it);
            }
            it++;
         }
         item.name = !match.captured("groupName").isEmpty() ? match.captured("groupName") : QString("unnamedGroup")+unnamedGroupCounter++;
         item.isLiteral = true;
         item.required = false;
         item.positional = true;
         item.hasValue = false;
      }
      /**
        * Required literal param alternative
        *    ( something | somethingElse | anotherOne )
        *    ( something | somethingElse | anotherOne ):namedGroup
        */
      else if((match = regex13.match(def, pos)).hasMatch()){
         QList<QString> options = match.captured("options").trimmed().split(QRegularExpression(" *\\| *"), QString::SkipEmptyParts);
         QList<QString>::const_iterator it = options.cbegin();
         while(it != options.cend()){
            if(!item.alternatives.contains(*it)){
               item.alternatives.push_back(*it);
            }
            it++;
         }
         item.name = !match.captured("groupName").isEmpty()? match.captured("groupName") : QString("unnamedGroupAt")+unnamedGroupCounter++;
         item.isLiteral = true;
         item.required = true;
         item.positional = true;
         item.hasValue = false;
      }
      /**
        * Required long/short flag alternative
        *    ( --something | --somethingElse | --anotherOne | -s | -a )
        *    ( --something | --somethingElse | --anotherOne | -s | -a ):namedGroup
        */
      else if((match = regex14.match(def, pos)).hasMatch()){
         QList<QString> options = match.captured("options").trimmed().split(QRegularExpression(" *\\| *"), QString::SkipEmptyParts);
         QList<QString>::const_iterator it = options.cbegin();
         while(it != options.cend()){
            QString opt(*it);
            opt = opt.replace(QRegularExpression("^-+"), "");
            if(!item.alternatives.contains(opt)){
               item.alternatives.push_back(opt);
            }
            it++;
         }
         item.name = !match.captured("groupName").isEmpty()? match.captured("groupName") : QString("unnamedGroupAt")+unnamedGroupCounter++;
         item.isLiteral = false;
         item.required = true;
         item.positional = false;
         item.hasValue = false;
      }
      /**
        * Optional flag alternative
        *    [ --something | --somethingElse | --anotherOne | -s | -a ]
        *    [ --something | --somethingElse | --anotherOne | -s | -a ]:namedGroup
        */
      else if((match = regex15.match(def, pos)).hasMatch()){
         QList<QString> options = match.captured("options").trimmed().split(QRegularExpression(" *\\| *"), QString::SkipEmptyParts);
         QList<QString>::const_iterator it = options.cbegin();
         while(it != options.cend()){
            QString opt(*it);
            opt = opt.replace(QRegularExpression("^-+"), "");
            if(!item.alternatives.contains(opt)){
               item.alternatives.push_back(opt);
            }
            it++;
         }
         item.name = !match.captured("groupName").isEmpty()? match.captured("groupName") : QString("unnamedGroupAt")+unnamedGroupCounter++;
         item.isLiteral = false;
         item.required = false;
         item.positional = false;
         item.hasValue = false;
      }
      else{
         throw ErrorInfo(QString("Cannot understand Console route at \"%1\"").arg(def.mid(pos)));
      }
      pos += match.capturedTexts().first().size();
      m_parts.push_back(item);
   }
}

QList<QString> RouteMatcher::getAliases(const QString &name)
{
   QList<QString> aliases{
      name
   };
   QMap<QString, QString>::const_iterator it = m_aliases.cbegin();
   while(it != m_aliases.cend()){
      if(name == it.value()){
         aliases << it.key();
      }
   }
   return aliases;
}

QString RouteMatcher::getCanonicalName(const QString &name)
{
   if(m_aliases.contains(name)){
      return m_aliases.value(name);
   }
   return name;
}

}//corelib
}//sn