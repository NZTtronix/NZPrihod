#include "MatchModel.h"

int MatchModel::rowCount(const QModelIndex &parent) const { return m_items.size(); };
MatchModel::MatchModel(QObject *parent) 
    : QAbstractListModel(parent) 
{
    
}
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
    roles[CodeIDRole] = "codeIDRole";
    roles[KeywordRole] = "keywordRole";
    roles[CheckCodeTypeRole] = "checkCodeTypeRole";
    roles[MatchStartRole] = "matchStartRole";
    roles[MatchMiddleRole] = "matchMiddleRole";
    roles[MatchEndRole] = "matchEndRole";
    roles[MatchSplitRole] = "matchSplitRole";
    roles[MatchMinLengthRole] = "matchMinLengthRole";
    roles[MatchMaxLengthRole] = "matchMaxLengthRole";
    roles[CaseSensitiveRole] = "caseSensitiveRole";
    roles[MatchISNumberRole] = "matchISNumberRole";
    roles[OnlyTakeBeNumberRole] = "onlyTakeBeNumberRole";
    roles[CharacteristicRole] = "characteristicRole";
    roles[CodeTypeRole] = "codeTypeRole";
    roles[StartTextRole] = "startTextRole";
    roles[MiddleTextRole] = "middleTextRole";
    roles[EndTextRole] = "endTextRole";
    roles[SplitTextRole] = "splitTextRole";
    roles[MinLengthRole] = "minLengthRole";
    roles[MaxLengthRole] = "maxLengthRole";
    roles[MatchMiddleTypeRole] = "matchMiddleTypeRole";
    roles[MiddleTextCountRole] = "middleTextCountRole";
    roles[SubstringStartRole] = "substringStartRole";
    roles[SubstringLengthRole] = "substringLengthRole";
    roles[SplitPartRole] = "splitPartRole";
    return roles;
}

bool MatchModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;
    const int row = index.row();
    if (row < 0 || row >= m_items.size()) return false;
    
    MatchItem &item = m_items[index.row()];
    bool changed=false;
    switch (role){
case CodeIDRole: if (item.CodeID != value.toInt()) {item.CodeID = value.toInt(); changed = true;} break;
case KeywordRole: if (item.Keyword != value.toString()) {item.Keyword = value.toString(); changed = true;} break; 
case CheckCodeTypeRole:  if (item.CheckCodeType != value.toBool()) {item.CheckCodeType = value.toBool(); changed = true;} break;
case MatchStartRole: if (item.MatchStart != value.toBool()) {item.MatchStart = value.toBool(); changed = true;} break;
case MatchMiddleRole: if (item.MatchMiddle != value.toBool()) {item.MatchMiddle = value.toBool(); changed = true;} break;
case MatchEndRole: if (item.MatchEnd != value.toBool()) {item.MatchEnd = value.toBool(); changed = true;} break;
case MatchSplitRole: if (item.MatchSplit != value.toBool()) {item.MatchSplit = value.toBool(); changed = true;} break;
case MatchMinLengthRole: if (item.MatchMinLength != value.toBool()) {item.MatchMinLength = value.toBool(); changed = true;} break;
case MatchMaxLengthRole: if (item.MatchMaxLength != value.toBool()) {item.MatchMaxLength = value.toBool(); changed = true;} break;
case CaseSensitiveRole: if (item.CaseSensitive != value.toBool()) {item.CaseSensitive = value.toBool(); changed = true;} break;
case MatchISNumberRole: if (item.MatchISNumber != value.toBool()) {item.MatchISNumber = value.toBool(); changed = true;} break;
case OnlyTakeBeNumberRole: if (item.OnlyTakeBeNumber != value.toBool()) {item.OnlyTakeBeNumber = value.toBool(); changed = true;} break;
case CharacteristicRole: if (item.Characteristic != value.toBool()) {item.Characteristic = value.toBool(); changed = true;} break;
case CodeTypeRole: if (item.CodeType != value.toString()) {item.CodeType = value.toString(); changed = true;} break;
case StartTextRole: if (item.StartText != value.toString()) {item.StartText = value.toString(); changed = true;} break;
case MiddleTextRole: if (item.MiddleText != value.toString()) {item.MiddleText = value.toString(); changed = true;} break;
case EndTextRole: if (item.EndText != value.toString()) {item.EndText = value.toString(); changed = true;} break;
case SplitTextRole: if (item.SplitText != value.toString()) {item.SplitText = value.toString(); changed = true;} break;
case MinLengthRole: if (item.MinLength != value.toInt()) {item.MinLength = value.toInt(); changed = true;} break;
case MaxLengthRole: if (item.MaxLength != value.toInt()) {item.MaxLength = value.toInt(); changed = true;} break;
case MatchMiddleTypeRole: if (item.MatchMiddleType != value.toInt()) {item.MatchMiddleType = value.toInt(); changed = true;} break;
case MiddleTextCountRole: if (item.MiddleTextCount != value.toInt()) {item.MiddleTextCount = value.toInt(); changed = true;} break;
case SubstringStartRole: if (item.SubstringStart != value.toInt()) {item.SubstringStart = value.toInt(); changed = true;} break;
case SubstringLengthRole: if (item.SubstringLength != value.toInt()) {item.SubstringLength = value.toInt(); changed = true;} break;
case SplitPartRole: if (item.SplitPart != value.toInt()) {item.SplitPart = value.toInt(); changed = true;} break;
        default: return false;
}
    if (changed) {
        emit dataChanged(index, index, {role});
        return true;
    }

}

