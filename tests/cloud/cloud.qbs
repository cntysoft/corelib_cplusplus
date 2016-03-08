import qbs 1.0
CoreLibAutoTest
{
   testName: "cloud"
   cpp.defines: base.concat([
                               'SRCDIR="' + path + '"'
                            ])
   files : [
      "test_cloud.h",
      "test_cloud.cpp"
   ]
}