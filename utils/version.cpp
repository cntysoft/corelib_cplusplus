#include "version.h"

#include <QDebug>

namespace sn{
namespace corelib{
namespace utils{

const QMap<QString, int> Version::sm_releaseValueMap{
   {"devel", -6},
   {"alpha", -5},
   {"a", -5},
   {"beta", -4},
   {"b", -4},
   {"rc", -3},
   {"#", -2},
   {"p", -1},
   {"pl", -1}
};

const QRegularExpression Version::sm_endOfVersionRe("([^\\d\\.])"); 
const QRegularExpression Version::sm_notDigitsRe("[^\\d]");
const QRegularExpression Version::sm_plusMinusRe("[\\-+]");
const QRegularExpression Version::sm_stripRe("\\s");
const QRegularExpression Version::sm_underscoreRe("_");

Version::Version(QString version)
{
   version = version.toLower();
   m_version = version.replace(Version::sm_underscoreRe, QString(".")).replace(Version::sm_plusMinusRe, QString(""));
   int releaseStartIndex = version.indexOf(Version::sm_endOfVersionRe);
   m_shortVersion = version;
   QVariant release;
   if(releaseStartIndex != -1){
      m_release = version.mid(releaseStartIndex, version.length());
      m_shortVersion = version.mid(0, releaseStartIndex);
      if(Version::sm_releaseValueMap.contains(m_release)){
         release.setValue(Version::sm_releaseValueMap[m_release]);
      }else{
         release.setValue(Version::sm_releaseValueMap["dev"]);
      }
   }
   if(!release.isNull()){
      m_releaseValue = release.toInt();
   }else{
      m_releaseValue = Version::sm_releaseValueMap["dev"];
   }
   m_shortVersion.replace(Version::sm_notDigitsRe, QString(""));
   QStringList partsStr = version.split('.');
   for(int i = 0; i < partsStr.size(); i++){
      m_parts.append(parseVersionNumber(partsStr[i]));
   }
   if(m_parts.length() == 1){
      m_major = m_parts[0];
   }else if(m_parts.length() == 2){
      m_major = m_parts[0];
      m_minor = m_parts[1];
   }else if(m_parts.length() == 3){
      m_major = m_parts[0];
      m_minor = m_parts[1];
      m_patch = m_parts[2];
   }else if(m_parts.length() == 4){
      m_major = m_parts[0];
      m_minor = m_parts[1];
      m_patch = m_parts[2];
      m_build = m_parts[3];
   }
}

QString Version::toVersion()
{
   return m_version;
}

int Version::compareTo(const Version &version)
{
   int lhsLength = m_parts.length();
   int rhsLength = version.m_parts.length();
   int length = qMax(lhsLength, rhsLength);
   int lhs;
   int rhs;
   const QList<int> &lhsParts = m_parts;
   const QList<int> &rhsParts = version.m_parts;
   for(int i = 0; i < length; i++){
      if(i < lhsLength){
         lhs = lhsParts[i];
      }
      if(i < rhsLength){
         rhs = rhsParts[i];
      }
      if(lhs < rhs){
         return -1;
      }
      if(lhs > rhs){
         return 1;
      }
   }
   lhs = m_releaseValue;
   rhs = version.m_releaseValue;
   if (lhs < rhs) {
      return -1;
   }
   if (lhs > rhs) {
      return 1;
   }
   return 0;
}

bool Version::operator >(const Version &version)
{
   return compareTo(version) > 0;
}

bool Version::operator >=(const Version &version)
{
   return compareTo(version) >= 0;
}

bool Version::operator <(const Version &version)
{
   return compareTo(version) < 0;
}

bool Version::operator <=(const Version &version)
{
   return compareTo(version) <= 0;
}

bool Version::operator ==(const Version &version)
{
   return compareTo(version) == 0;
}

int Version::parseVersionNumber(QString part)
{
   QString str;
   for(int i = 0; i < part.length(); i++){
      if(part[i].isDigit()){
         str.append(part[i]);
      }
   }
   return str.toInt();
}

bool Version::match(const QString &version)
{
   return m_version.mid(0, version.length()) == version;
}

QString Version::getShortVersion()
{
   return m_shortVersion;
}

int Version::getMajor()
{
   return m_major;
}
int Version::getMinor()
{
   return m_minor;
}

int Version::getPatch()
{
   return m_patch;
}

int Version::getBuild()
{
   return m_build;
}

QString Version::getRelease()
{
   return m_release;
}

QList<QVariant> Version::toArray()
{
   return {getMajor(), getMinor(), getPatch(), getBuild(), getRelease()};
}

}//utils
}//corelib
}//sn
