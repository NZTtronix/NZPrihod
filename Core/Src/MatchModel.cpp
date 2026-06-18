#include "MatchModel.h"

int MatchModel::rowCount(const QModelIndex &parent) const { return m_items.size(); };
MatchModel::MatchModel(QObject *parent) : QAbstractListModel(parent) {}
QVariant MatchModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    const int row = index.row();
    if (row < 0 || row >= m_items.size()) return QVariant();
    const MatchItem &it = m_items.at(row);
    switch (role) {
    case Qt::DisplayRole: return QVariant(it.Keyword);
    case CodeIDRole: return QVariant(it.CodeID);
    case KeywordRole: return QVariant(it.Keyword);
    case MatchStartRole: return QVariant(it.MatchStart);
    case MatchMiddleRole: return QVariant(it.MatchMiddle);
    case MatchEndRole: return QVariant(it.MatchEnd);
    case MatchSplitRole: return QVariant(it.MatchSplit);
    case MatchMinLengthRole: return QVariant(it.MatchMinLength);
    case MatchMaxLengthRole: return QVariant(it.MatchMaxLength);
    case CaseSensitiveRole: return QVariant(it.CaseSensitive);
    case MatchISNumberRole: return QVariant(it.MatchISNumber);
    case OnlyTakeBeNumberRole: return QVariant(it.OnlyTakeBeNumber);
    case CharacteristicRole: return QVariant(it.Characteristic);
    case CodeTypeRole: return QVariant(it.CodeType);
    case StartTextRole: return QVariant(it.StartText);
    case MiddleTextRole: return QVariant(it.MiddleText);
    case EndTextRole: return QVariant(it.EndText);
    case SplitTextRole: return QVariant(it.SplitText);
    case MinLengthRole: return QVariant(it.MinLength);
    case MaxLengthRole: return QVariant(it.MaxLength);
    case MatchMiddleTypeRole: return QVariant(it.MatchMiddleType);
    case MiddleTextCountRole: return QVariant(it.MiddleTextCount);
    case SubstringStartRole: return QVariant(it.SubstringStart);
    case SubstringLengthRole: return QVariant(it.SubstringLength);
    case SplitPartRole: return QVariant(it.SplitPart);
    default: return QVariant();
    }
};

QHash<int, QByteArray> MatchModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CodeIDRole] = "CodeIDRole";
    roles[KeywordRole] = "KeywordRole";
    roles[CheckCodeTypeRole] = "CheckCodeTypeRole";
    roles[MatchStartRole] = "MatchStartRole";
    roles[MatchMiddleRole] = "MatchMiddleRole";
    roles[MatchEndRole] = "MatchEndRole";
    roles[MatchSplitRole] = "MatchSplitRole";
    roles[MatchMinLengthRole] = "MatchMinLengthRole";
    roles[MatchMaxLengthRole] = "MatchMaxLengthRole";
    roles[CaseSensitiveRole] = "CaseSensitiveRole";
    roles[MatchISNumberRole] = "MatchISNumberRole";
    roles[OnlyTakeBeNumberRole] = "OnlyTakeBeNumberRole";
    roles[CharacteristicRole] = "CharacteristicRole";
    roles[CodeTypeRole] = "CodeTypeRole";
    roles[StartTextRole] = "StartTextRole";
    roles[MiddleTextRole] = "MiddleTextRole";
    roles[EndTextRole] = "EndTextRole";
    roles[SplitTextRole] = "SplitTextRole";
    roles[MinLengthRole] = "MinLengthRole";
    roles[MaxLengthRole] = "MaxLengthRole";
    roles[MatchMiddleTypeRole] = "MatchMiddleTypeRole";
    roles[MiddleTextCountRole] = "MiddleTextCountRole";
    roles[SubstringStartRole] = "SubstringStartRole";
    roles[SubstringLengthRole] = "SubstringLengthRole";
    roles[SplitPartRole] = "SplitPartRole";
    return roles;
}




