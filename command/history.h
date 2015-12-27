#ifndef SN_CORELIB_COMMAND_HISTORY_H
#define SN_CORELIB_COMMAND_HISTORY_H

#include <QString>
#include <QStringList>
#include <QDebug>

#include "global/global.h"

namespace sn{
namespace corelib{

class SN_CORELIB_EXPORT History
{
public:
   History(const QString &historyDir, const QString &group, int sizeLimit = 20);
public:
   History& addItem(const QString& command);
   History& clear();
   QString next();
   QString prev();
   QString last();
   bool isLast();
protected:
   QString m_historyFilename;
   QStringList m_items;
   int m_curIndex;
   int m_limit;
};

}//command
}//sn

#endif // SN_CORELIB_COMMAND_HISTORY_H
