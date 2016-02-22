#ifndef UPGRADE_TEST_UPGRADEENV_H
#define UPGRADE_TEST_UPGRADEENV_H

#include <QObject>
#include <QString>
#include "upgrade/upgradeenv.h"
#include "kernel/errorinfo.h"

namespace corelibtest{
namespace upgrade{

using sn::corelib::ErrorInfo;

using sn::corelib::upgrade::UpgradeEnv;

class TestUpgradeEnv : public QObject
{
   Q_OBJECT
public:
   TestUpgradeEnv();
private slots:
   void initTestCase();
   void testUpgradeEnv();
public slots:
   void writeLogMsg(const QString &msg);
   void exceptionHandler(ErrorInfo error);
protected:
   QString m_dataDir;
   UpgradeEnv m_upgradeEnv;
};

}//upgrade
}//corelibtest

#endif // UPGRADE_TEST_UPGRADEENV_H
