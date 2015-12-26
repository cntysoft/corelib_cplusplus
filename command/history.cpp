#include <QFile>
#include <QTextStream>

#include "history.h"
#include "io/filesystem.h"
#include "kernel/errorinfo.h"

namespace sn{
namespace corelib{

History::History(const QString &historyDir, const QString &group, int sizeLimit)
   : m_curIndex(0),
     m_limit(sizeLimit)
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
         if(i > 9){
            break;
         }
         m_items << file.readLine();
         i++;
      }
   }
}

QString History::prev()
{
   int limit = qMin(m_items.size(), m_limit);
   if(0 == limit){
      return QString();
   }else{
      m_curIndex = m_curIndex + (limit -1) % limit;
      return m_items[m_curIndex];
   }
}

QString History::next()
{
   int limit = qMin(m_items.size(), m_limit);
   if(0 == limit){
      return QString();
   }else{
      m_curIndex = (m_curIndex + 1) % limit;
      return m_items[m_curIndex];
   }
}

History& History::addItem(const QString &command)
{
   if(m_items.size() < m_limit){
      m_items << command;
   }else{
      int index = (m_curIndex + 1) % m_limit;
      m_items[index] = command;
   }
   QFile file(m_historyFilename);
   if(!file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)){
      throw ErrorInfo(QString("open history file %1 fail").arg(m_historyFilename)); 
   }
   for(int i = 0; i < m_items.size(); i++){
      QString item(m_items[i]+"\n");
      file.write(item.toLocal8Bit());
   }
}

History& History::clear()
{
   m_items.clear();
   QFile file(m_historyFilename);
   if(file.exists()){
      file.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text);
   }
}

}//command
}//sn