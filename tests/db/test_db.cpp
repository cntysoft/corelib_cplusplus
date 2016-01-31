#include <QObject>
#include <QtTest/QtTest>

#include "test_db.h"
#include "db/engine/engine.h"

namespace corelibtest{
namespace db{

TestDb::TestDb()
   : m_engine(new Engine(Engine::QMYSQL, {
{"host", "127.0.0.1"},
{"username","root"},
{"password", "cntysoft"},
{"dbname", "meta_info_devel"}
}))
{
   
}

void TestDb::initTestCase()
{
   m_metadata.reset(new Metadata(m_engine));
}


}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestDb)