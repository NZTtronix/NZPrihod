#include <QCoreApplication>
#include <QGuiApplication>
#include <ZXingQt.h> // Ensure you link the zxing-cpp Qt wrapper module
#include <QQmlApplicationEngine>
#include "barcodeanalyzer.h"
#include "opencvimageprovider.h"


int main(int argc, char *argv[])
{   
    QGuiApplication  app(argc, argv);
    QQmlApplicationEngine engine;
    QLOGGING_H
    // OpenCVImageProvider *provider = new OpenCVImageProvider();
    // engine.addImageProvider("ImageProvider", provider);


    qmlRegisterType<BarcodeAnalyzer>("Backend", 1, 0, "BarcodeAnalyzer");

    engine.addImportPath("C:/zxing-cpp/wrappers/qt");
    engine.addImportPath("C:/zxing-cpp");

    const QUrl url("qrc:/ZXing/main.qml"); // ссылка на файл qml
    engine.load(url);   // загружаем файл qml
    return app.exec();

}
#include "moc_ZXingQt.cpp"
