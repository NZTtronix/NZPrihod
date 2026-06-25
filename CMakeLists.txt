cmake_minimum_required(VERSION 3.19)
set(CMAKE_PROJECT_NAME QmlApp)

project(${CMAKE_PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(ZXing_INCLUDE_DIRS C:/ZXing/include)
set(ZXing_DIR C:/zxing_cpp_vs/build)
set(OpenCV_DIR C:/OpenCV/OpenCV-MinGW-Build-OpenCV-4.5.2-x64)



list(APPEND CMAKE_PREFIX_PATH "C:/ZXing")
list(APPEND CMAKE_PREFIX_PATH "C:/OpenCV/OpenCV-MinGW-Build-OpenCV-4.5.2-x64")


find_package(Qt6 6.5 REQUIRED COMPONENTS
    Quick
    Multimedia
        Qml
        Gui
        PrintSupport
        Widgets

)
find_package(ZXing REQUIRED )
find_package(OpenCV REQUIRED)
qt_standard_project_setup()

qt_add_executable(QmlApp
    Core/Src/main.cpp
)


qt_add_qml_module(QmlApp
    URI ${CMAKE_PROJECT_NAME}URI
    VERSION 1.0
    


        QML_FILES   qml/main.qml
                    qml/TemplateConfiguration.qml
                    qml/MainPage.qml
                    qml/MainMenuSideBar.qml
                    qml/CustomListView.qml   
                    qml/CustomPageHeader.qml
                    qml/KeywordsConfiguration.qml
                    qml/PrintConfiguration.qml
                    qml/TemplateMatchConditions.qml
                    # qml/printtable.qml
                    qml/FieldTable.qml
                    qml/PrintSizeForm.ui.qml
                    qml/ExtractionListView.ui.qml
                    qml/ExtractionListViewDelegate.ui.qml
                    qml/ExtractionListViewModel.qml
                    # qml/MainPageToolBar.ui.qml
                    # qml/MainPageToolBarDelegate.ui.qml
                    # qml/MainPageToolBarModel.qml
                    qml/MainPageButton.ui.qml

        SOURCES  
         Core/Src/barcodeanalyzer.cpp
         Core/Src/barcodelist.cpp
         Core/Src/mybarcodeinfo.cpp
         Core/Src/PersonModel.cpp
         Core/Src/KeywordsModel.cpp
         Core/Src/MatchModel.cpp
         Core/Src/BarcodesModel.cpp
         Core/Src/FieldListModel.cpp


         Core/Include/barcodeanalyzer.h
         Core/Include/barcodelist.h 
         Core/Include/mybarcodeinfo.h 
         Core/Include/PersonModel.h 
         Core/Include/KeywordsModel.h 
         Core/Include/MatchModel.h 
         Core/Include/BarcodesModel.h 
         Core/Include/FieldListModel.h

         RESOURCES 
            qml/Resources/icons/back.svg
            qml/Resources/icons/home.svg
            qml/Resources/icons/info.svg
            qml/Resources/icons/print.svg
            qml/Resources/icons/settings.svg
            qml/Resources/icons/templates.svg
            qml/Resources/icons/qr_code_scanner.svg
            qml/Resources/icons/back_white.svg
            qml/Resources/icons/image_search.svg
            qml/Resources/icons/keywords.svg
            qml/Resources/icons/rule.svg
            qml/Resources/icons/history.svg
            qml/Resources/icons/label.svg
            qml/Resources/icons/add_a_photo.svg
            qml/Resources/icons/add_photo_alternate.svg
            qml/Resources/icons/delete.svg
            qml/Resources/icons/lightbulb.svg
            qml/Resources/icons/live_tv.svg
            qml/Resources/icons/tv_off.svg



)





target_link_libraries(QmlApp
    PRIVATE
        Qt6::Quick
        Qt6::Multimedia
            Qt6::Qml
        Qt6::PrintSupport
        ${OpenCV_LIBS}
        ZXing::ZXing
        
)

target_include_directories(QmlApp PRIVATE
    ${ZXing_INCLUDE_DIRS}
    "C:/ZXing/include"
    "C:/ZXing/include/ZXing"
    "C:/ZXing/lib"
    ${OpenCV_INCLUDE_DIRS}
    "C:/Qt/6.9.2/mingw_64/bin"
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/qml
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources/icons

        

)

include(GNUInstallDirs)

install(TARGETS QmlApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

 


