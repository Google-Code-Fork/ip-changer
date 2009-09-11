include(${QT_USE_FILE})

add_subdirectory(pList)


set(ip-changer_SRCS
    main.cpp
    mainwindow.cpp
    history.cpp
    client.cpp
    process.cpp
)



set(ip-changer_UIS
    mainwindow.ui
)

QT4_WRAP_UI(ip-changer_UIS_H ${ip-changer_UIS})



set(ip-changer_MOC_HDRS
    mainwindow.h
)

QT4_WRAP_CPP(ip-changer_MOC_SRCS ${ip-changer_MOC_HDRS})



set(ip-changer_RCS
    resources.qrc
)

QT4_ADD_RESOURCES(ip-changer_RC_SRCS ${ip-changer_RCS})



add_executable(ip-changer ${ip-changer_SRCS} ${ip-changer_UIS_H} ${ip-changer_MOC_SRCS} ${ip-changer_RC_SRCS})

target_link_libraries(ip-changer ${QT_LIBRARIES} pList)



install(
    TARGETS ip-changer
    RUNTIME DESTINATION /usr/bin

)

install(
    DIRECTORY versions DESTINATION /usr/share/ip-changer	
)