bool MatchModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;
    const int row = index.row();
    if (row < 0 || row >= m_items.size()) return false;

    MatchItem &item = m_items[index.row()];
    bool changed = false;
    switch (role) {
    case CodeIDRole:
        if (item.CodeID != value.toInt()) {
            item.CodeID = value.toInt();
            


            changed = true;
        }
        break;
    case KeywordRole:
        if (item.Keyword != value.toString()) {
            item.Keyword = value.toString();
            changed = true;
        }
        break;
    case CheckCodeTypeRole:
        if (item.CheckCodeType != value.toBool()) {
            item.CheckCodeType = value.toBool();
            changed = true;
        }
        break;
    case MatchStartRole:
        if (item.MatchStart != value.toBool()) {
            item.MatchStart = value.toBool();
            changed = true;
        }
        break;
    case MatchMiddleRole:
        if (item.MatchMiddle != value.toBool()) {
            item.MatchMiddle = value.toBool();
            changed = true;
        }
        break;
    case MatchEndRole:
        if (item.MatchEnd != value.toBool()) {
            item.MatchEnd = value.toBool();
            changed = true;
        }
        break;
    case MatchSplitRole:
        if (item.MatchSplit != value.toBool()) {
            item.MatchSplit = value.toBool();
            changed = true;
        }
        break;
    case MatchMinLengthRole:
        if (item.MatchMinLength != value.toBool()) {
            item.MatchMinLength = value.toBool();
            changed = true;
        }
        break;
    case MatchMaxLengthRole:
        if (item.MatchMaxLength != value.toBool()) {
            item.MatchMaxLength = value.toBool();
            changed = true;
        }
        break;
    case CaseSensitiveRole:
        if (item.CaseSensitive != value.toBool()) {
            item.CaseSensitive = value.toBool();
            changed = true;
        }
        break;
    case MatchISNumberRole:
        if (item.MatchISNumber != value.toBool()) {
            item.MatchISNumber = value.toBool();
            changed = true;
        }
        break;
    case OnlyTakeBeNumberRole:
        if (item.OnlyTakeBeNumber != value.toBool()) {
            item.OnlyTakeBeNumber = value.toBool();
            changed = true;
        }
        break;
    case CharacteristicRole:
        if (item.Characteristic != value.toBool()) {
            item.Characteristic = value.toBool();
            changed = true;
        }
        break;
    case CodeTypeRole:
        if (item.CodeType != value.toString()) {
            item.CodeType = value.toString();
            changed = true;
        }
        break;
    case StartTextRole:
        if (item.StartText != value.toString()) {
            item.StartText = value.toString();
            changed = true;
        }
        break;
    case MiddleTextRole:
        if (item.MiddleText != value.toString()) {
            item.MiddleText = value.toString();
            changed = true;
        }
        break;
    case EndTextRole:
        if (item.EndText != value.toString()) {
            item.EndText = value.toString();
            changed = true;
        }
        break;
    case SplitTextRole:
        if (item.SplitText != value.toString()) {
            item.SplitText = value.toString();
            changed = true;
        }
        break;
    case MinLengthRole:
        if (item.MinLength != value.toInt()) {
            item.MinLength = value.toInt();
            changed = true;
        }
        break;
    case MaxLengthRole:
        if (item.MaxLength != value.toInt()) {
            item.MaxLength = value.toInt();
            changed = true;
        }
        break;
    case MatchMiddleTypeRole:
        if (item.MatchMiddleType != value.toInt()) {
            item.MatchMiddleType = value.toInt();
            changed = true;
        }
        break;
    case MiddleTextCountRole:
        if (item.MiddleTextCount != value.toInt()) {
            item.MiddleTextCount = value.toInt();
            changed = true;
        }
        break;
    case SubstringStartRole:
        if (item.SubstringStart != value.toInt()) {
            item.SubstringStart = value.toInt();
            changed = true;
        }
        break;
    case SubstringLengthRole:
        if (item.SubstringLength != value.toInt()) {
            item.SubstringLength = value.toInt();
            changed = true;
        }
        break;
    case SplitPartRole:
        if (item.SplitPart != value.toInt()) {
            item.SplitPart = value.toInt();
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
}

bool MatchModel::loadFromJson(const QString &filePath) {
    QFile qjsonfile(filePath);
    
    qDebug() << "loadFromJson(" << filePath << ")";
    if (!qjsonfile.open(QIODevice::ReadOnly)) {
        qDebug() << "cant open file";
        return false;
    }
    const QByteArray qbytearrayfiledata = qjsonfile.readAll();
    qjsonfile.close();

    QJsonParseError err;
    QJsonDocument qjsondoc = QJsonDocument::fromJson(qbytearrayfiledata, &err);
    QJsonObject qjsonobj = qjsondoc.object();
    if (!qjsonobj.contains("Match") || !qjsonobj.value("Match").isArray()) {
        qjsonobj.insert("Match", QJsonArray());
        qjsondoc.setObject(qjsonobj);
        qjsonfile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        qjsonfile.write(qjsondoc.toJson(QJsonDocument::Indented));
        qjsonfile.close();
    }
    m_fullJson = qjsonobj;
    QJsonArray qjsonarraymatch = qjsonobj["Match"].toArray();

    m_items.clear();
    beginResetModel();

    for (int i = 0; i < qjsonarraymatch.size(); i++) {
        QJsonObject obj = qjsonarraymatch.at(i).toObject();
        MatchItem item;
        item.CodeID = obj["CodeID"].toInt();
        item.Keyword = obj["Keyword"].toString();
        item.CheckCodeType = obj["CheckCodeType"].toBool();
        item.MatchStart = obj["MatchStart"].toBool();
        item.MatchMiddle = obj["MatchMiddle"].toBool();
        item.MatchEnd = obj["MatchEnd"].toBool();
        item.MatchSplit = obj["MatchSplit"].toBool();
        item.MatchMinLength = obj["MatchMinLength"].toBool();
        item.MatchMaxLength = obj["MatchMaxLength"].toBool();
        item.CaseSensitive = obj["CaseSensitive"].toBool();
        item.MatchISNumber = obj["MatchISNumber"].toBool();
        item.OnlyTakeBeNumber = obj["OnlyTakeBeNumber"].toBool();
        item.Characteristic = obj["Characteristic"].toBool();
        item.CodeType = obj["CodeType"].toString();
        item.StartText = obj["StartText"].toString();
        item.MiddleText = obj["MiddleText"].toString();
        item.EndText = obj["EndText"].toString();
        item.SplitText = obj["SplitText"].toString();
        item.MinLength = obj["MinLength"].toInt();
        item.MaxLength = obj["MaxLength"].toInt();
        item.MatchMiddleType = obj["MatchMiddleType"].toInt();
        item.MiddleTextCount = obj["MiddleTextCount"].toInt();
        item.SubstringStart = obj["SubstringStart"].toInt();
        item.SubstringLength = obj["SubstringLength"].toInt();
        item.SplitPart = obj["SplitPart"].toInt();
        m_items.append(item);
    }
    endResetModel();
    return true;
}

void MatchModel::setDataAccordingToBarcodesModel(BarcodesModel *barcodesmod)
{           
        for(int i = 0;i  < rowCount();i++){
            QVariant temp = barcodesmod->data(barcodesmod->index(i,0), BarcodesModel::CodeTypeRole);
            qDebug()<<"CodeTypeRole from b:"<<temp.toString();
            QModelIndex idx = index(i, 0);
            setData(idx,temp,CodeTypeRole);
            qDebug()<< data(idx, CodeTypeRole);
        }
           
}

void MatchModel::addEmptyItem(void) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    MatchItem defaultMatchItem;
    m_items.append(defaultMatchItem);
    endInsertRows();
}

