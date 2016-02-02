#include <QDateTime>

#include "abstract_source.h"
#include "db/metadata/object/view_object.h"
#include "db/metadata/object/constraintkey_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

using sn::corelib::db::metadata::object::ViewObject;
using sn::corelib::db::metadata::object::TableObject;
using sn::corelib::db::metadata::object::ConstraintKeyObject;

const QString AbstractSource::__DEFAULT_SCHEMA__ = "__DEFAULT_SCHEMA__";

AbstractSource::AbstractSource(QSharedPointer<engine::Engine> engine)
   : m_engine(engine)
{
   QString currentSchema(m_engine->getCurrentSchema());
   if(!currentSchema.isEmpty()){
      m_defaultSchema = currentSchema;
   }else{
      m_defaultSchema = AbstractSource::__DEFAULT_SCHEMA__;
   }
}

const QStringList& AbstractSource::getSchemas()
{
   loadSchemaData();
   return m_schemasData;
}

void AbstractSource::getTableNames(QStringList &tableNames, QString schema, bool includeViews)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadTableNameData(schema);
   if(includeViews){
      tableNames = m_schemasTablesData[schema].keys();
      return;
   }
   QMap<QString, QMap<QString, QString>> &schemaTablesData = m_schemasTablesData[schema];
   QMap<QString, QMap<QString, QString>>::const_iterator it = schemaTablesData.cbegin();
   QMap<QString, QMap<QString, QString>>::const_iterator endMarker = schemaTablesData.cend();
   while(it != endMarker){
      if("BASE TABLE" == it.value().value("table_type")){
         tableNames.append(it.key());
      }
      it++;
   }
}

void AbstractSource::getTables(QList<QSharedPointer<AbstractTableObject>> &tables, QString schema, 
                               bool includeViews)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   QStringList tableNames;
   getTableNames(tableNames, schema, includeViews);
   int total = tableNames.size();
   for(int i = 0; i < total; i++){
      tables.append(getTable(tableNames[i], schema));
   }
   
}

QSharedPointer<AbstractTableObject> AbstractSource::getTable(const QString &tableName, QString schema)throw(ErrorInfo)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadTableNameData(schema);
   if(!m_schemasTablesData.contains(schema) || !m_schemasTablesData[schema].contains(tableName)){
      return QSharedPointer<AbstractTableObject>();
   }
   QMap<QString, QString> &data = m_schemasTablesData[schema][tableName];
   QSharedPointer<AbstractTableObject> table;
   QString tableType(data["table_type"]);
   if(tableType == "BASE TABLE"){
      table.reset(new TableObject(tableName));
   }else if(tableType == "VIEW"){
      table.reset(new ViewObject(tableName));
      ViewObject* viewPtr = dynamic_cast<ViewObject*>(table.data());
      viewPtr->setViewDefinition(data["view_definition"]);
      viewPtr->setCheckOption(data["check_option"]);
      viewPtr->setIsUpdatable(data["is_updatable"] == "YES" ? true : false);
   }else{
      throw ErrorInfo(QString("Table '%1' is of an unsupported type '%2'").arg(tableName).arg(data["table_type"]));
   }
   QList<QSharedPointer<ColumnObject>> columns;
   getColumns(columns, tableName, schema);
   table->setColumns(columns);
   return table;
}

void AbstractSource::getViewNames(QStringList &viewNames, QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadTableNameData(schema);
   if(!m_schemasTablesData.contains(schema)){
      viewNames.clear();
   }
   QMap<QString, QMap<QString, QString>> &tablesMap = m_schemasTablesData[schema];
   QMap<QString, QMap<QString, QString>>::const_iterator it = tablesMap.cbegin();
   QMap<QString, QMap<QString, QString>>::const_iterator endMarker = tablesMap.cend();
   while(it != endMarker){
      if("VIEW" == it.value().value("table_type")){
         viewNames.append(it.key());
      }
      it++;
   }
}

void AbstractSource::getViews(QList<QSharedPointer<ViewObject>> &views, QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   QStringList viewNames;
   getViewNames(viewNames, schema);
   int total = viewNames.size();
   for(int i = 0; i < total; i++){
      views.append(getTable(viewNames[i], schema).dynamicCast<ViewObject>());
   }
}

