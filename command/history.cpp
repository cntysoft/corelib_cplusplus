#include <QFile>
#include <QTextStream>

#include "history.h"
#include "io/filesystem.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{

History::History(const QString &historyDir, const QString &group, int sizeLimit)
   : m_limit(sizeLimit)
{
   if(!Filesystem::dirExist(historyDir) && !Filesystem::createPath(historyDir)){
      throw ErrorInfo(QString("create history directory %1 fail").arg(historyDir));
   }
   m_historyFilename = historyDir+"/" + group + ".txt";
   QFile file(m_historyFilename);
   if(file.exists()){
      if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
         throw ErrorInfo(QString("open history file %1 fail").arg(m_historyFilename));
      }
      int i = 0;
      while(!file.atEnd()){
         if(i > sizeLimit){
            break;
         }
         QByteArray cur(file.readLine());
         cur.remove(cur.count() - 1, 1);
         if(!m_items.isEmpty() && m_items.last() != cur){
            m_items << cur;
         }else if(m_items.isEmpty()){
            m_items << cur;
         }
         i++;
      }
   }
   m_curIndex = qMax(0, m_items.size() - 1);
}

QString History::prev()
{
   if(m_items.isEmpty()){
      return QString();
   }else{
      QString command = m_items[m_curIndex];
      m_curIndex = qMax(m_curIndex - 1, 0);
      return command;
   }
}

QString History::next()
{
   if(m_items.isEmpty()){
      return QString();
   }else{
      m_curIndex = qMin(m_curIndex + 1, m_items.size() - 1);
      return m_items[m_curIndex];
   }
}

QString History::last()
{
   return m_items.last();
}

bool History::isLast()
{
   return m_curIndex == (m_items.size() - 1);
}

History& History::addItem(const QString &command)
{
   bool needWriteToFile = false;
   if(m_items.isEmpty() || (m_items.size() < m_limit && m_items.last() != command)){
      m_items << command;
      needWriteToFile = true;
   }else if(m_items.size() == m_limit && m_items.last() != command){
      m_items.removeFirst();
      m_items << command;
      needWriteToFile = true;
   }
   if(!needWriteToFile){
      return *this;
   }
   QFile file(m_historyFilename);
   if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)){
      throw ErrorInfo(QString("open history file %1 fail").arg(m_historyFilename)); 
   }
   for(int i = 0; i < m_items.size(); i++){
      QString item(m_items[i]+"\n");
      file.write(item.toLocal8Bit());
   }
   m_curIndex = m_items.size() - 1;
   return *this;
}

History& History::clear()
{
   m_items.clear();
   QFile file(m_historyFilename);
   if(file.exists()){
      file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
   }
   return *this;
}

}//command
}//sn