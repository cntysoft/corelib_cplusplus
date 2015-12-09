#include <QVariant>
#include <QFile>

#include "settings.h"
#include "stddir.h"

namespace sn 
{
namespace corelib 
{

Settings::Settings(const QString& filename, CfgInitializerFnType initializer)
   : m_cfgFilename(StdDir::getSysConfDir()+"/"+filename+"/"), 
     m_settings(createQSettings())
{
   QFile cfgFilename(m_cfgFilename);
   if(!cfgFilename.exists()){
      if(nullptr != initializer){
         initializer(*this);
      }
   }
}

const QString& Settings::getCfgFilename()
{
   return m_cfgFilename;
}

QSettings* Settings::createQSettings()
{
   return new QSettings(m_cfgFilename, QSettings::Format::NativeFormat);
}


QStringList Settings::getChildKeys(const QString& path)
{
   QStringList keys;
   if(!path.isEmpty()){
      int depth = enterGroup(path);
      keys = m_settings->childKeys();
      exitGroup(depth);
   }else{
      keys = m_settings->childKeys();
   }
   return keys;  
}

Settings::Status Settings::getStatus()const
{
   return m_settings->status();
}

QVariant Settings::getValue(const QString &key, const QString &group, const QVariant &defaultValue) const
{
   m_settings->beginGroup(group);
   QVariant value = m_settings->value(key, defaultValue);
   m_settings->endGroup();
   return value;
}

void Settings::setValue(const QString &key, const QVariant &value, const QString &group)
{
   m_settings->beginGroup(group);
   m_settings->setValue(key,value);
   m_settings->endGroup();
}

int Settings::enterGroup(const QString &path)
{
   QStringList parts = path.split('.');
   auto it = parts.cbegin();
   while(it != parts.cend()){
      m_settings->beginGroup(*it);
      it++;
   }
   return parts.count();
}

void Settings::exitGroup(int depth)
{
   for(int i = 0; i < depth; i++){
      m_settings->endGroup();
   }
}

void Settings::sync()
{
   m_settings->sync();
}

Settings::~Settings()
{
   delete m_settings;
}

}//corelib
}//sn
