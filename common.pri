infile(config.pri, LIBRARY, yes): CONFIG += qupnp-uselib
TEMPLATE += fakelib
QUPNP_LIBNAME = $$qtLibraryTarget(QtUPnP-0.1)
TEMPLATE -= fakelib
QUPNP_LIBDIR = $$PWD/lib
unix:qupnp-uselib:!qupnp-buildlib:QMAKE_RPATHDIR += $$QUPNP_LIBDIR