void MatchModel::deleteItem(int row)
{
    beginRemoveRows(QModelIndex(),row,row);
    m_items.remove(row,1);
    endRemoveRows();
}
void MatchModel::debugPrint(int index) {
    qDebug() << "========== DEBUG START ==========";

    // 1. Выводим весь JSON, который сейчас сформирован из m_items
    QJsonArray matchArray;
    for (const MatchItem &item : m_items) {
        QJsonObject obj;
        obj["CodeID"] = item.CodeID;
        obj["Keyword"] = item.Keyword;
        obj["MatchStart"] = item.MatchStart;
        obj["MatchMiddle"] = item.MatchMiddle;
        obj["MatchEnd"] = item.MatchEnd;
        obj["MatchSplit"] = item.MatchSplit;
        obj["CheckCodeType"] = item.CheckCodeType;
        obj["CodeType"] = item.CodeType;
        obj["StartText"] = item.StartText;
        obj["MiddleText"] = item.MiddleText;
        obj["MatchMiddleType"] = item.MatchMiddleType;
        obj["MiddleTextCount"] = item.MiddleTextCount;
        obj["EndText"] = item.EndText;
        obj["SplitText"] = item.SplitText;
        obj["CaseSensitive"] = item.CaseSensitive;
        obj["MatchMinLength"] = item.MatchMinLength;
        obj["MatchMaxLength"] = item.MatchMaxLength;
        obj["MinLength"] = item.MinLength;
        obj["MaxLength"] = item.MaxLength;
        obj["SubstringStart"] = item.SubstringStart;
        obj["SubstringLength"] = item.SubstringLength;
        obj["SplitPart"] = item.SplitPart;
        obj["MatchISNumber"] = item.MatchISNumber;
        obj["OnlyTakeBeNumber"] = item.OnlyTakeBeNumber;
        obj["Characteristic"] = item.Characteristic;
        matchArray.append(obj);
    }
    QJsonObject rootObj;
    rootObj["Match"] = matchArray;

    QJsonDocument doc(rootObj);
    qDebug() << "--- CURRENT FULL JSON ---";
    qDebug().noquote() << doc.toJson(QJsonDocument::Indented);

    // 2. Выводим пары ключ-значение для конкретного индекса
    qDebug() << "--- ITEM AT INDEX:" << index << "---";
    if (index < 0 || index >= m_items.size()) {
        qDebug() << "Error: Index out of range!";
    } else {
        const MatchItem &item = m_items[index];
        qDebug() << "CodeID:" << item.CodeID;
        qDebug() << "Keyword:" << item.Keyword;
        qDebug() << "MatchStart:" << item.MatchStart;
        qDebug() << "MatchMiddle:" << item.MatchMiddle;
        qDebug() << "MatchEnd:" << item.MatchEnd;
        qDebug() << "MatchSplit:" << item.MatchSplit;
        qDebug() << "CheckCodeType:" << item.CheckCodeType;
        qDebug() << "CodeType:" << item.CodeType;
        qDebug() << "StartText:" << item.StartText;
        qDebug() << "MiddleText:" << item.MiddleText;
        qDebug() << "MatchMiddleType:" << item.MatchMiddleType;
        qDebug() << "MiddleTextCount:" << item.MiddleTextCount;
        qDebug() << "EndText:" << item.EndText;
        qDebug() << "SplitText:" << item.SplitText;
        qDebug() << "CaseSensitive:" << item.CaseSensitive;
        qDebug() << "MatchMinLength:" << item.MatchMinLength;
        qDebug() << "MatchMaxLength:" << item.MatchMaxLength;
        qDebug() << "MinLength:" << item.MinLength;
        qDebug() << "MaxLength:" << item.MaxLength;
        qDebug() << "SubstringStart:" << item.SubstringStart;
        qDebug() << "SubstringLength:" << item.SubstringLength;
        qDebug() << "SplitPart:" << item.SplitPart;
        qDebug() << "MatchISNumber:" << item.MatchISNumber;
        qDebug() << "OnlyTakeBeNumber:" << item.OnlyTakeBeNumber;
        qDebug() << "Characteristic:" << item.Characteristic;
    }

    qDebug() << "========== DEBUG END ==========";
}

