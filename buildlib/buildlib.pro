TEMPLATE=lib
CONFIG += qt dll qupnp-buildlib
QT += network xml
mac:CONFIG += absolute_library_soname
win32|mac:!wince*:!win32-msvc:!macx-xcode:CONFIG += debug_and_release build_all
include(../src/qupnp.pri)
TARGET = $$QUPNP_LIBNAME
DESTDIR = $$QUPNP_LIBDIR
win32 {
    DLLDESTDIR = $$[QT_INSTALL_BINS]
    QMAKE_DISTCLEAN += $$[QT_INSTALL_BINS]\\$${QUPNP_LIBNAME}.dll
}
target.path = $$DESTDIR
INSTALLS += target
