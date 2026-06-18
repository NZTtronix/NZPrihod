#include "FieldListModel.h"
#include <QDebug>

FieldListModel::FieldListModel(QObject *parent) : QAbstractListModel(parent) {
   
}


QHash<int,QByteArray> FieldListModel::roleNames() const {
    QHash<int,QByteArray> roles;
    roles[XRole] = "X";
    roles[YRole] = "Y";
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    roles[ContentRole] = "content";
    roles[CodeTypeRole] = "codeType";
    return roles;
}
int FieldListModel::rowCount(const QModelIndex &parent) const { return m_items.size(); };
bool FieldListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    qDebug()<<"setData is called"<< "index is "<< index << "value" << value << " role " << role;
    if (!index.isValid()) return false;
    const int row = index.row();
    if (row < 0 || row >= m_items.size()) return false;

    FieldItem &item = m_items[index.row()];
    bool changed = false;
    switch (role) {
    case XRole:
        if (item.X != value.toDouble()) {
            item.X = value.toDouble();
            changed = true;
        }
        break;
    case YRole:
        if (item.Y != value.toDouble()) {
            item.Y = value.toDouble();
            changed = true;
        }
        break;
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
    case CodeTypeRole:
        if (item.codeType != value.toString()) {
            item.codeType = value.toString();
            changed = true;
        }
        break;
    case ContentRole:
        if (item.content != value.toString()) {
            item.content = value.toString();
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
bool FieldListModel::setData(int row, const QVariant &value, int role){
    if (row < 0 || row >= m_items.size()) return false;
    QModelIndex idx = index(row, 0);
    return setData(idx, value, role);
}

QVariant FieldListModel::data(int row, int role ) const  {
    if (row < 0 || row >= m_items.size()) return QVariant();
    QModelIndex idx = index(row, 0);
    return data(idx, role);
}
QVariant FieldListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) return QVariant();
    const int row = index.row();
    if (row < 0 || row >= m_items.size()) return QVariant();
    const FieldItem &it = m_items.at(row);
    switch (role) {
    // case Qt::DisplayRole: return QVariant(it.Keyword);
    case XRole: return QVariant(it.X);
    case YRole: return QVariant(it.Y);
    case WidthRole: return QVariant(it.width);
    case HeightRole: return QVariant(it.height);
    case ContentRole: return QVariant(it.content);
    case CodeTypeRole: return QVariant(it.codeType);
    
    default: return QVariant();
    }
};

void FieldListModel::addEmptyItem(void) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    FieldItem defaultFieldItem;
    m_items.append(defaultFieldItem);
    endInsertRows();
}

void FieldListModel::deleteItem(int row)
{
    beginRemoveRows(QModelIndex(),row,row);
    m_items.remove(row,1);
    endRemoveRows();
};

bool FieldListModel::updatePosition(int row, double x, double y){
        bool changedY = false;
        bool changedX = false;
        QModelIndex idx = index(row,0,QModelIndex());
        QList<int> roles;
        roles.clear();
    if (row>=0, row< m_items.size()) {
        FieldItem &item = m_items[row];
            if (item.X != x) {
                item.X = x;
                changedX = true;
                roles.append(XRole);
            }
            if (item.Y != y) {
                item.Y = y;
                changedY = true;
                roles.append(YRole);
            }    
    }
    if (roles.size()>0) {
        emit dataChanged(idx, idx, roles);
        return true;
    }
    
    
    return false;

    }








bool FieldListModel::loadFromJson(const QString path) {
    return false;
}
bool FieldListModel::saveToJson(const QString path) {
    return false;
}











    
    QString  FieldListModel::GenerateBarcode( int row, QString barcodetext, QString barcodeformat) {
        
        auto barcode = ZXing::CreateBarcodeFromText(barcodetext.toStdString(), ZXing::BarcodeFormat::QRCode);
        auto svgBytes = ZXing::WriteBarcodeToSVG(barcode);
        // see also ZXing::WriteBarcodeToImage()
        QString svg = QString::fromUtf8(svgBytes);                   // ваш SVG
        QString encoded = QUrl::toPercentEncoding(svg,QByteArray());
        QString dataUrl = "data:image/svg+xml;utf8," + encoded;
        qDebug()<< dataUrl << "\n";
        return dataUrl;
    }

