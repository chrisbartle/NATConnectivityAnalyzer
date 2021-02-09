QT += quick

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        NATConnectivityAnalyzerLogic.cpp \
        NATConnectivityAnalyzerLogicRelay.cpp \
        Stunner/StunAddressAttribute.cpp \
        Stunner/StunAttribute.cpp \
        Stunner/StunBindingErrorResponseMessage.cpp \
        Stunner/StunBindingMessage.cpp \
        Stunner/StunBindingRequestMessage.cpp \
        Stunner/StunBindingResponseMessage.cpp \
        Stunner/StunChangeRequestAttribute.cpp \
        Stunner/StunClientHelper.cpp \
        Stunner/StunClientTransaction.cpp \
        Stunner/StunErrorCodeAttribute.cpp \
        Stunner/StunErrorResponseMessage.cpp \
        Stunner/StunGlobals.cpp \
        Stunner/StunHeader.cpp \
        Stunner/StunMessage.cpp \
        Stunner/StunPasswordAttribute.cpp \
        Stunner/StunSharedSecretErrorResponseMessage.cpp \
        Stunner/StunSharedSecretRequestMessage.cpp \
        Stunner/StunSharedSecretResponseMessage.cpp \
        Stunner/StunTransaction.cpp \
        Stunner/StunUnknownAttributes.cpp \
        Stunner/StunUsernameAttribute.cpp \
        Stunner/stdafx.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    NATConnectivityAnalyzerLogic.h \
    NATConnectivityAnalyzerLogicRelay.h \
    Stunner/StunAddressAttribute.h \
    Stunner/StunAttribute.h \
    Stunner/StunBindingErrorResponseMessage.h \
    Stunner/StunBindingMessage.h \
    Stunner/StunBindingRequestMessage.h \
    Stunner/StunBindingResponseMessage.h \
    Stunner/StunChangeRequestAttribute.h \
    Stunner/StunClientHelper.h \
    Stunner/StunClientTransaction.h \
    Stunner/StunErrorCodeAttribute.h \
    Stunner/StunErrorResponseMessage.h \
    Stunner/StunGlobals.h \
    Stunner/StunHeader.h \
    Stunner/StunMessage.h \
    Stunner/StunPasswordAttribute.h \
    Stunner/StunSharedSecretErrorResponseMessage.h \
    Stunner/StunSharedSecretRequestMessage.h \
    Stunner/StunSharedSecretResponseMessage.h \
    Stunner/StunTransaction.h \
    Stunner/StunUnknownAttributes.h \
    Stunner/StunUsernameAttribute.h \
    Stunner/resource.h \
    Stunner/stdafx.h

win32: LIBS += -lWS2_32
