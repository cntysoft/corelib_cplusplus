import qbs
Project
{
   name: "corelibtestproject"
   minimumQbsVersion: "1.4.2"
   qbsSearchPaths:["tests/qbs-resources"]
   references: [
      "tests/db/db.qbs"
   ]
}
