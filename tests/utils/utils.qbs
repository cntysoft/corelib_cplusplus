import qbs 1.0
CoreLibAutoTest
{
   cpp.defines: base.concat([
                               'SRCDIR="' + path + '"'
                            ])
   testName: "utils"
   files : [
        "test_version.cpp",
        "test_version.h",
    ]
}