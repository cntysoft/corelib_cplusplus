#include "test_sql.h"
#include <QtTest/QtTest>
#include "db/sql/sql.h"
#include "db/sql/delete.h"
#include "db/sql/table_identifier.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/predicate/predicate.h"
#include "db/sql/predicate/simple_predicate.h"
#include "db/sql/simple_sql.h"
#include "db/sql/expression.h"
#include "kernel/errorinfo.h"
#include "global/common_funcs.h"
#include <QSharedPointer>
#include <QDebug>
#include "global/common_funcs.h"
#include <QMap>
#include <QString>
#include <QList>

//struct MyValue
//{
//   QString name;
//};



//Q_DECLARE_METATYPE(QSharedPointer<QString>)
//Q_DECLARE_METATYPE(MyValue)
//Q_DECLARE_METATYPE(QSharedPointer<ns::MyClass>)
namespace corelibtest{
namespace db{

using sn::corelib::db::sql::Sql;
using sn::corelib::db::sql::Delete;
using sn::corelib::db::sql::Select;
using sn::corelib::db::sql::TableIdentifier;
using sn::corelib::db::sql::AbstractSql;
using sn::corelib::db::sql::AbstractPreparableSql;
using sn::corelib::db::sql::predicate::PredicateSet;
using sn::corelib::instanceof;
using sn::corelib::db::sql::Where;
using sn::corelib::db::sql::AbstractExpression;
using sn::corelib::db::sql::Expression;
using sn::corelib::ErrorInfo;



TestSql::TestSql()
   : m_engine(Engine::QMYSQL, {
{"host", "127.0.0.1"},
{"username","root"},
{"password", "cntysoft"},
{"dbname", "meta_info_devel"}
})
{
   
}

void TestSql::initTestCase()
{
   qRegisterMetaType<QSharedPointer<QString>>("StrPtr1");
   qRegisterMetaType<QSharedPointer<int>>("StrPtr");
}

void TestSql::testSqlTableName()
{
   Sql sql(m_engine, "userinfo");
   QCOMPARE(sql.getTableName(), QString("userinfo"));
}

void TestSql::testDeleteSql()
{
   Sql sql(m_engine, "userinfo");
   {
      QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
      TableIdentifier& table = deleteSql->getTable();
      QCOMPARE(table.getTable(), QString("userinfo"));
      deleteSql->where("`name` = `xiuxiu`");
      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu`"));
      deleteSql->where("`age` = 123", PredicateSet::OP_AND);
      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu` AND `age` = 123"));
   }
   {
      QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
      deleteSql->where("`name` = `xiuxiu`");
      deleteSql->where("`age` = 123", PredicateSet::OP_OR);
      QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = `xiuxiu` OR `age` = 123"));
   }
}

void TestSql::testWherePredicate()
{
   try{
      Sql sql(m_engine, "userinfo");
            {
               QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
               QSharedPointer<Where> where(new Where);
               deleteSql->where(where);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo`"));
               where->equalTo("name", "xiuxiu", AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
               where->equalTo("age", 12, AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
               where->setOrCombination();
               where->equalTo("depth", 1, AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = 'xiuxiu' AND `age` = 12 OR `depth` = 1"));
               where->clearPredicates();
               where->notEqualTo("name", "xiuxiu", AbstractExpression::TYPE_IDENTIFIER, AbstractExpression::TYPE_VALUE);
               //qDebug() << sql.buildSqlString(deleteSql);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` != 'xiuxiu'"));
            }
            {
               QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
               QSharedPointer<Where> where(new Where);
               where->isNull("name");
               deleteSql->where(where);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` IS NULL"));
            }
            {
               QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
               QSharedPointer<Where> where(new Where);
               where->isNotNull("name");
               deleteSql->where(where);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` IS NOT NULL"));
            }
            {
               QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
               QSharedPointer<Where> where(new Where);
               where->like("name", "mike");
               where->like("address", "bier");
               deleteSql->where(where);
               //qDebug() << sql.buildSqlString(deleteSql);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` LIKE 'mike' AND `address` LIKE 'bier'"));
               where->clearPredicates();
               where->notLike("name", "mike");
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` NOT LIKE 'mike'"));
               //qDebug() << sql.buildSqlString(deleteSql);
            }
            {
               QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
               QSharedPointer<Where> where(new Where);
               where->between("total", 12, 21);
               deleteSql->where(where);
               //qDebug() << sql.buildSqlString(deleteSql);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `total` BETWEEN 12 AND 21"));
               where->clearPredicates();
               where->between("range", "a", "z");
               //qDebug() << sql.buildSqlString(deleteSql);
               QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `range` BETWEEN 'a' AND 'z'"));
            }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->expression("`name` = ? AND `age` = ?", {QVariant("xiuxiu"), QVariant(123)});
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = 'xiuxiu' AND `age` = 123"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->literal("`name` = 'xiuxiu'");
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `name` = 'xiuxiu'"));
      }
      {
         QSharedPointer<Delete> deleteSql = sql.getDeleteSql();
         QSharedPointer<Where> where(new Where);
         where->in("id", {1, 23, 221, "z"});
         deleteSql->where(where);
         //qDebug() << sql.buildSqlString(deleteSql);
         QCOMPARE(sql.buildSqlString(deleteSql), QString("DELETE FROM `userinfo` WHERE `id` IN (1, 23, 221, 'z')"));
      }
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}



void TestSql::testSelectSql()
{
   try{

      Sql sql(m_engine, "userinfo");
      QSharedPointer<Select> selectSql = sql.getSelectSql();
//      MyValue v;
//      ns::MyClass *m = new ns::MyClass;
//      sn::corelib::db::sql::Expression *expression = new Expression("asdasdasd");
//      expression->name = "cntysoft";
      
//      QVariant xx1 = QVariant::fromValue(QSharedPointer<Expression>(new Expression("asdasdasd")));

//      qDebug() << xx1.value<QSharedPointer<Expression>>().isNull();
//      m->name = "zhangxiuchun";
//      QVariant xxx = QVariant::fromValue(QSharedPointer<ns::MyClass>(m));
//      qDebug() << xxx.value<QSharedPointer<ns::MyClass>>().data()->name;
//      v.name = QString("name");
//      QVariant xxx = QVariant::fromValue(v);
//      qDebug() << xxx.value<MyValue>().name;
//      qDebug() << qMetaTypeId<QSharedPointer<Expression>>();
//      QSharedPointer<Expression> expression = QSharedPointer<Expression>(new Expression("asdasdasdasdas"));
//      qDebug() << expression.isNull();
//      QVariant x1 = QVariant::fromValue(expression);
//      qDebug() << x1.typeName();
//      qDebug() << x1.value<QSharedPointer<Expression>>().data()->m_expression;
//      qDebug() << expression.isNull();
//      QVariant x2 = QVariant::fromValue(QSharedPointer<QString>(new QString("xiuxiu")));
//      qDebug() << x2.value<QSharedPointer<QString>>().isNull();
//      qDebug() << *x2.value<QSharedPointer<QString>>().data();
//      qDebug() << xxx.typeToName(1024);
//      qDebug() << xxx.canConvert(1024);
//      QSharedPointer<Expression> s = xxx.value<QSharedPointer<Expression>>();
//      qDebug() << s.isNull();
      qDebug() << sql.buildSqlString(selectSql);
      //qDebug() << QString("%1_%2 %3%4%5%6%7%8%9%10%11%12").arg("asdasd", "asdas");
//      qDebug() << sn::corelib::format_str("%1 sdas %2_%3 sdas %4_%5 sdas %6_%7 sdas %8_%9 sdas %10_%11 sdas %12_%13 sdas %14_%15 sdas %16", 
//      {"1", "2","3","4", "5","6","7", "8","9","10", "11","12","13", "14","15","16","17"});
   }catch(ErrorInfo exp){
      qDebug() << exp.toString();
   }
}

}//db
}//corelibtest
QTEST_MAIN(corelibtest::db::TestSql)