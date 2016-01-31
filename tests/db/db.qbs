import qbs 1.0
CoreLibAutoTest
{
   testName: "db"
   files : [
        "test_db.cpp",
        "test_db.h",
        "test_sql.cpp",
        "test_metadata.cpp",
    ]
}