bool MatchModel::saveToJson(const QString &filePath) {
    QFile qjsonfile(filePath);
    // Открываем файл для записи. Если он существовал, содержимое сотрется (Truncate)
  

    QJsonArray matchArray;

    // Конвертируем каждый элемент структуры в QJsonObject
    for (const MatchItem &item : m_items) {
        QJsonObject obj;

        // Числа (сохраняются как числа в JSON)
        obj["CodeID"] = item.CodeID;
        obj["MinLength"] = item.MinLength;
        obj["MaxLength"] = item.MaxLength;
        obj["MatchMiddleType"] = item.MatchMiddleType;
        obj["MiddleTextCount"] = item.MiddleTextCount;
        obj["SubstringStart"] = item.SubstringStart;
        obj["SubstringLength"] = item.SubstringLength;
        obj["SplitPart"] = item.SplitPart;

        // Булевы флаги (сохраняются как true/false в JSON)
        obj["MatchStart"] = item.MatchStart;
        obj["MatchMiddle"] = item.MatchMiddle;
        obj["MatchEnd"] = item.MatchEnd;
        obj["MatchSplit"] = item.MatchSplit;
        obj["CheckCodeType"] = item.CheckCodeType;
        obj["CaseSensitive"] = item.CaseSensitive;
        obj["MatchMinLength"] = item.MatchMinLength;
        obj["MatchMaxLength"] = item.MatchMaxLength;
        obj["MatchISNumber"] = item.MatchISNumber;
        obj["OnlyTakeBeNumber"] = item.OnlyTakeBeNumber;
        obj["Characteristic"] = item.Characteristic;

        // Строки
        obj["Keyword"] = item.Keyword;
        obj["CodeType"] = item.CodeType;
        obj["StartText"] = item.StartText;
        obj["MiddleText"] = item.MiddleText;
        obj["EndText"] = item.EndText;
        obj["SplitText"] = item.SplitText;

        matchArray.append(obj);
    }

    m_fullJson.insert("Match",matchArray);
    QJsonDocument qjsondoc;
    qjsondoc.setObject(m_fullJson);

    if (!qjsonfile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    qjsonfile.write(qjsondoc.toJson(QJsonDocument::Indented));
    qjsonfile.close();

    return true;
}
