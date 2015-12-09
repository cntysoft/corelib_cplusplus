#ifndef SN_CORELIB_SETTINGS_H
#define SN_CORELIB_SETTINGS_H

#include <QSettings>
#include <QVariant>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

namespace sn 
{
namespace corelib 
{

class SN_CORELIB_EXPORT Settings
{
public:
   using Status = QSettings::Status;
public:
   Settings();
   ~Settings();
   static const QString& getCfgDir();
   static const QString& getCfgFilename();
   void sync();
   Status getStatus() const;
   QVariant getValue(const QString& key, const QString& group = CFG_GROUP_GLOABL, const QVariant & defaultValue = QVariant()) const;
   void setValue(const QString& key, const QVariant& value, const QString& group = CFG_GROUP_GLOABL);
   QStringList getChildKeys(const QString& path = QString());
private:
   QSettings* createQSettings();
   void initDefaultConf();
   int enterGroup(const QString& path);
   void exitGroup(int depth);
private:  
   QSettings* const m_settings;
};

}//corelib
}//sn

#endif // SN_CORELIB_SETTINGS_H
