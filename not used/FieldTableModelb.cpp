// FieldTableModel.cpp
#include "FieldTableModel.h"
#include <QDebug>

FieldTableModel::FieldTableModel(QObject *parent) : QAbstractTableModel(parent) {
    // начальные данные (пример)
    m_items.append(FieldItem{10,10,100,40,"A1","Type1"});
    m_items.append(FieldItem{130,10,120,50,"B2","Type2"});
}

QVariant FieldTableModel::data(const QModelIndex &index, int role) const {
        qDebug()<<"data(const QModelIndex &index, int role) ) is "<<
    "data("<<index << ","<<  role<<")";

    if (!index.isValid()) return {};
    const FieldItem &it = m_items.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
            case 0: return it.X;
            case 1: return it.Y;
            case 2: return it.width;
            case 3: return it.height;
            case 4: return it.content;
            case 5: return it.codeType;
        }
    }
    switch (role) {
        case XRole: return it.X;
        case YRole: return it.Y;
        case WidthRole: return it.width;
        case HeightRole: return it.height;
        case ContentRole: return it.content;
        case CodeTypeRole: return it.codeType;
    }
    return {};
}

QHash<int,QByteArray> FieldTableModel::roleNames() const {
    QHash<int,QByteArray> roles;
    roles[XRole] = "X";
    roles[YRole] = "Y";
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    roles[ContentRole] = "content";
    roles[CodeTypeRole] = "codeType";
    return roles;
}
Qt::ItemFlags FieldTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}
// Qt::ItemFlags FieldTableModel::flags(const QModelIndex &index) const 
// {
//     Q_UNUSED(index)
//     return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
// }

// Qt::ItemFlags FieldTableModel::flags(const QModelIndex &index) const {
//     if (!index.isValid()) return Qt::NoItemFlags;
//     return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
// }
bool FieldTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid()) return false;
         qDebug()<< "SetDataIsCalled with args " << index << "  "<< value  << "   " << role ;

    FieldItem &it = m_items[index.row()];
    bool changed = false;
    if (role == Qt::EditRole) {
        switch (index.column()) {
            case 0: it.X = value.toDouble(); changed = true; break;
            case 1: it.Y = value.toDouble(); changed = true; break;
            case 2: it.width = value.toDouble(); changed = true; break;
            case 3: it.height = value.toDouble(); changed = true; break;
            case 4: it.content = value.toString(); changed = true; break;
            case 5: it.codeType = value.toString(); changed = true; break;
        }
    } else {
        if (role == XRole) { it.X = value.toDouble(); changed = true; }
        else if (role == YRole) { it.Y = value.toDouble(); changed = true; }
        else if (role == WidthRole) { it.width = value.toDouble(); changed = true; }
        else if (role == HeightRole) { it.height = value.toDouble(); changed = true; }
        else if (role == ContentRole) { it.content = value.toString(); changed = true; }
        else if (role == CodeTypeRole) { it.codeType = value.toString(); changed = true; }
    }
    
    if (changed) {
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

// bool FieldTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
//     if (!index.isValid()) return false;
    
//     qDebug()<< "SetDataIsCalled with args " << index << "  "<< value  << "   " << role ;
//     int r = index.row();
//     if (r < 0 || r >= m_items.size()) return false;
//     FieldItem &it = m_items[r];

//     bool changed = false;
//     if (role == XRole || (role == Qt::EditRole && index.column() == 0)) {
//         double v = value.toDouble(); if (!qFuzzyCompare(it.X + 1.0, v + 1.0)) { it.X = v; changed = true; }
//     } else if (role == YRole || (role == Qt::EditRole && index.column() == 1)) {
//         double v = value.toDouble(); if (!qFuzzyCompare(it.Y + 1.0, v + 1.0)) { it.Y = v; changed = true; }
//     } else if (role == WidthRole || (role == Qt::EditRole && index.column() == 2)) {
//         double v = value.toDouble(); if (!qFuzzyCompare(it.width + 1.0, v + 1.0)) { it.width = v; changed = true; }
//     } else if (role == HeightRole || (role == Qt::EditRole && index.column() == 3)) {
//         double v = value.toDouble(); if (!qFuzzyCompare(it.height + 1.0, v + 1.0)) { it.height = v; changed = true; }
//     } else if (role == ContentRole || (role == Qt::EditRole && index.column() == 4)) {
//         QString s = value.toString(); if (it.content != s) { it.content = s; changed = true; }
//     } else if (role == CodeTypeRole || (role == Qt::EditRole && index.column() == 5)) {
//         QString s = value.toString(); if (it.codeType != s) { it.codeType = s; changed = true; }
//     }

//     if (changed) {
//         qDebug()<< "dataChanged emmitted with args " << index << "  "<< index  << "   " << role ;
//         emit dataChanged(index, index, {role});
//         return true;
//     }
//     return false;
// }

bool FieldTableModel::insertRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    if (row < 0 || row > m_items.size()) return false;
    beginInsertRows(QModelIndex(), row, row + count - 1);
    for (int i=0;i<count;++i) m_items.insert(row, FieldItem());
    endInsertRows();
    return true;
}

bool FieldTableModel::removeRows(int row, int count, const QModelIndex &parent) {
    Q_UNUSED(parent);
    if (row < 0 || row + count > m_items.size()) return false;
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    for (int i=0;i<count;++i) m_items.removeAt(row);
    endRemoveRows();
    return true;
}

void FieldTableModel::addRow() { insertRows(m_items.size(), 1); }
void FieldTableModel::removeRow(int row) { removeRows(row, 1); }
void FieldTableModel::clear() {
    if (m_items.isEmpty()) return;
    beginRemoveRows(QModelIndex(), 0, m_items.size()-1);
    m_items.clear();
    endRemoveRows();
}

QVariantMap FieldTableModel::getItem(int row) const {
    QVariantMap map;
    qDebug()<<"inside getItem  m_items.at(row).X: " << m_items.at(row).X
    << "  m_items.at(row).Y:"  << m_items.at(row).Y << " row = " <<row;

    if (row < 0 || row >= m_items.size()) return map;

    const FieldItem &it = m_items.at(row);
    map["X"] = it.X;
    map["Y"] = it.Y;
    map["width"] = it.width;
    map["height"] = it.height;
    map["content"] = it.content;
    map["codeType"] = it.codeType;
            qDebug()<<"inside getItem map[X] is: "<< map["X"]<< "map[Y] is "<< map["Y"] ;

    return map;
}
