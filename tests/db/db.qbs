import qbs 1.0
CoreLibAutoTest
{
   testName: "db"
   Group {
      name: "adapter"
      prefix: name+"/"
      files: [
         "test_adapter.cpp",
         "test_adapter.h",
      ]
   }
}