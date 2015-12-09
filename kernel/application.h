#ifndef SN_CORELIB_APPLICATION_H
#define SN_CORELIB_APPLICATION_H

#include <QCoreApplication>

#include "global/global.h"

QT_BEGIN_NAMESPACE
class QObject;
class QEvent;
QT_END_NAMESPACE

namespace sn 
{
namespace corelib 
{
class SN_CORELIB_EXPORT Application
{
   Q_OBJECT
public:
   Application(int &argc, char **argv);
   bool notify(QObject * receiver, QEvent * event);
   static Application *instance();
   Settings& getSettings();
private:
   Settings m_settings;
};

}//corelib
}//sn

#endif //SN_CORELIB_APPLICATION_H
