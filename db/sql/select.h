#ifndef SN_CORELIB_DB_SQL_SELECT_H
#define SN_CORELIB_DB_SQL_SELECT_H

#include <QString>
#include <QVariant>
#include <QSharedPointer>
#include <QPair>
#include <QMap>

#include "global/global.h"
#include "kernel/errorinfo.h"

#include "db/sql/abstract_preparable_sql.h"
#include "db/sql/abstract_expression.h"
#include "db/sql/simple_sql.h"
#include "db/sql/abstract_sql.h"
#include "db/sql/predicate/predicateset.h"
#include "table_identifier.h"

namespace sn{
namespace corelib{
namespace db{
namespace sql{

using sn::corelib::db::sql::AbstractSql;
using sn::corelib::db::sql::predicate::PredicateSet;
using sn::corelib::ErrorInfo;

class SN_CORELIB_EXPORT Select : public AbstractPreparableSql
{
public:
   const static QString SELECT;
   const static QString QUANTIFIER;
   const static QString COLUMNS;
   const static QString TABLE;
   const static QString JOINS;
   const static QString WHERE;
   const static QString GROUP;
   const static QString HAVING;
   const static QString ORDER;
   const static QString LIMIT;
   const static QString OFFSET;
   const static QString QUANTIFIER_DISTINCT;
   const static QString QUANTIFIER_ALL;
   const static QString JOIN_INNER;
   const static QString JOIN_OUTER;
   const static QString JOIN_LEFT;
   const static QString JOIN_RIGHT;
   const static QString JOIN_OUTER_RIGHT;
   const static QString JOIN_OUTER_LEFT;
   const static QString SQL_STAR;
   const static QString ORDER_ASCENDING;
   const static QString ORDER_DESCENDING;
   const static QString COMBINE;
   const static QString COMBINE_UNION;
   const static QString COMBINE_EXCEPT;
   const static QString COMBINE_INTERSECT;
   
