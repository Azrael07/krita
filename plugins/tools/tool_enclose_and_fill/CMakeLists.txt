set(kritatoolencloseandfill_SOURCES
    KisToolEncloseAndFill.cpp
    KisToolEncloseAndFillPlugin.cpp
    subtools/KisRectangleEnclosingProducer.cpp
    subtools/KisEllipseEnclosingProducer.cpp
    subtools/KisPathEnclosingProducer.cpp
    subtools/KisLassoEnclosingProducer.cpp
    subtools/KisToolBasicBrushBase.cpp
    subtools/KisBrushEnclosingProducer.cpp
    )

kis_add_library(kritatoolencloseandfill MODULE ${kritatoolencloseandfill_SOURCES})

target_link_libraries(kritatoolencloseandfill kritaui kritabasicflakes)

install(TARGETS kritatoolencloseandfill  DESTINATION ${KRITA_PLUGIN_INSTALL_DIR})

install( FILES KisToolEncloseAndFill.action DESTINATION  ${KDE_INSTALL_DATADIR}/krita/actions)
