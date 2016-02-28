#ifndef SN_CORELIB_UTILS_VERSION_H
#define SN_CORELIB_UTILS_VERSION_H

#include <QMap>
#include <QChar>
#include <QRegularExpression>
#include <QString>
#include <QVariant>
#include <QList>

#include "global/global.h"

namespace sn{
namespace corelib{
namespace utils{

class SN_CORELIB_EXPORT Version
{
public:
   Version(QString version);
   QString toVersion();
   int getMajor();
   int getMinor();
   int getPatch();
   int getBuild();
   QString getRelease();
   int getReleaseValue();
   QString getShortVersion();
   bool match(const QString &version);
   QList<QVariant> toArray();
public:
   bool operator >(const Version &version);
   bool operator >=(const Version &version);
   bool operator <(const Version &version);
   bool operator <=(const Version &Version);
   bool operator ==(const Version &version);
protected:
   int parseVersionNumber(QString part);
   int compareTo(const Version &version);
protected:
   const static QMap<QString, int> sm_releaseValueMap;
   const static QRegularExpression sm_endOfVersionRe;
   const static QRegularExpression sm_notDigitsRe;
   const static QRegularExpression sm_plusMinusRe;
   const static QRegularExpression sm_stripRe;
   const static QRegularExpression sm_underscoreRe;
protected:
   QString m_version;
   QString m_shortVersion;
   QString m_release;
   int m_releaseValue;
   int m_major = -1;
   int m_minor = -1;
   int m_patch = -1;
   int m_build = -1;
   QList<int> m_parts;
};

}//utils
}//corelib
}//sn


#endif // SN_CORELIB_UTILS_VERSION_H
