import qbs
QtApplication {
   type: ["application", "autotest"]
   consoleApplication: true
   property string testName
   name: "corelibtest_" + testName
   Depends { name: "Qt.test" }
   Depends {
      name: "corelib"
   }
   cpp.includePaths: "../"
   cpp.cxxLanguageVersion: "c++14"
   destinationDirectory: "bin"
}
