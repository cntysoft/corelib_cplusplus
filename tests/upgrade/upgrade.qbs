import qbs 1.0
CoreLibAutoTest
{
   testName: "upgrade"
   files : [
      "test_upgradeenv.cpp",
      "test_upgradeenv.h",
   ]
   cpp.defines: base.concat([
                               'SRCDIR="' + path + '"'
                            ])
   Group {
      name: "data"
      files : [
           "data/upgrade_script.js",
       ]
   }
}