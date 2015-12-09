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
   cpp.defines: base.concat(type == "staticlibrary" ? ["SE_STATIC_LIB"] : ["SE_LIBRARY"])
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
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
}