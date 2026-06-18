#include "KeywordsModel.h"

KeywordItem::KeywordItem(QString _ID, QString _Name, bool _Enabled) : ID(_ID), Name(_Name), Enabled(_Enabled) {};

KeywordsModel::KeywordsModel(QObject *parent) : QAbstractListModel(parent) {};

QHash<int, QByteArray> KeywordsModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[IDRole] = "IDRole";
    roles[NameRole] = "NameRole";
    roles[EnabledRole] = "EnabledRole";

    return roles;
};

QVariant KeywordsModel::data(const QModelIndex &index, int role) const {
    if (index.row() < 0 || index.row() >= keywordslist.count()) return QVariant();
    const KeywordItem &keywordItem = keywordslist[index.row()];

    switch (role) {
    case IDRole: return QVariant(keywordItem.ID);
    case NameRole: return QVariant(keywordItem.Name);
    case EnabledRole: return QVariant(keywordItem.Enabled);
    default: return QVariant();
    }
    return QVariant();
}

// QStringList KeywordsModel::enabledList() const {
//     QStringList outList;
//     for (int i = 0; i< keywordslist.size();i++){
//         if (keywordslist.at(i).Enabled==true){
//             outList.append(keywordslist.at(i).ID);
//         }
//     }
//     return outList;

// }
int KeywordsModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return keywordslist.count();
}

bool KeywordsModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;
    const int row = index.row();
    if (row < 0 || row >= keywordslist.size()) return false;

    KeywordItem &item = keywordslist[index.row()];
    bool changed = false;
    switch (role) {
    case IDRole:
        if (item.ID != value.toString()) {
            item.ID = value.toString();
            changed = true;
        }
        break;
    case NameRole:
        if (item.Name != value.toString()) {
            item.Name = value.toString();
            changed = true;
        }
        break;
    case EnabledRole:
        if (item.Enabled != value.toBool()) {
            item.Enabled = value.toBool();
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

bool KeywordsModel::loadFromJson(const QString path) {
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
    if (!qjsonobj.contains("Keywords") || !qjsonobj.value("Keywords").isArray()) {
        qjsonobj.insert("Keywords", QJsonArray());
        qjsondoc.setObject(qjsonobj);
        qjsonfile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        qjsonfile.write(qjsondoc.toJson(QJsonDocument::Indented));
        qjsonfile.close();
    }
    qJsonObjLoaded = qjsonobj;
    QJsonArray qjsonarraymatch = qjsonobj["Keywords"].toArray();

    keywordslist.clear();
    beginResetModel();

    for (int i = 0; i < qjsonarraymatch.size(); i++) {
        QJsonObject obj = qjsonarraymatch.at(i).toObject();
        KeywordItem item;
        item.ID = obj["IDRole"].toString();
        item.Name = obj["NameRole"].toString();
        item.Enabled = obj["EnabledRole"].toBool();

        keywordslist.append(item);
    }
    endResetModel();
    return true;
};

bool KeywordsModel::saveToJson(const QString path) {
    QFile qjsonfile(path);
    QJsonArray keywordsArray;
    for (const KeywordItem &item : keywordslist) {
        QJsonObject obj;
        obj["IDRole"] = item.ID;
        obj["NameRole"] = item.Name;
        obj["EnabledRole"] = item.Enabled;
        keywordsArray.append(obj);
    }
    qJsonObjLoaded.insert("Keywords", keywordsArray);
    QJsonDocument qjsondoc;
    qjsondoc.setObject(qJsonObjLoaded);

    if (!qjsonfile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    qjsonfile.write(qjsondoc.toJson(QJsonDocument::Indented));
    qjsonfile.close();

    return true;
}

void KeywordsModel::addEmptyItem(void) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    KeywordItem defaultMatchItem;
    keywordslist.append(defaultMatchItem);
    endInsertRows();
}

void KeywordsModel::deleteItem(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    keywordslist.remove(row, 1);
    endRemoveRows();
}
