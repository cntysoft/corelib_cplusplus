#include "parameter_container.h"

namespace sn{
namespace corelib{
namespace db{

const QString ParameterContainer::TYPE_AUTO    = "auto";
const QString ParameterContainer::TYPE_NULL    = "null";
const QString ParameterContainer::TYPE_DOUBLE  = "double";
const QString ParameterContainer::TYPE_INTEGER = "integer";
const QString ParameterContainer::TYPE_BINARY  = "binary";
const QString ParameterContainer::TYPE_STRING  = "string";
const QString ParameterContainer::TYPE_LOB     = "lob";

ParameterContainer::ParameterContainer(const QMap<QString, QVariant> data)
{
   if(!data.empty()){
      QMap<QString, QVariant>::const_iterator it = data.cbegin();
      while(it != data.cend()){
         offsetSet(it.key(), it.value());
         it++;
      }
   }
}

ParameterContainer& ParameterContainer::offsetSet(const QString &name, const QVariant &value, const QString &errata, int maxLength)
{
   if(!m_data.contains(name)){
      m_positions << name;
   }
   m_data[name] = value;
   if(!errata.isEmpty()){
      
   }
   if(-1 != maxLength){
      
   }
   return *this;
}

const QVariant& ParameterContainer::offsetGet(const QString &name)throw(ErrorInfo)
{
   if(!m_data.contains(name)){
      throw ErrorInfo("Data does not exist for this name");
   }
   return m_data[name];
}

ParameterContainer& ParameterContainer::offsetUnset(const QString &name)
{
   if(m_data.contains(name)){
      m_data.remove(name);
   }
   return *this;
}

ParameterContainer& ParameterContainer::offsetUnset(int position)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetErrata", "position out of range");
   QString name = m_positions[position];
   return offsetUnset(name);
}


bool ParameterContainer::offsetExists(const QString &name)
{
   return m_data.contains(name);
}

ParameterContainer& ParameterContainer::offsetSetErrata(const QString &name, const QString &errata)
{
   m_errata[name] = errata;
   return *this;
}

ParameterContainer& ParameterContainer::offsetSetErrata(int position, const QString &errata)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetErrata", "position out of range");
   QString name = m_positions[position];
   m_errata[name] = errata;
   return *this;
}

const QString& ParameterContainer::offsetGetErrata(const QString &name) throw(ErrorInfo)
{
   if(!m_errata.contains(name)){
      throw ErrorInfo("Data does not exist for this name");
   }
   return m_errata[name];
}

const QString& ParameterContainer::offsetGetErrata(int position) throw(ErrorInfo)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetErrata", "position out of range");
   QString name = m_positions[position];
   return offsetGetErrata(name);
}

bool ParameterContainer::offsetHasErrata(const QString &name)
{
   return m_errata.contains(name);
}

bool ParameterContainer::offsetHasErrata(int position)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetErrata", "position out of range");
   QString name = m_positions[position];
   return offsetHasErrata(name);
}

ParameterContainer& ParameterContainer::offsetUnsetErrata(const QString &name)
{
   if(m_errata.contains(name)){
      m_errata.remove(name);
   }
   return *this;
}

ParameterContainer& ParameterContainer::offsetUnsetErrata(int position)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetErrata", "position out of range");
   QString name = m_positions[position];
   return offsetUnsetErrata(name);
}


ParameterContainer& ParameterContainer::offsetSetMaxLength(const QString &name, int maxLength)
{
   m_maxLength[name] = maxLength;
   return *this;
}

ParameterContainer& ParameterContainer::offsetSetMaxLength(int position, int maxLength)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetMaxLength", "position out of range");
   QString name = m_positions[position];
   m_maxLength[name] = maxLength;
   return *this;
}

int ParameterContainer::offsetGetMaxLength(const QString &name) throw(ErrorInfo)
{
   if(!m_maxLength.contains(name)){
      throw ErrorInfo("Data does not exist for this name");
   }
   return m_maxLength[name];
}

int ParameterContainer::offsetGetMaxLength(int position) throw(ErrorInfo)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetMaxLength", "position out of range");
   QString name = m_positions[position];
   return offsetGetMaxLength(name);
}

bool ParameterContainer::offsetHasMaxLength(const QString &name)
{
   return m_maxLength.contains(name);
}

bool ParameterContainer::offsetHasMaxLength(int position)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetMaxLength", "position out of range");
   QString name = m_positions[position];
   return offsetHasMaxLength(name);
}

ParameterContainer& ParameterContainer::offsetUnsetMaxLength(const QString &name)
{
   if(m_maxLength.contains(name)){
      m_maxLength.remove(name);
   }
   return *this;
}

ParameterContainer& ParameterContainer::offsetUnsetMaxLength(int position)
{
   Q_ASSERT_X(position >= 0 && position < m_positions.length(), "ParameterContainer::offsetSetMaxLength", "position out of range");
   QString name = m_positions[position];
   return offsetUnsetMaxLength(name);
}

const QMap<QString, QVariant>& ParameterContainer::getNamedDataMap()
{
   return m_data;
}

QList<QVariant> ParameterContainer::getPositionalDataList()
{
   return m_data.values();
}

int ParameterContainer::count()const
{
   return m_data.size();
}

ParameterContainer& ParameterContainer::merge(const ParameterContainer &params)
{
   if(0 == params.count()){
      return *this;
   }
   QMap<QString, QVariant>::const_iterator it = params.m_data.cbegin();
   QMap<QString, QVariant>::const_iterator end = params.m_data.cend();
   while(it != end){
      offsetSet(it.key(), it.value());
      it++;
   }
   return *this;
}

}//db
}//corelib
}//sn
