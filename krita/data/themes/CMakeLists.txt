########### install files ###############

if (FOUNDATION_BUILD)
    install(FILES
        BreezeDark.colors
        BreezeHighContrast.colors
        BreezeLight.colors
         ${schemefiles} DESTINATION ${KDE_INSTALL_DATADIR}/color-schemes
)
endif()

install(FILES
KritaNeutral.colors
KritaBlender.colors
KritaBright.colors
KritaDark.colors
KritaDarkOrange.colors
KritaDarker.colors
 ${schemefiles} DESTINATION ${KDE_INSTALL_DATADIR}/color-schemes
)

