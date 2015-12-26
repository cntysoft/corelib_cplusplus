#ifndef SN_CORELIB_SETTINGS_H
#define SN_CORELIB_SETTINGS_H

#include <QSettings>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QString;
class QVariant;
QT_END_NAMESPACE

namespace sn {
namespace corelib{

class SN_CORELIB_EXPORT Settings
{
public:
   using Status = QSettings::Status;
   using CfgInitializerFnType = void (*)(Settings& settings);
public:
   Settings(const QString& filename, CfgInitializerFnType initializer = nullptr);
   const QString& getCfgFilename();
   void sync();
   Status getStatus() const;
   QVariant getValue(const QString& key, const QString& group = "Global", const QVariant& defaultValue = QVariant()) const;
   void setValue(const QString& key, const QVariant& value, const QString& group = "Global");
   bool hasKey(const QString& key, const QString& group = "Global");
   QStringList getChildKeys(const QString& path = QString());
   ~Settings();
private:
   QSettings* createQSettings();
   int enterGroup(const QString& path);
   void exitGroup(int depth);
private:
   QString m_cfgFilename;
   QSettings* const m_settings;
private:
   Q_DISABLE_COPY(Settings)
};

}//corelib
}//sn

#endif // SN_CORELIB_SETTINGS_H
