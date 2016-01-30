import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "corelib"
   targetName : "sncore"
   Depends { 
      name: "Qt"; 
      submodules: ["core","network","websockets","sql"]
   }
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: {
      var defines = [];
      if(product.type == "staticlibrary"){
         defines.push("SN_CORELIB_STATIC_LIB");
      }else{
         defines.push("SN_CORELIB_LIBRARY");
      }
      defines = defines.concat([
                                  'SN_INSTALL_ROOT="' + qbs.installRoot + '"',
                                  'SN_SHARE_RES_DIR="' + qbs.installRoot+'/'+project.resourcesInstallDir+ '"',
                                  'SN_CORELIB_VERSION="'+ version+'"'
                               ]);
      return defines;
   }
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   
   cpp.includePaths: base.concat(["."])
   Group {
      fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
      qbs.install: true
      qbs.installDir: "lib"
   }
   
   Export {
      Depends { name: "cpp" }
      Depends { name: "Qt"; submodules: ["core","network","websockets","sql"] }
      cpp.rpaths: ["$ORIGIN/../lib"]
      cpp.includePaths: [product.sourceDirectory]
   }
   
   Group {
      name: "kernel"
      prefix: name+"/"
      files :[
         "settings.cpp",
         "settings.h",
         "application.cpp",
         "application.h",
         "erroritem.cpp",
         "erroritem.h",
         "errorinfo.h",
         "errorinfo.cpp",
         "code_location.h",
         "code_location.cpp",
         "stddir.cpp",
         "stddir.h",
      ]
   }
   
   Group {
      name : "global"
      prefix : name + '/'
      files : [
         "global.h",
         "global.cpp",
         "common_funcs.cpp",
         "common_funcs.h",
         
      ]
   }
   
   Group {
      name : "io"
      prefix : name + '/'
      files : [
         "filesystem.cpp",
         "filesystem.h",
         "terminal.cpp",
         "terminal.h",
      ]
   }
   
   Group {
      name: "command"
      prefix: name+"/"
      files: [
         "abstract_command.cpp",
         "abstract_command.h",
         "abstract_command_runner.cpp",
         "abstract_command_runner.h",
         "command_meta.cpp",
         "command_meta.h",
         "history.cpp",
         "history.h",
         "route_item.cpp",
         "route_item.h",
         "route_match_result.cpp",
         "route_match_result.h",
         "route_matcher.cpp",
         "route_matcher.h",
         "route_stack.cpp",
         "route_stack.h",
      ]
   }
   
   Group {
      name: "ds"
      prefix: name+"/"
      files: [
         "priority_list.h",
      ]
   }
   
   Group {
      name: "network"
      prefix: name+"/"
      files:[
         "abstract_multi_thread_server.cpp",
         "abstract_multi_thread_server.h",
         "rpc/abstract_service.cpp",
         "rpc/abstract_service.h",
         "rpc/invoke_meta.cpp",
         "rpc/invoke_meta.h",
         "rpc/service_error_code.h",
         "rpc/service_invoker.cpp",
         "rpc/service_invoker.h",
         "rpc/service_provider.cpp",
         "rpc/service_provider.h",
         "rpc/tcpsocket_data_dispatch_worker.cpp",
         "rpc/tcpsocket_data_dispatch_worker.h",
      ]
   }
   
   Group {
      name: "db"
      prefix: name+"/"
      files: [
           "engine/connection.h",
           "engine/engine.cpp",
           "engine/engine.h",
           "engine/interface.h",
           "engine/parameter_conatiner.cpp",
           "engine/parameter_container.h",
           "engine/statement_container.cpp",
           "engine/statement_container.h",
           "metadata/metadata.h",
           "rowgateway/row_gateway.h",
           "sql/abstract_expression.cpp",
           "sql/abstract_expression.h",
           "sql/abstract_preparable_sql.cpp",
           "sql/abstract_preparable_sql.h",
           "sql/abstract_sql.cpp",
           "sql/abstract_sql.h",
           "sql/ddl/alter_table.cpp",
           "sql/ddl/alter_table.h",
           "sql/ddl/column/abstract_column.cpp",
           "sql/ddl/column/abstract_column.h",
           "sql/ddl/column/simple_column.cpp",
           "sql/ddl/column/simple_column.h",
           "sql/ddl/constraint/abstract_constraint.cpp",
           "sql/ddl/constraint/abstract_constraint.h",
           "sql/ddl/constraint/simple_constraint.cpp",
           "sql/ddl/constraint/simple_constraint.h",
           "sql/ddl/create_table.cpp",
           "sql/ddl/create_table.h",
           "sql/ddl/drop_table.cpp",
           "sql/ddl/drop_table.h",
           "sql/ddl/index/index.cpp",
           "sql/ddl/index/index.h",
           "sql/delete.cpp",
           "sql/delete.h",
           "sql/expression.cpp",
           "sql/expression.h",
           "sql/insert.cpp",
           "sql/insert.h",
           "sql/platform/abstract_platform.cpp",
           "sql/platform/abstract_platform.h",
           "sql/platform/mysql/create_table_decorator.cpp",
           "sql/platform/mysql/create_table_decorator.h",
           "sql/platform/mysql/select_decorator.cpp",
           "sql/platform/mysql/select_decorator.h",
           "sql/platform/platform.cpp",
           "sql/platform/platform.h",
           "sql/platform/platform_types.cpp",
           "sql/platform/platform_types.h",
           "sql/predicate/predicate.cpp",
           "sql/predicate/predicate.h",
           "sql/predicate/predicateset.cpp",
           "sql/predicate/predicateset.h",
           "sql/predicate/simple_predicate.cpp",
           "sql/predicate/simple_predicate.h",
           "sql/select.cpp",
           "sql/select.h",
           "sql/simple_sql.cpp",
           "sql/simple_sql.h",
           "sql/sql.cpp",
           "sql/sql.h",
           "sql/table_identifier.cpp",
           "sql/table_identifier.h",
           "sql/update.cpp",
           "sql/update.h",
           "tablegateway/table_gateway.h",
       ]
   }
}