QSharedPointer<ViewObject> AbstractSource::getView(const QString &viewName, QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadTableNameData(schema);
   QMap<QString, QMap<QString, QString>> &tablesMap = m_schemasTablesData[schema];
   if(tablesMap.contains(viewName) && tablesMap[viewName].value("table_type") == "VIEW"){
      return getTable(viewName, schema).dynamicCast<ViewObject>();
   }
   return QSharedPointer<ViewObject>();
}

QSharedPointer<ColumnObject> AbstractSource::getColumn(const QString &columnName, const QString &tableName, 
                                                       QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadColumnData(tableName, schema);
   if(!m_tableColumnsData[schema][tableName].contains(columnName)){
      return QSharedPointer<ColumnObject>();
   }
   QMap<QString, QVariant> &info = m_tableColumnsData[schema][tableName][columnName];
   QSharedPointer<ColumnObject> column(new ColumnObject(columnName, tableName, schema));
   column->setOrdinalPosition(info["ordinal_position"].toInt());
   column->setColumnDefault(info["column_default"].toString());
   column->setIsNullable(info["is_nullable"].toBool());
   column->setDataType(info["data_type"].toString());
   column->setCharacterMaximumLength(info["character_maximum_length"].toInt());
   column->setCharacterOctetLength(info["character_octet_length"].toInt());
   column->setNumericPrecision(info["numeric_precision"].toInt());
   column->setNumericScale(info["numeric_scale"].toInt());
   column->setNumericUnsigned(info["numeric_unsigned"].toBool());
   column->setErratas(info["erratas"].toMap());
   return column;
}

void AbstractSource::getColumns(QList<QSharedPointer<ColumnObject>> &columns, const QString &tableName, 
                                QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadColumnData(tableName, schema);
   QStringList columnNames;
   getColumnNames(columnNames, tableName, schema);
   int total = columnNames.size();
   for(int i = 0; i < total; i++){
      columns.append(getColumn(columnNames[i], tableName, schema));
   }
}

void AbstractSource::getColumnNames(QStringList &columnNames, const QString &tableName, QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadColumnData(tableName, schema);
   if(!m_tableColumnsData.contains(schema) || !m_tableColumnsData[schema].contains(tableName)){
      columnNames.clear();
   }
   columnNames = m_tableColumnsData[schema][tableName].keys();
}

QSharedPointer<ConstraintObject> AbstractSource::getConstraint(const QString &constraintName, const QString &table, 
                                                               QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadConstraintData(table, schema);
   if(!m_tableConstraintData.contains(schema) ||
         !m_tableConstraintData[schema].contains(table) ||
         !m_tableConstraintData[schema][table].contains(constraintName)){
      return QSharedPointer<ConstraintObject>();
   }
   QMap<QString, QVariant> &info = m_tableConstraintData[schema][table][constraintName];
   QSharedPointer<ConstraintObject> constraint(new ConstraintObject(constraintName, table, schema));
   constraint->setType(info["constraint_type"].toString());
   constraint->setMatchOption(info["match_option"].toString());
   constraint->setUpdateRule(info["update_rule"].toString());
   constraint->setDeleteRule(info["delete_rule"].toString());
   constraint->setColumns(info["columns"].toStringList());
   constraint->setReferencedTableSchema(info["referenced_table_schema"].toString());
   constraint->setReferencedTableName(info["referenced_table_name"].toString());
   constraint->setReferencedColumns(info["referenced_columns"].toStringList());
   constraint->setCheckClause(info["check_clause"].toString());
   return constraint;
}

void AbstractSource::getConstraints(QList<QSharedPointer<ConstraintObject>> &constraints, const QString &table, 
                                    QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadConstraintData(table, schema);
   if(!m_tableConstraintData.contains(schema) || !m_tableConstraintData[schema].contains(table)){
      constraints.clear();
      return;
   }
   QStringList names(m_tableConstraintData[schema][table].keys());
   int nameCount = names.size();
   for(int i = 0; i < nameCount; i++){
      constraints.append(getConstraint(names[i], table, schema));
   }
}

