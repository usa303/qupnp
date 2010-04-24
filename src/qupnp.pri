include(../common.pri)
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD
qupnp-uselib:!qupnp-buildlib:LIBS += -L$$QUPNP_LIBDIR \
    -l$$QUPNP_LIBNAME
else { 
    SOURCES += $$PWD/qthttpserver.cpp \
        $$PWD/qtupnpdeviceinfo.cpp \
        $$PWD/qtupnpdeviceproxy.cpp \
        $$PWD/qtupnpdevice.cpp \
        $$PWD/qtupnprootdevice.cpp \
        $$PWD/qtupnpcontrolpoint.cpp \
        $$PWD/qtupnpserviceinfo.cpp \
        $$PWD/qtupnpserviceproxy.cpp \
        $$PWD/qtupnpservice.cpp \
        $$PWD/qthttpheader.cpp \
        $$PWD/qtmulticastbus.cpp \
        $$PWD/qtudpsocket.cpp \
        $$PWD/qtssdpresourcebrowser.cpp \
        $$PWD/qtssdpresourcegroup.cpp \
        $$PWD/qtupnpserviceintrospection.cpp \
        $$PWD/qtupnpserviceactionargument.cpp \
        $$PWD/qtupnpserviceaction.cpp \
        $$PWD/qtupnpservicestatevariable.cpp \
        $$PWD/qtsoapinvocationhandler.cpp \
        $$PWD/qtgenanotificationhandler.cpp \
        $$PWD/qtgenasubscriptionhandler.cpp \
        $$PWD/qthttpstandardhandler.cpp
    HEADERS += $$PWD/qthttpserver.h \
        $$PWD/qtupnpdeviceinfo.h \
        $$PWD/qtupnpdeviceproxy.h \
        $$PWD/qtupnpdevice.h \
        $$PWD/qtupnprootdevice.h \
        $$PWD/qtupnpcontrolpoint.h \
        $$PWD/qtupnpserviceinfo.h \
        $$PWD/qtupnpserviceproxy.h \
        $$PWD/qtupnpservice.h \
        $$PWD/qthttpheader.h \
        $$PWD/qtmulticastbus.h \
        $$PWD/qtudpsocket.h \
        $$PWD/qtupnpprotocol.h \
        $$PWD/qtssdpresourcebrowser.h \
        $$PWD/qtssdpresourcegroup.h \
        $$PWD/qthttpserverhandlerinterface.h \
        $$PWD/qtupnpserviceintrospection.h \
        $$PWD/qtupnpserviceactionargument.h \
        $$PWD/qtupnpserviceaction.h \
        $$PWD/qtupnpservicestatevariable.h \
        $$PWD/qtsoapinvocationhandler.h \
        $$PWD/qtgenanotificationhandler.h \
        $$PWD/qtgenasubscriptionhandler.h \
        $$PWD/qthttpstandardhandler.h
}
win32 { 
    contains(TEMPLATE, lib):contains(CONFIG, shared):DEFINES += QT_QUPNP_EXPORT
    else:qupnp-uselib:DEFINES += QT_QUPNP_IMPORT
}
HEADERS += qtupnpcontext.h \
    qtupnpresourcefactory.h \
    qtsharedpointer.h \
    qtupnpcontextmanager.h
SOURCES += qtupnpcontext.cpp \
    qtupnpresourcefactory.cpp \
    qtupnpcontextmanager.cpp
