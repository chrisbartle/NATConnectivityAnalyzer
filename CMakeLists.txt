cmake_minimum_required(VERSION 3.16)

project(NATConnectivityAnalyzer VERSION 1.0.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

qt_standard_project_setup(REQUIRES 6.8)

# Include libpcpnatpmp for NAT hole punching using PMP and PCPs
add_subdirectory (libpcpnatpmp)

# Include miniupnpc for port routering using UPnP
add_subdirectory (miniupnpc)

qt_add_executable(natconnectivityanalyzer
    main.cpp
)

# We want to make the build timestamp available
string(TIMESTAMP BUILD_TIMESTAMP "%m/%d/%Y %H:%M:%S")

# Generate a header file with the version information filled in by cmake
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake_vals.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/cmake_vals.h
    @ONLY
)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/generated)

# Generate an rc file with the version information filled in by cmake
set(APP_ICON "${CMAKE_CURRENT_SOURCE_DIR}/windows/NATCA.ico")
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/windows/NATCA.rc.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/NATCA.rc
    @ONLY
)

qt_add_qml_module(natconnectivityanalyzer
    URI NATConnectivityAnalyzer
    QML_FILES
        Main.qml
        QML_FILES MainPage.qml
        QML_FILES Credits.qml
        SOURCES MainThreadController.h MainThreadController.cpp
        SOURCES WorkerThreadController.h WorkerThreadController.cpp
        SOURCES Stunner/resource.h Stunner/stdafx.cpp Stunner/stdafx.h Stunner/StunAddressAttribute.cpp Stunner/StunAddressAttribute.h Stunner/StunAttribute.cpp Stunner/StunAttribute.h Stunner/StunBindingErrorResponseMessage.cpp Stunner/StunBindingErrorResponseMessage.h Stunner/StunBindingMessage.cpp Stunner/StunBindingMessage.h Stunner/StunBindingRequestMessage.cpp Stunner/StunBindingRequestMessage.h Stunner/StunBindingResponseMessage.cpp Stunner/StunBindingResponseMessage.h Stunner/StunChangeRequestAttribute.cpp Stunner/StunChangeRequestAttribute.h Stunner/StunClientHelper.cpp Stunner/StunClientHelper.h Stunner/StunClientTransaction.cpp Stunner/StunClientTransaction.h Stunner/StunErrorCodeAttribute.cpp Stunner/StunErrorCodeAttribute.h Stunner/StunErrorResponseMessage.cpp Stunner/StunErrorResponseMessage.h Stunner/StunGlobals.cpp Stunner/StunGlobals.h Stunner/StunHeader.cpp Stunner/StunHeader.h Stunner/StunMessage.cpp Stunner/StunMessage.h Stunner/StunPasswordAttribute.cpp Stunner/StunPasswordAttribute.h Stunner/StunSharedSecretErrorResponseMessage.cpp Stunner/StunSharedSecretErrorResponseMessage.h Stunner/StunSharedSecretRequestMessage.cpp Stunner/StunSharedSecretRequestMessage.h Stunner/StunSharedSecretResponseMessage.cpp Stunner/StunSharedSecretResponseMessage.h Stunner/StunTransaction.cpp Stunner/StunTransaction.h Stunner/StunUnknownAttributes.cpp Stunner/StunUnknownAttributes.h Stunner/StunUsernameAttribute.cpp Stunner/StunUsernameAttribute.h Stunner/StunOS.h
        QML_FILES NatTypeExplanation.qml
        QML_FILES PortForwardExplanation.qml
        SOURCES ${CMAKE_CURRENT_BINARY_DIR}/generated/NATCA.rc
RESOURCES windows/NATCA.ico
RESOURCES cmake_vals.h.in
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(natconnectivityanalyzer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appNATConnectivityAnalyzer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(natconnectivityanalyzer PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    pcpnatpmp
    libminiupnpc-static
)

if(WIN32)
    target_sources(natconnectivityanalyzer PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated/NATCA.rc)
endif()

# pcpnatpmp and libminiupnpc need to be compiled with the same libraries as the main application. One was using -MD, the other -MT
if(MSVC)
    set_target_properties(pcpnatpmp PROPERTIES
        MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}"
    )
    set_target_properties(libminiupnpc-static PROPERTIES
        MSVC_RUNTIME_LIBRARY "${CMAKE_MSVC_RUNTIME_LIBRARY}"
    )
endif()

include(GNUInstallDirs)
install(TARGETS natconnectivityanalyzer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