void AbstractSource::getConstraintKeys(QList<QSharedPointer<ConstraintKeyObject>> &keys, const QString &constraint, const QString &table, 
                                       QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadConstraintReferences(table, schema);
   QMap<QString, QMap<QString, QString>> references;
   if(m_constraintReferencesData.contains(schema)){
      QList<QMap<QString, QString>> &items = m_constraintReferencesData[schema];
      QList<QMap<QString, QString>>::const_iterator it = items.cbegin();
      QList<QMap<QString, QString>>::const_iterator endMarker = items.cend();
      while(it != endMarker){
         QMap<QString, QString> refKeyInfo(*it);
         qDebug() << refKeyInfo["constraint_name"];
         if(constraint.endsWith(refKeyInfo["constraint_name"])){
            references[constraint] = refKeyInfo;
         }
         it++;
      }
   }
   
   loadConstraintDataKeys(schema);
   if(!m_constraintKeysData.contains(schema)){
      keys.clear();
      return;
   }
   QList<QMap<QString, QString>> &keysItems = m_constraintKeysData[schema];
   QList<QMap<QString, QString>>::const_iterator it = keysItems.cbegin();
   QList<QMap<QString, QString>>::const_iterator endMarker = keysItems.cend();
   while(it != endMarker){
      QMap<QString, QString> constraintKeyInfo = *it;
      if(constraintKeyInfo["table_name"] == table && constraint.endsWith(constraintKeyInfo["constraint_name"])){
         QSharedPointer<ConstraintKeyObject> key(new ConstraintKeyObject(constraintKeyInfo["column_name"]));
         key->setOrdinalPosition(constraintKeyInfo["ordinal_position"].toInt());
         if(references.contains(constraint)){
            key->setForeignKeyUpdateRule(references[constraint]["update_rule"]);
            key->setForeignKeyDeleteRule(references[constraint]["delete_rule"]);
            key->setReferencedTableName(references[constraint]["referenced_table_name"]);
            key->setReferencedColumnName(references[constraint]["referenced_column_name"]);
            keys.append(key);
         }
      }
      it++;
   }
}

QSharedPointer<TriggerObject> AbstractSource::getTrigger(const QString &triggerName, QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadTriggerData(schema);
   if(!m_triggersData.contains(schema) || !m_triggersData[schema].contains(triggerName)){
      return QSharedPointer<TriggerObject>();
   }
   QMap<QString, QString> info = m_triggersData[schema][triggerName];
   QSharedPointer<TriggerObject> trigger(new TriggerObject());
   trigger->setName(triggerName);
   trigger->setEventManipulation(info["event_manipulation"]);
   trigger->setEventObjectCatalog(info["event_object_catalog"]);
   trigger->setEventObjectSchema(info["event_object_schema"]);
   trigger->setEventObjectTable(info["event_object_table"]);
   trigger->setActionOrder(info["action_order"]);
   trigger->setActionCondition(info["action_condition"]);
   trigger->setActionStatement(info["action_statement"]);
   trigger->setActionOrientation(info["action_orientation"]);
   trigger->setActionTiming(info["action_timing"]);
   trigger->setActionReferenceOldTable(info["action_reference_old_table"]);
   trigger->setActionReferenceNewTable(info["action_reference_new_table"]);
   trigger->setActionReferenceOldRow(info["action_reference_old_row"]);
   trigger->setActionReferenceNewRow(info["action_reference_new_row"]);
   trigger->setCreated(QDateTime::fromString(info["created"]));
   return trigger;
}

void AbstractSource::getTriggers(QList<QSharedPointer<TriggerObject>> &triggers, QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   QStringList triggerNames;
   getTriggerNames(triggerNames, schema);
   int nameCount = triggerNames.size();
   for(int i = 0; i < nameCount; i++){
      triggers.append(getTrigger(triggerNames[i], schema));
   }
}

void AbstractSource::getTriggerNames(QStringList &names, QString schema)
{
   if(schema.isEmpty()){
      schema = m_defaultSchema;
   }
   loadTriggerData(schema);
   if(!m_triggersData.contains(schema)){
      names.clear();
   }
   names = m_triggersData[schema].keys();
}

AbstractSource::~AbstractSource()
{}

}//source
}//metadata
}//db
}//corelib
}//sn