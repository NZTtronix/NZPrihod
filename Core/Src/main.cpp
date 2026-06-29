#include <QCoreApplication>
#include <QGuiApplication>
#include <ZXingQt.h> // Ensure you link the zxing-cpp Qt wrapper module
#include <QQmlApplicationEngine>
#include "barcodeanalyzer.h"
#include "MatchModel.h"
#include "PersonModel.h"
#include "KeywordsModel.h"
#include "BarcodesModel.h"
#include "FieldListModel.h"
#include <QQmlContext>
#include "MyCamera.h"
#include "MyImageProvider.h"


int main(int argc, char *argv[])
{   
    QGuiApplication  app(argc, argv);
    QQmlApplicationEngine engine;

    MyCamera cam;
    
    qmlRegisterType<MatchModel>("MyComponents", 1, 0, "MatchModel");
    qmlRegisterType<PersonModel>("PersonModel", 1, 0, "PersonModel");
    qmlRegisterType<KeywordsModel>("KeywordsModel", 1, 0, "KeywordsModel");
    qmlRegisterType<BarcodesModel>("BarcodesModel", 1, 0, "BarcodesModel");

    qmlRegisterType<EnabledProxyModel>("EnabledProxyModel", 1, 0, "EnabledProxyModel");

    qmlRegisterType<BarcodeAnalyzer>("Backend", 1, 0, "BarcodeAnalyzer");
    qmlRegisterType<FieldListModel>("FieldListModel", 1, 0, "FieldListModel");

    
    MyImageProvider *provider = new MyImageProvider(&cam);
    
    engine.addImageProvider("myimageprovider", provider);





    engine.rootContext()->setContextProperty("myCamera", &cam);

    engine.addImportPath("C:/zxing-cpp/wrappers/qt");
    engine.addImportPath("C:/zxing-cpp");
    
    cam.initCamera();
    const QUrl url("qrc:/QmlAppURI/qml/main.qml"); // ссылка на файл qml
    engine.load(url); 
    return app.exec();

}
#include "moc_ZXingQt.cpp"
