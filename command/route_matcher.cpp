#include <QRegularExpression>
#include <QStringList>
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

RouteMatcher::MatchResult RouteMatcher::match(QStringList cmdArgs)
{
   MatchResult matches;
   SyntaxPartListType positional;
   SyntaxPartListType named;
   SyntaxPartListType::const_iterator iterator = m_parts.cbegin();
   while(iterator != m_parts.cend()){
      if((*iterator).positional){
         positional << *iterator;
      }else{
         named << *iterator;
      }
      iterator++;
   }
   iterator = named.cbegin();
   for(int i = 0; i < named.count(); i++){
      SyntaxPart part = named[i];
      QString regexStr;
      QRegularExpression regex;
      if(!part.alternatives.isEmpty()){
         // an alternative of flags
         regexStr = "^\\-+(?P<name>";
         QStringList alternativeAliases;
         QStringList::const_iterator it = part.alternatives.cbegin();
         while(it != part.alternatives.cend()){
            alternativeAliases <<  "(?:" + getAliases(*it).join('|') + ")";
            it++;
         }
         regexStr += alternativeAliases.join('|');
         if(part.hasValue){
            regexStr += ")(?:\\=(?P<value>.*?)$)?$";
            regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
         }else{
            regexStr += ")$";
         }
      }else{
         // a single named flag
         QString name("(?:" + getAliases(part.name).join('|') + ")");
         if(part.isShort){
            // short variant
            if(part.hasValue){
               regexStr = "^\\-" + name + "(?:\\=(?P<value>.*?)$)?$";
            }else{
               regexStr = "^\\-" + name + "$";
            }
            
         }else{
            // long variant
            if(part.hasValue){
               regexStr = "^\\-{2,}" + name + "(?:\\=(?P<value>.*?)$)?$";
            }else{
               regexStr = "^\\-{2,}" + name + "$";
            }
         }
         regex.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
      }
      //Look for param
      QString value;
      QString param;
      QString matchedName;
      regex.setPattern(regexStr);
      for(int x = 0; x < cmdArgs.count(); x++){
         QRegularExpressionMatch match = regex.match(cmdArgs[x]);
         if(match.hasMatch()){
            param = cmdArgs[x];
            cmdArgs.removeAt(x);
            if(!match.captured("value").isEmpty()){
               value = match.captured("value");
            }
            if(!match.captured("name").isEmpty()){
               matchedName = getCanonicalName(match.captured("name"));
            }
            break;
         }
      }
      if(param.isEmpty()){
         if(part.required){
            matches.status = false;
            return matches;
         }else{
            continue;
         }
      }
      //Value for flags is always boolean
      if(!param.isEmpty() && !part.hasValue){
         value = "true";
      }
      //Try to retrieve value if it is expected
      if(value.isEmpty() && part.hasValue){
         if((i < cmdArgs.count()+1)){
            value = cmdArgs[i];
            cmdArgs.removeAt(i);
         }else{
            // there are no more params available
            matches.status = false;
            return matches;
         }
      }
      //Validate the value against constraints
      if(part.hasValue && m_constraints.contains(part.name)){
         QRegularExpression constraintRegex(m_constraints[part.name]);
         if(!constraintRegex.match(value).hasMatch()){
            // constraint failed
            matches.status = false;
            return matches;
         }
      }
      //Store the value
      if(part.hasValue){
         matches.matches[part.name] = value;
      }else{
         matches.matches[part.name] = "true";
      }
      //If there are alternatives, fill them
      if(!part.alternatives.isEmpty()){
         if(part.hasValue){
            QStringList::const_iterator it = part.alternatives.cbegin();
            while(it != part.alternatives.cend()){
               QString altName = *it;
               if(altName == matchedName && !matches.matches.contains(altName)){
                  matches.matches[altName] = value;
               }else if(!matches.matches.contains(altName)){
                  matches.matches[altName] = "null";
               }
               it++;
            }
         }else{
            QStringList::const_iterator it = part.alternatives.cbegin();
            while(it != part.alternatives.cend()){
               QString altName = *it;
               if(altName == matchedName && !matches.matches.contains(altName)){
                  matches.matches[altName] = m_defaults.contains(altName) ? m_defaults[altName] : "true";
               }else if(!matches.matches.contains(altName)){
                  matches.matches[altName] = "false";
               }
               it++;
            }
         }
      } 
   }
   //Scan for left-out flags that should result in a mismatch
   QStringList::const_iterator it = cmdArgs.cbegin();
   QRegularExpression stripMinusRegex("^\\-+");
   while(it != cmdArgs.cend()){
      if(stripMinusRegex.match(*it).hasMatch()){
         //there is an unrecognized flag
         matches.status = false;
      }
      it++;
   }
   //Go through all positional params
   int argPos = 0;
   for(int i = 0; i < positional.count(); i++){
      SyntaxPart part = positional[i];
      QString value;
      //Check if param exists
      if(cmdArgs[argPos].isEmpty()){
         if(part.required){
            matches.status = false;
            return matches;
         }else{
            //stop matching
            break;
         }
      }
      value = cmdArgs[argPos];
      //Check if literal param matches
      if(part.isLiteral){
         if((!part.alternatives.isEmpty() && !part.alternatives.contains(value)) ||
               (part.alternatives.isEmpty() && value != part.name)){
            matches.status = false;
            return matches;
         }
      }
      //Validate the value against constraints
      if(part.hasValue && m_constraints.contains(part.name)){
         QRegularExpression constraintRegex(m_constraints[part.name]);
         if(!constraintRegex.match(value).hasMatch()){
            // constraint failed
            matches.status = false;
            return matches;
         }
      }
      //Store the value
      if(part.hasValue){
         matches.matches[part.name] = value;
      }else if(!part.alternatives.isEmpty()){
         QStringList::const_iterator it = part.alternatives.cbegin();
         while(it != part.alternatives.cend()){
            QString altName = *it;
            if(altName == value){
               matches.matches[altName] = m_defaults.contains(altName) ? m_defaults[altName] : "true";
            }else{
               matches.matches[altName] = "false";
            }
            it++;
         }
         //set alternatives group value
         matches.matches[part.name] = value;
      }else if(!part.required){
         //set optional parameter flag
         QString name = part.name;
         matches.matches[name] = m_defaults.contains(name) ? m_defaults[name] : "true";
      }
      //Advance to next argument
      argPos++;
   }
   //Check if we have consumed all positional parameters
   if(argPos < cmdArgs.size()){
      matches.status = false;
      return matches;
   }
   //Any optional flags that were not entered have value false
   for(int i = 0; i < m_parts.size(); i++){
      SyntaxPart part = m_parts[i];
      if(!part.required && !part.hasValue){
         if(!matches.matches.contains(part.name)){
            matches.matches[part.name] = "false";
         }
         //unset alternatives also should be false
         if(!part.alternatives.isEmpty()){
            QStringList::const_iterator it = part.alternatives.cbegin();
            while(it != part.alternatives.cend()){
               QString altName = *it;
               if(!matches.matches.contains(altName)){
                  matches.matches[altName] = "false";
               }
               it++;
            }
         }
      }
   }
   QMap<QString, QString> defaults = m_defaults;
   QMap<QString, QString>::const_iterator defaultsIt = defaults.cbegin();
   while(defaultsIt != defaults.cend()){
      QString argName = *defaultsIt;
      if(!matches.matches.contains(argName)){
          matches.matches[argName] = defaults[argName];
      }
      defaultsIt++;
   }
   return matches;
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