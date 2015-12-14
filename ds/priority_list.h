#ifndef SN_CORELIB_DS_PRIORITY_LIST
#define SN_CORELIB_DS_PRIORITY_LIST
#include <QDebug>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QList>

#include <algorithm>

#include "global/global.h"

namespace sn
{
namespace corelib 
{
template <typename DataType>
class SN_CORELIB_EXPORT PriorityList
{
public:
   struct ListItem
   {
      DataType data;
      int priority;
      int serial;
   };
public:
   void insert(const QString& name, DataType value, int priority = 0);
   PriorityList<DataType>& setPriority(const QString& name, int priority);
   PriorityList<DataType>& remove(const QString& name);
   void clear();
   DataType& get(const QString& name, DataType defaultValue);
   DataType& get(int pos);
   int count();
   void sort();
   QList<int> getPriorityItems();
   QList<DataType> getDataItems();
public:
   DataType& operator[](int i);
protected:
   QMap<QString, ListItem> m_items;
   QStringList m_names;
   int m_serial = 0;
   int m_count = 0;
   bool m_sorted = false;
};

template <typename DataType>
void PriorityList<DataType>::insert(const QString &name, DataType value, int priority)
{
   m_sorted = false;
   m_count++;
   m_items.insert(name, {value, priority, m_serial++});
   m_names.append(name);
}

template <typename DataType>
PriorityList<DataType>& PriorityList<DataType>::setPriority(const QString& name, int priority)
{
   if(m_items.contains(name)){
      m_items[name].priority = priority;
      m_sorted = false;
   }
   return *this;
}

template <typename DataType>
PriorityList<DataType>& PriorityList<DataType>::remove(const QString& name)
{
   if(m_items.contains(name)){
      m_count--;
      m_items.remove(name);
      m_names.removeAt(m_names.indexOf(name));
   }
   return *this;
}

template <typename DataType>
void PriorityList<DataType>::clear()
{
   m_items.clear();
   m_names.clear();
   m_serial = 0;
   m_count = 0;
   m_sorted = false;
}

template <typename DataType>
DataType& PriorityList<DataType>::get(const QString& name, DataType defaultValue)
{
   if(m_items.contains(name)){
      return m_items[name].data;
   }
   return defaultValue;
}

template <typename DataType>
void PriorityList<DataType>::sort()
{
   if(!m_sorted){
      std::sort(m_names.begin(), m_names.end(), [this](const QString& leftName, const QString& rightName){
         ListItem left = this->m_items[leftName];
         ListItem right = this->m_items[rightName];
         if(left.priority == right.priority){
            return left.serial > right.serial;
         }else{
            return left.priority > right.priority;
         }
      });
      m_sorted = true;
   }
}

template <typename DataType>
DataType& PriorityList<DataType>::operator[](int i)
{ 
   Q_ASSERT_X(i >= 0 && i < m_items.size(), "PriorityList<T>::operator[]", "index out of range");
   QString name = m_names[i];
   return m_items[name].data;
}

template <typename DataType>
QList<int> PriorityList<DataType>::getPriorityItems()
{
   QList<int> ret;
   sort();
   QStringList::const_iterator it = m_names.cbegin();
   while(it != m_names.cend()){
      ret << m_items[*it].priority;
      it++;
   }
   return ret;
}

template <typename DataType>
QList<DataType> PriorityList<DataType>::getDataItems()
{
   QList<DataType> ret;
   sort();
   QStringList::const_iterator it = m_names.cbegin();
   while(it != m_names.cend()){
      ret << m_items[*it].data;
      it++;
   }
   return ret;
}

template <typename DataType>
int PriorityList<DataType>::count()
{
   return m_count;
}

}//corelib
}//sn

#endif // SN_CORELIB_DS_PRIORITY_LIST

