cmake_minimum_required(VERSION 3.16)

project(CreditCalc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

set(SRC_LIST
    Calculator/Credit.h
    Calculator/Credit.cpp
    Calculator/CreditCalculator.h
    Calculator/CreditCalculator.cpp
    Calculator/Fwd.h
)

qt_add_executable(appCreditCalc
    main.cpp
)

qt_add_qml_module(appCreditCalc
    URI CreditCalc
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES ${SRC_LIST}
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appCreditCalc PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appCreditCalc
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appCreditCalc
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appCreditCalc
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
