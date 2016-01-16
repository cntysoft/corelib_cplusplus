import qbs 1.0
QtApplication {
    type: ["application", "autotest"]
    consoleApplication: true
    name:"defaulttest"
    Depends { name: "Qt.test" }
    cpp.cxxLanguageVersion: "c++14"
    destinationDirectory: "bin"
}
