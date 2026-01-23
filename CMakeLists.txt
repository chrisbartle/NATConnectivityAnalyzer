cmake_minimum_required(VERSION 3.16)

project(NATConnectivityAnalyzer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(NATConnectivityAnalyzer
    main.cpp
)

qt_add_qml_module(NATConnectivityAnalyzer
    URI NATConnectivityAnalyzer
    QML_FILES
        Main.qml
SOURCES NATConnectivityAnalyzerLogic.cpp NATConnectivityAnalyzerLogic.h NATConnectivityAnalyzerLogicRelay.cpp NATConnectivityAnalyzerLogicRelay.h
QML_FILES Credits.qml Intro.qml Results.qml
SOURCES Stunner/resource.h Stunner/stdafx.cpp Stunner/stdafx.h Stunner/StunAddressAttribute.cpp Stunner/StunAddressAttribute.h Stunner/StunAttribute.cpp Stunner/StunAttribute.h Stunner/StunBindingErrorResponseMessage.cpp Stunner/StunBindingErrorResponseMessage.h Stunner/StunBindingMessage.cpp Stunner/StunBindingMessage.h Stunner/StunBindingRequestMessage.cpp Stunner/StunBindingRequestMessage.h Stunner/StunBindingResponseMessage.cpp Stunner/StunBindingResponseMessage.h Stunner/StunChangeRequestAttribute.cpp Stunner/StunChangeRequestAttribute.h Stunner/StunClientHelper.cpp Stunner/StunClientHelper.h Stunner/StunClientTransaction.cpp Stunner/StunClientTransaction.h Stunner/StunErrorCodeAttribute.cpp Stunner/StunErrorCodeAttribute.h Stunner/StunErrorResponseMessage.cpp Stunner/StunErrorResponseMessage.h Stunner/StunGlobals.cpp Stunner/StunGlobals.h Stunner/StunHeader.cpp Stunner/StunHeader.h Stunner/StunMessage.cpp Stunner/StunMessage.h Stunner/StunPasswordAttribute.cpp Stunner/StunPasswordAttribute.h Stunner/StunSharedSecretErrorResponseMessage.cpp Stunner/StunSharedSecretErrorResponseMessage.h Stunner/StunSharedSecretRequestMessage.cpp Stunner/StunSharedSecretRequestMessage.h Stunner/StunSharedSecretResponseMessage.cpp Stunner/StunSharedSecretResponseMessage.h Stunner/StunTransaction.cpp Stunner/StunTransaction.h Stunner/StunUnknownAttributes.cpp Stunner/StunUnknownAttributes.h Stunner/StunUsernameAttribute.cpp Stunner/StunUsernameAttribute.h
RESOURCES qml.qrc qtquickcontrols2.conf
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(NATConnectivityAnalyzer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appNATConnectivityAnalyzer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(NATConnectivityAnalyzer
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS NATConnectivityAnalyzer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
