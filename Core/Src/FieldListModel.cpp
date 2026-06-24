#include "FieldListModel.h"
#include <QDebug>

FieldListModel::FieldListModel(QObject *parent) : QAbstractListModel(parent) {}

QHash<int, QByteArray> FieldListModel::roleNames() const {
    QHash<int, QByteArray> roles;
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
    qDebug() << "setData is called" << "index is " << index << "value" << value << " role " << role;
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
bool FieldListModel::setData(int row, const QVariant &value, int role) {
    if (row < 0 || row >= m_items.size()) return false;
    QModelIndex idx = index(row, 0);
    return setData(idx, value, role);
}

QVariant FieldListModel::data(int row, int role) const {
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

void FieldListModel::deleteItem(int row) {
    beginRemoveRows(QModelIndex(), row, row);
    m_items.remove(row, 1);
    endRemoveRows();
};

bool FieldListModel::updatePosition(int row, double x, double y) {
    bool changedY = false;
    bool changedX = false;
    QModelIndex idx = index(row, 0, QModelIndex());
    QList<int> roles;
    roles.clear();
    if (row >= 0, row < m_items.size()) {
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
    if (roles.size() > 0) {
        emit dataChanged(idx, idx, roles);
        return true;
    }

    return false;
}

bool FieldListModel::loadFromJson(const QString path) {
    qDebug() << "loadFromJson: qFile(" << path << ")";
    QFile qFile(path);
    if (!qFile.open(QIODevice::ReadOnly)) {
        qDebug() << "cant open file";
        return false;
    }
    const QByteArray qByteArray = qFile.readAll();
    qFile.close();
    QJsonParseError qJsonParseError;
    QJsonDocument qJsonDocument = QJsonDocument::fromJson(qByteArray, &qJsonParseError);
    QJsonObject qJsonObject = qJsonDocument.object();
    if (!qJsonObject.contains("FieldItems") || !qJsonObject.value("FieldItems").isArray()) {
        qJsonObject.insert("FieldItems", QJsonArray());
        qJsonDocument.setObject(qJsonObject);
        qFile.open(QIODevice::WriteOnly | QIODevice::Truncate);
        qFile.write(qJsonDocument.toJson(QJsonDocument::Indented));
        qFile.close();
    }
    m_fullJson = qJsonObject;
    QJsonArray qJsonArray = qJsonObject["FieldItems"].toArray();

    m_items.clear();
    beginResetModel();

    for (int i = 0; i < qJsonArray.size(); i++) {
        QJsonObject obj = qJsonArray.at(i).toObject();
        FieldItem fieldItem;
        fieldItem.X = obj["XRole"].toDouble();
        fieldItem.Y = obj["YRole"].toDouble();
        fieldItem.width = obj["WidthRole"].toDouble();
        fieldItem.height = obj["HeightRole"].toDouble();
        fieldItem.content = obj["ContentRole"].toString();
        fieldItem.codeType = obj["CodeTypeRole"].toString();
        m_items.append(fieldItem);
    }
    endResetModel();
    return true;

}
bool FieldListModel::saveToJson(const QString path) {
    qDebug() << "saveToJson: qFile(" << path << ")";
    QFile qFile(path);
    QJsonArray qJsonArray;
    for (int i=0; i< m_items.size(); i++){
        FieldItem fieldItem = m_items[i];
        QJsonObject obj;
        obj["XRole"]=fieldItem.X;
        obj["YRole"]=fieldItem.Y;
        obj["WidthRole"]=fieldItem.width;
        obj["HeightRole"]=fieldItem.height;
        obj["ContentRole"]=fieldItem.content;
        obj["CodeTypeRole"]=fieldItem.codeType;
        qJsonArray.append(obj);
    }
    m_fullJson.insert("FieldItems", qJsonArray);


    QJsonDocument qJsonDocument;
    qJsonDocument.setObject(m_fullJson);

    if (!qFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return false;
    }
    qFile.write(qJsonDocument.toJson(QJsonDocument::Indented));
    qFile.close();

    return true;

}
 void FieldListModel::print(QString labelTemplateName) {
        
    
    
 }    
void FieldListModel::print(QImage img) {


}

void FieldListModel::print(QQuickItemGrabResult *pr) {
    qDebug() << "FieldListModel::print is called";
    qDebug() << "QQuickItemGrabResult *pr is ", pr;

    if (!pr) return;
    QImage img = pr->image();   // <- получить QImage
    if (img.isNull()) {
            qWarning() << "print: image is null";
            return;
        }
        QPrinter printer(QPrinter::HighResolution);
        printer.setResolution(300);
        printer.setPageSize(QPageSize(QPageSize::A4));
        printer.setFullPage(false);
        printer.setPrinterName("Pantum M6500W Series");
        QPainter painter;
        if (!painter.begin(&printer)) {
            qDebug() << "printImage: cannot begin painter on printer";
            return;
        }
        qDebug() << "painter on printer began";

        
        // Страница в логических координатах принтера
    QRectF pageRect = printer.pageRect(QPrinter::DevicePixel);
                qDebug() << "pageRect.width()="<<pageRect.width()
                <<" pageRect.height()="<<pageRect.height();

    // Размер изображения в пикселях
    const qreal pxW = img.width();
    const qreal pxH = img.height();
            qDebug() << "pxW="<<pxW<<" pxH="<<pxH;

    //размер в мм: 
    qreal pxWmm=pxW/5.58;
    qreal pxHmm=pxH/5.58;
            qDebug() << "pxWmm="<<pxW<<" pxHmm="<<pxH;
        qreal pxWinches=pxWmm/25.4;
    qreal pxHinches=pxHmm/25.4;
            qDebug() << "pxHinches="<<pxW<<" pxWinches="<<pxH;

    // dpi принтера (пикселей на дюйм)
    const qreal dpi = printer.resolution();
        qDebug() << "dpi="<<dpi;

    // Размер в "физических" единицах печати: пиксели -> дюймы -> пункты
    // QPrinter рисует в пунктах (1 pt = 1/72 inch), поэтому:
    // pt = (px / dpi) * 72
    const qreal wPt = pxWinches * dpi;
    const qreal hPt = pxHinches * dpi;
                qDebug() << "wPt="<<wPt<<" hPt="<<hPt;

    // Центрирование
    const qreal x = (pageRect.width() - wPt) / 2.0;
    const qreal y = (pageRect.height() - hPt) / 2.0;
                qDebug() << "x="<<x<<" y="<<y;

    painter.drawImage(QRectF(x, y, wPt, hPt), img);

    painter.end();
      
    
 }
        
        

QString FieldListModel::GenerateBarcode(int row, QString barcodetext, QString barcodeformat) {

    ZXing::BarcodeFormat format;
    qDebug() << "barcodeformat = " << barcodeformat;

    if (barcodeformat == QString("Code39")) {
        format = ZXing::BarcodeFormat::Code39;
        qDebug() << "barcodeformat = " << barcodeformat;
        auto barcode = ZXing::CreateBarcodeFromText(barcodetext.toStdString(), format);
        auto svgBytes = ZXing::WriteBarcodeToSVG(barcode);
        QString svg = QString::fromUtf8(svgBytes); // ваш SVG
        QString encoded = QUrl::toPercentEncoding(svg, QByteArray());
        QString dataUrl = "data:image/svg+xml;utf8," + encoded;
        qDebug() << dataUrl << "\n";
        return dataUrl;
    } else if (barcodeformat == QString("QRCode")) {
        format = ZXing::BarcodeFormat::QRCode;
        qDebug() << "barcodeformat = " << barcodeformat;
        auto barcode = ZXing::CreateBarcodeFromText(barcodetext.toStdString(), format);
        auto svgBytes = ZXing::WriteBarcodeToSVG(barcode);
        QString svg = QString::fromUtf8(svgBytes); // ваш SVG
        QString encoded = QUrl::toPercentEncoding(svg, QByteArray());
        QString dataUrl = "data:image/svg+xml;utf8," + encoded;
        qDebug() << dataUrl << "\n";
        return dataUrl;
    } else {
        format = ZXing::BarcodeFormat::None;
        qDebug() << "ToString(format) = " << ToString(format);
    }

    // auto barcode = ZXing::CreateBarcodeFromText(barcodetext.toStdString(), format);
    // auto svgBytes = ZXing::WriteBarcodeToSVG(barcode);
    // QString svg = QString::fromUtf8(svgBytes);                   // ваш SVG
    // QString encoded = QUrl::toPercentEncoding(svg,QByteArray());
    // QString dataUrl = "data:image/svg+xml;utf8," + encoded;
    // qDebug()<< dataUrl << "\n";
    QString dataString = barcodetext;
    return dataString;
}
