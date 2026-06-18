#include "BarcodesModel.h"

QHash<int, QByteArray> BarcodesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[WidthRole] = "WidthRole";
    roles[HeightRole] = "HeightRole";
    roles[CenterXRole] = "CenterXRole";
    roles[CenterYRole] = "CenterYRole";
    roles[AngleRole] = "AngleRole";
    roles[AreaRole] = "AreaRole";
    roles[IdxRole] = "IdxRole";
    roles[ContentRole] = "ContentRole";
    roles[CodeTypeRole] = "CodeTypeRole";

    return roles;
};

BarcodesModel::BarcodesModel(QObject *parent) : QAbstractListModel(parent) {};
QVariant BarcodesModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= barcodeItems.count()) return QVariant();
    const BarcodeItem &item = barcodeItems[index.row()];

    switch (role) {
    case WidthRole: return QVariant(item.width);
    case HeightRole: return QVariant(item.height);
    case CenterXRole: return QVariant(item.centerX);
    case CenterYRole: return QVariant(item.centerY);
    case AngleRole: return QVariant(item.angle);
    case AreaRole: return QVariant(item.area);
    case IdxRole: return QVariant(index.row());
    case ContentRole: return QVariant(item.content);
    case CodeTypeRole: return QVariant(item.codeType);
    default: return QVariant();
    }
    return QVariant();
}
int BarcodesModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return barcodeItems.count();
}

bool BarcodesModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;
    const int row = index.row();
    if (row < 0 || row >= barcodeItems.size()) return false;

    BarcodeItem &item = barcodeItems[index.row()];
    bool changed = false;
    switch (role) {
    case WidthRole:
        if (item.width != value.toDouble()) {
            item.width = value.toDouble();
            changed = true;
        }
        break;
      case HeightRole:
        if (item.height != value.toDouble()) {
            item.height = value.toDouble();
            changed = true;
        }
        break;
           case CenterXRole:
        if (item.centerX != value.toDouble()) {
            item.centerX = value.toDouble();
            changed = true;
        }
        break;
           case CenterYRole:
        if (item.centerY != value.toDouble()) {
            item.centerY = value.toDouble();
            changed = true;
        }
        break;
           case AngleRole:
        if (item.angle != value.toDouble()) {
            item.angle = value.toDouble();
            changed = true;
        }
        break;
     case AreaRole:
        if (item.area != value.toDouble()) {
            item.area = value.toDouble();
            changed = true;
        }
        break;
    case ContentRole:
        if (item.content != value.toString()) {
            item.content = value.toString();
            changed = true;
        }
        break;
    case CodeTypeRole:
        if (item.codeType != value.toString()) {
            item.codeType = value.toString();
            changed = true;
        }
        break;
    case IdxRole:
        if (item.idx != value.toInt()) {
            item.idx = value.toInt();
            changed = true;
        }
        break;
    default: return false;
    }
    if (!index.isValid()) return false;

    if (changed) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
};

bool BarcodesModel::loadFromJson(const QString path) {
    QFile qjsonfile(path);
    qDebug() << "loadFromJson(" << path << ")";
    if (!qjsonfile.open(QIODevice::ReadOnly)) {
        qDebug() << "cant open file";
        return false;
    }
    const QByteArray qbytearrayfiledata = qjsonfile.readAll();
    qjsonfile.close();

    QJsonParseError err;
    QJsonDocument qjsondoc = QJsonDocument::fromJson(qbytearrayfiledata, &err);
    QJsonObject qjsonobj = qjsondoc.object();
    if (!qjsonobj.contains("barcodes") || !qjsonobj.value("barcodes").isArray()) {
        return false;
    }
    qJsonObjLoaded = qjsonobj;
    QJsonArray qjsonarraymatch = qjsonobj["barcodes"].toArray();

    barcodeItems.clear();
    beginResetModel();

    for (int i = 0; i < qjsonarraymatch.size(); i++) {
        QJsonObject obj = qjsonarraymatch.at(i).toObject();
        BarcodeItem item;
        qDebug() << obj;
        QJsonDocument doc(obj);
        qDebug() << doc.toJson(QJsonDocument::Indented);

        item.width = obj["width"].toDouble();
        item.height = obj["height"].toDouble();
        item.centerX = obj["centerX"].toDouble();
        item.centerY = obj["centerY"].toDouble();
        item.angle = obj["angle"].toDouble();
        item.area = obj["area"].toDouble();
        item.content = obj["content"].toString();
        item.codeType = obj["type"].toString();
        item.idx = i;

        barcodeItems.append(item);
    }
    endResetModel();
    return true;
};

bool BarcodesModel::saveToJson(const QString path) {
    QFile qjsonfile(path);
    QJsonArray barcodesArray;
    for (const BarcodeItem &item : barcodeItems) {
        QJsonObject obj;
        obj["width"]= item.width;  
        obj["height"]= item.height;
        obj["centerX"]= item.centerX;
        obj["centerY"]= item.centerY;
        obj["angle"]= item.angle;
        obj["area"]= item.area;
        obj["content"]= item.content;
        obj["type"]= item.codeType;
        barcodesArray.append(obj);       
    }
    qJsonObjLoaded.insert("barcodes", barcodesArray);
    QJsonDocument qjsondoc;
    qjsondoc.setObject(qJsonObjLoaded);

    if (!qjsonfile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    qjsonfile.write(qjsondoc.toJson(QJsonDocument::Indented));
    qjsonfile.close();

    return true;
}