bool MatchModel::loadFromJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    const QByteArray data = file.readAll();
    file.close();
    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject())
        return false;
    
    QJsonObject rootObj  = doc.object();
    // if (root.contains("name") && root["name"].isString())
    //     match = root["name"].toString().toStdString();

    if ((!rootObj.contains("match")) || (!rootObj["match"].isArray())) return false;

        
        QJsonArray matchArray = rootObj["match"].toArray();
        beginResetModel();
        m_items.clear();
        

        for (const QJsonValue &val : matchArray) {
            QJsonObject obj = val.toObject();
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




bool MatchModel::saveToJson(const QString &filePath) {
    QFile file(filePath);
    // Открываем файл для записи. Если он существовал, содержимое сотрется (Truncate)
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }

    QJsonArray matchArray;

    // Конвертируем каждый элемент структуры в QJsonObject
    for (const MatchItem &item : m_items) {
        QJsonObject obj;

        // Числа (сохраняются как числа в JSON)
        obj["CodeID"]           = item.CodeID;
        obj["MinLength"]        = item.MinLength;
        obj["MaxLength"]        = item.MaxLength;
        obj["MatchMiddleType"]  = item.MatchMiddleType;
        obj["MiddleTextCount"]  = item.MiddleTextCount;
        obj["SubstringStart"]   = item.SubstringStart;
        obj["SubstringLength"]  = item.SubstringLength;
        obj["SplitPart"]        = item.SplitPart;

        // Булевы флаги (сохраняются как true/false в JSON)
        obj["MatchStart"]       = item.MatchStart;
        obj["MatchMiddle"]      = item.MatchMiddle;
        obj["MatchEnd"]         = item.MatchEnd;
        obj["MatchSplit"]       = item.MatchSplit;
        obj["CheckCodeType"]    = item.CheckCodeType;
        obj["CaseSensitive"]    = item.CaseSensitive;
        obj["MatchMinLength"]   = item.MatchMinLength;
        obj["MatchMaxLength"]   = item.MatchMaxLength;
        obj["MatchISNumber"]    = item.MatchISNumber;
        obj["OnlyTakeBeNumber"] = item.OnlyTakeBeNumber;
        obj["Characteristic"]   = item.Characteristic;

        // Строки
        obj["Keyword"]          = item.Keyword;
        obj["CodeType"]         = item.CodeType;
        obj["StartText"]        = item.StartText;
        obj["MiddleText"]       = item.MiddleText;
        obj["EndText"]          = item.EndText;
        obj["SplitText"]        = item.SplitText;

        matchArray.append(obj);
    }

    // Создаем корневой объект и упаковываем туда наш массив
    QJsonObject rootObj;
    rootObj["Match"] = matchArray;

    // Создаем документ и форматируем его с отступами (Indented) для красивого вида в файле
    QJsonDocument doc(rootObj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}










































