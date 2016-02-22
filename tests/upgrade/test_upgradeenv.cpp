#include "test_upgradeenv.h"
#include <QtTest/QtTest>
#include <QDebug>

namespace corelibtest{
namespace upgrade{

TestUpgradeEnv::TestUpgradeEnv()
   : m_dataDir(QString(SRCDIR)+"/data"),
     m_upgradeEnv("127.0.0.1", "root", "cntysoft", "upgrade")
{
   
}
void TestUpgradeEnv::initTestCase()
{
   QObject::connect(&m_upgradeEnv, &UpgradeEnv::logMsgSignal, this, &TestUpgradeEnv::writeLogMsg);
   QObject::connect(&m_upgradeEnv, &UpgradeEnv::excpetionSignal, this, &TestUpgradeEnv::exceptionHandler);
   m_upgradeEnv.setMetaInfo("ConfigBaseDir", "/srv/www/cloudcontrollerdevel/Config");
}

void TestUpgradeEnv::testUpgradeEnv()
{
   m_upgradeEnv.exec(m_dataDir+"/"+"upgrade_script.js");
}

void TestUpgradeEnv::writeLogMsg(const QString &msg)
{
   qDebug() << msg;
}

void TestUpgradeEnv::exceptionHandler(ErrorInfo error)
{
   qDebug() << error.toString();
}

}//upgrade
}//corelibtest
QTEST_MAIN(corelibtest::upgrade::TestUpgradeEnv)