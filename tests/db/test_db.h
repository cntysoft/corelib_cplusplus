#ifndef DB_ADAPTER_TEST_SQL_H
#define DB_ADAPTER_TEST_SQL_H
#include <QSharedPointer>
#include <QObject>
#include "db/engine/engine.h"
#include "db/metadata/metadata.h"

namespace corelibtest{
namespace db{

using sn::corelib::db::engine::Engine;
using sn::corelib::db::metadata::Metadata;

class TestDb : public QObject
{
   Q_OBJECT
public:
   TestDb();
private slots:
   void initTestCase();
   void testSqlTableName();
   void testDeleteSql();
   void testWherePredicate();
   void testSelectSql();
   void testExpression();
   void testSubSelect();
   void testInsertSql();
   void testUpdateSql();
   void testCreateTable();
   void testDropTable();
   void testAlterTable();
   void testSelectDecorator();
   void testCreateTableDecorator();
   //metadata
   void testMetadata();
protected:
   QSharedPointer<Engine> m_engine;
   QSharedPointer<Metadata> m_metadata;
};

}//db
}//corelibtest

#endif // DB_ADAPTER_TEST_SQL_H
