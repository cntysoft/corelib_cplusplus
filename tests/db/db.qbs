import qbs 1.0
CoreLibAutoTest
{
    files: [
    ]
   testName: "db"
   Group {
      name: "sql"
      prefix: name+"/"
      files: [
           "test_sql.h",
           "test_sql.cpp",
       ]
   }
}