#ifndef DB_ADAPTER_TEST_SQL_H
#define DB_ADAPTER_TEST_SQL_H

#include <QObject>
#include "db/engine/engine.h"

namespace corelibtest{
namespace db{

using sn::corelib::db::engine::Engine;

class TestSql : public QObject
{
   Q_OBJECT
public:
   TestSql();
private slots:
   void initTestCase();
   void testSqlTableName();
   void testDeleteSql();
   void testWherePredicate();
   void testSelectSql();
protected:
   Engine m_engine;
};

}//db
}//corelibtest

#endif // DB_ADAPTER_TEST_SQL_H
