import qbs 1.0
Product
{
    type: "dynamiclibrary"
    name : "corelib"
    targetName : "sncore"
    Depends { 
        name: "Qt"; 
        submodules: ["core"]
    }
    Depends { name:"cpp"}
    destinationDirectory: "lib"
    cpp.defines: {
        var defines = [];
        if(product.type == "staticlibrary"){
            defines.push("SE_STATIC_LIB");
        }else{
            defines.push("SN_LIBRARY");
        }
        defines = defines.concat([
                                     'SN_INSTALL_ROOT="' + qbs.installRoot + '"',
                                     'SN_SHARE_RES_DIR="' + qbs.installRoot+'/'+project.resourcesInstallDir+ '"'
                                 ]);
        return defines;
    }
    cpp.visibility: "minimal"
    cpp.cxxLanguageVersion: "c++14"
    version : "0.1.1"
    cpp.includePaths: base.concat(["."])
    Group {
        fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
        qbs.install: true
        qbs.installDir: "bin"
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
            "const.h",
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
            "command_meta.cpp",
            "command_meta.h",
        ]
    }
}