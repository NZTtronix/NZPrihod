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


int main(int argc, char *argv[])
{   
    QGuiApplication  app(argc, argv);
    QQmlApplicationEngine engine;


    qmlRegisterType<MatchModel>("MyComponents", 1, 0, "MatchModel");
    qmlRegisterType<PersonModel>("PersonModel", 1, 0, "PersonModel");
    qmlRegisterType<KeywordsModel>("KeywordsModel", 1, 0, "KeywordsModel");
    qmlRegisterType<BarcodesModel>("BarcodesModel", 1, 0, "BarcodesModel");

    qmlRegisterType<EnabledProxyModel>("EnabledProxyModel", 1, 0, "EnabledProxyModel");

    qmlRegisterType<BarcodeAnalyzer>("Backend", 1, 0, "BarcodeAnalyzer");
    qmlRegisterType<FieldListModel>("FieldListModel", 1, 0, "FieldListModel");
    FieldListModel *model = new FieldListModel(&app);

  
    engine.rootContext()->setContextProperty("fieldListModel", model);



    engine.addImportPath("C:/zxing-cpp/wrappers/qt");
    engine.addImportPath("C:/zxing-cpp");
    
    const QUrl url("qrc:/QmlAppURI/qml/FieldTable.qml"); // ссылка на файл qml
    // const QUrl url("qrc:/QmlAppURI/qml/main.qml"); // ссылка на файл qml
    engine.load(url);   // загружаем файл qml
    return app.exec();

}
#include "moc_ZXingQt.cpp"
