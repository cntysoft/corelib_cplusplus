#include "abstract_source.h"
#include "db/metadata/object/view_object.h"

namespace sn{
namespace corelib{
namespace db{
namespace metadata{
namespace source{

using sn::corelib::db::metadata::object::ViewObject;
using sn::corelib::db::metadata::object::TableObject;

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
   return table;
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

AbstractSource::~AbstractSource()
{}

}//source
}//metadata
}//db
}//corelib
}//sn