   struct RawState
   {
      TableIdentifier table;
      bool tableReadOnly = false;
      bool prefixColumnsWithTable = true;
      QVariant quantifier;
      QList<QVariant> columns;
      QSharedPointer<Where> where;
      QSharedPointer<Having> having;
      QStringList group;
      QMap<QString, QString> order;
      QVariant limit;
      QVariant offset;
      QMap<QString, QVariant> combine;
      QList<QMap<QString, QVariant>> joins;
   };

public:
   friend ProcessResultPointerType select_process_statement_start(AbstractSql *self,const Engine &engine, 
                                                                  ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                                  QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_select(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                         QMap<QString, ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_join(AbstractSql *self,const Engine &engine, 
                                                       ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                       QMap<QString, ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_where(AbstractSql *self,const Engine &engine, 
                                                        ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                        QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_having(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_group(AbstractSql *self,const Engine &engine, 
                                                        ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                        QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_order(AbstractSql *self,const Engine &engine, 
                                                        ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                        QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_limit(AbstractSql *self,const Engine &engine, 
                                                        ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                        QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_offset(AbstractSql *self,const Engine &engine, 
                                                         ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                         QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_statement_end(AbstractSql *self,const Engine &engine, 
                                                                ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                                QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
   friend ProcessResultPointerType select_process_combine(AbstractSql *self,const Engine &engine, 
                                                          ParameterContainer *parameterContainer, QMap<QString, QString> &sqls, 
                                                          QMap<QString, AbstractSql::ProcessResultPointerType> &parameters);
public:
   Select(const TableIdentifier &table = TableIdentifier());
   Select(const QString &table, const QString &schema = QString());
   Select& from(const QString &tableName, const QString &schema = QString())throw(ErrorInfo);
   Select& from(const TableIdentifier &table)throw(ErrorInfo);
   Select& setQuantifier(const QString &quantifier);
   /*
     Specify columns from which to select
     Possible valid states:
     array()
     array(value, ...)
     value can be strings or Expression objects
     array(string => value, ...)
     key string will be use as alias,
     value can be string or Expression objects
   */
   Select& setColumns(QList<QVariant> &columns, bool prefixColumnsWithTable = true);
   Select& addColumn(const QSharedPointer<AbstractExpression> &column, int index);
   Select& addColumn(const QVariant &column, int index);
   Select& addColumn(const QSharedPointer<AbstractExpression> &column, const QString &alias);
   Select& addColumn(const QVariant &column, const QString &alias);
   Select& setPrefixColumnsWithTable(bool flag);
   Select& where(const QSharedPointer<Where> &where);
   Select& where(const QString &where, const QString &combination = PredicateSet::OP_AND);
   Select& having(const QSharedPointer<Having> &having);
   Select& having(const QString &having, const QString &combination = PredicateSet::OP_AND);
   Select& group(const QString &group);
   Select& group(const QStringList &groups);
   Select& order(const QString &name, const QString &orderType = Select::ORDER_ASCENDING);
   Select& order(const QMap<QString, QString> &orders);
   Select& limit(quint32 limit);
   Select& join(const QString &name, const QString &on, const QMap<QVariant, QVariant> &columns = QMap<QVariant, QVariant>{{QVariant(0), QVariant(Select::SQL_STAR)}}, 
                const QString &type = Select::JOIN_INNER);
   Select& join(const QVariant &name, const QString &on, const QMap<QVariant, QVariant> &columns = QMap<QVariant, QVariant>{{QVariant(0), QVariant(Select::SQL_STAR)}}, 
                const QString &type = Select::JOIN_INNER);
   Select& join(const QVariant &name, const QString &alias, const QString &on, const QMap<QVariant, QVariant> &columns = QMap<QVariant, QVariant>{{QVariant(0), QVariant(Select::SQL_STAR)}}, 
                const QString &type = Select::JOIN_INNER);
   Select& join(const QSharedPointer<Select> &name, const QString &alias, const QString &on, const QMap<QVariant, QVariant> &columns = QMap<QVariant, QVariant>{{QVariant(0), QVariant(Select::SQL_STAR)}}, 
                const QString &type = Select::JOIN_INNER);
   Select& offset(quint32 offset);
   Select& combine(const QSharedPointer<Select> select, const QString &type = Select::COMBINE_UNION, 
                   const QString &modifier = QString())throw(ErrorInfo);
   virtual Select& setSubject(QSharedPointer<Select> subject);
   Select& setTableReadOnly(bool flag);
   const QSharedPointer<Where>& getWhere()const;
   const QSharedPointer<Having>& getHaving()const;
   Select& reset(const QString &part)throw(ErrorInfo);
   RawState getRawState()const;
   virtual QString getDecoratorClassName()const;
protected:
   QPair<QString, QString> resolveTable(const TableIdentifier &table, const Engine &engine, 
                                        ParameterContainer *parameterContainer = nullptr);
   QPair<QString, QString> resolveTable(const QString &table, const Engine &engine, 
                                        ParameterContainer *parameterContainer = nullptr);
   QPair<QString, QString> resolveTable(const QString &table, const QString &alias, 
                                        const Engine &engine, ParameterContainer *parameterContainer = nullptr);
   using AbstractSql::resolveTable;
   QString renderTable(const QString &table, const QString &alias = QString());
protected:
   virtual void localizeVariables();
protected:
   TableIdentifier m_table;
   bool m_tableReadOnly = false;
   bool m_prefixColumnsWithTable = true;
   QVariant m_quantifier;
   QList<QVariant> m_columns;
   QSharedPointer<Where> m_where;
   QSharedPointer<Having> m_having;
   QStringList m_group;
   QMap<QString, QString> m_order;
   QVariant m_limit;
   QVariant m_offset;
   QMap<QString, QVariant> m_combine;
   QList<QMap<QString, QVariant>> m_joins;
};

}//sql
}//db
}//corelib
}//sn
Q_DECLARE_METATYPE(QSharedPointer<sn::corelib::db::sql::Select>)
#endif // SN_CORELIB_DB_SQL_SELECT_H
