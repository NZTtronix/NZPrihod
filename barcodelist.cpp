#include "barcodelist.h"


#include <QFile>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


bool BarcodeList::saveToFile(const QString &filePath) const {
    QJsonObject root;
    root["name"] = QString::fromStdString(name);
    QJsonArray arr;
    for (size_t i = 0; i < items.size(); ++i) {
        arr.append(items[i].toJson());
    }
    root["barcodes"] = arr;

    QJsonDocument doc(root);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
    return true;
}

bool BarcodeList::isfullMatch(const BarcodeList &cmpBarcodeList) const {
    if (cmpBarcodeList.items.size() != items.size())
        return false;
    for (int i = 0; i < cmpBarcodeList.items.size(); i++) {
        if (items[i].compare(cmpBarcodeList.items[i]) == false)
            return false;
    }
    // for (int i = 0; i < cmpBarcodeList.items.size() - 1; i++) {
    //     if ((abs((items[i].centerX - items[i + 1].centerX) -
    //              (cmpBarcodeList.items[i].centerX -
    //               cmpBarcodeList.items[i + 1].centerX)) /
    //          (abs(cmpBarcodeList.items[i].centerX -
    //               cmpBarcodeList.items[i + 1].centerX))) > 0.1)
    //         return false;
    //     if ((abs((items[i].centerY - items[i + 1].centerY) -
    //              (cmpBarcodeList.items[i].centerY -
    //               cmpBarcodeList.items[i + 1].centerY)) /
    //          (abs(cmpBarcodeList.items[i].centerY -
    //               cmpBarcodeList.items[i + 1].centerY))) > 0.1)
    //         return false;
    // }
    return true;
}

bool BarcodeList::loadFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    const QByteArray data = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return false;
    QJsonObject root = doc.object();
    if (root.contains("name") && root["name"].isString())
        name = root["name"].toString().toStdString();

    items.clear();
    if (root.contains("barcodes") && root["barcodes"].isArray()) {
        QJsonArray jsonarr = root["barcodes"].toArray();

        for (int i = 0; i < jsonarr.size(); i++) {

            QJsonValue jv = jsonarr.at(i);
            if (!jv.isObject())
                continue;
            items.emplace_back();
            items.back().setfromJson(jv.toObject());
        }
    }
    return true;
}


