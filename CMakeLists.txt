cmake_minimum_required(VERSION 3.19)
project(QmlApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(ZXing_INCLUDE_DIRS C:/ZXing/include)
set(ZXing_DIR C:/zxing_cpp_vs/build)
set(OpenCV_DIR C:/OpenCV/OpenCV-MinGW-Build-OpenCV-4.5.2-x64)

list(APPEND CMAKE_PREFIX_PATH "C:/ZXing")
list(APPEND CMAKE_PREFIX_PATH "C:/OpenCV/OpenCV-MinGW-Build-OpenCV-4.5.2-x64")


find_package(Qt6 6.5 REQUIRED COMPONENTS
    Quick
    Multimedia
)
find_package(ZXing REQUIRED )
find_package(OpenCV REQUIRED)
qt_standard_project_setup()

qt_add_executable(QmlApp
    main.cpp
)
# qt_add_resources(QmlApp "icons"
#     PREFIX "/icons"
#     FILES
        
#         Resources/icons/home.svg
#         Resources/icons/print.svg
#         Resources/icons/qr_code_scanner.svg
#         Resources/icons/info.svg
#         Resources/icons/templates.svg
#         Resources/icons/back.svg
# )


qt_add_qml_module(QmlApp
    URI ZXing
    VERSION 1.0
    


        QML_FILES main.qml
                    TemplateConfiguration.qml
                    MainPage.qml
                    MainMenuSideBar.qml
                    TemplatesSideBar.qml
                    TemplateProperties.qml
                    TemplatePropertyRecognition.qml
                    TemplatePropertyParse.qml   
                    CustomListView.qml   
                    CustomPageHeader.qml
                    KeywordsConfiguration.qml
                    PrintConfiguration.qml
                    
        SOURCES barcodeanalyzer.h barcodeanalyzer.cpp
                 opencvimageprovider.h
         barcodeanalyzer.h barcodeanalyzer.cpp
         fullmaterialtemplate.h fullmaterialtemplate.cpp
         barcodelist.h barcodelist.cpp
         mybarcodeinfo.h mybarcodeinfo.cpp
         printtemplate.h printtemplate.cpp
         keywordtemplate.h keywordtemplate.cpp
         MatchModel.h MatchModel.cpp
         RESOURCES 
            Resources/icons/back.svg
            Resources/icons/home.svg
            Resources/icons/info.svg
            Resources/icons/print.svg
            Resources/icons/settings.svg
            Resources/icons/templates.svg
            Resources/icons/qr_code_scanner.svg
            Resources/icons/back_white.svg
            Resources/icons/image_search.svg
            Resources/icons/keywords.svg
            Resources/icons/rule.svg
            Resources/icons/history.svg
                        Resources/icons/label.svg


            



)


target_link_libraries(QmlApp
    PRIVATE
        Qt6::Quick
        Qt6::Multimedia
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

)

include(GNUInstallDirs)

install(TARGETS QmlApp
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

