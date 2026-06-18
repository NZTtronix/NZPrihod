#ifndef BARCODELIST_H
#define BARCODELIST_H
#include <QString>
#include "mybarcodeinfo.h"

struct BarcodeList {
    std::string name = "unnamed";
    std::vector<MyBarcodeInfo> items;

    void setName(const QString &qstr) { name = qstr.toStdString(); }
    bool saveToFile(const QString &filePath) const;

    bool isfullMatch(const BarcodeList &cmpBarcodeList) const;
    bool loadFromFile(const QString &filePath);
};

#endif // BARCODELIST_H
