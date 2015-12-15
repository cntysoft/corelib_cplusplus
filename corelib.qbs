import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "corelib"
   targetName : "sncore"
   version : "0.1.1"
   Depends { 
      name: "Qt"; 
      submodules: ["core"]
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
      Depends { name: "Qt"; submodules: ["core"] }
      cpp.rpaths: ["$ORIGIN/../lib"]
      cpp.includePaths: [product.sourceDirectory]
   }
   Group {
      name: "kernel"
      prefix: name+"/"
      files :[
         "application.cpp",
         "application.h",
         "erroritem.cpp",
         "erroritem.h",
         "errorinfo.h",
         "errorinfo.cpp",
         "code_location.h",
         "code_location.cpp",
         "settings.cpp",
         "settings.h",
         "stddir.cpp",
         "stddir.h",
      ]
   }
   Group {
      name : "global"
      prefix : name + '/'
      files : [
           "common_funcs.cpp",
           "common_funcs.h",
           "global.h",
           "global.cpp",
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
}