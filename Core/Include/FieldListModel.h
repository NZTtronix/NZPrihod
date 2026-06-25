// FieldListModel.h
#pragma once
#include <QAbstractListModel>
#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPainter>
#include <QPrinter>
#include <QQuickItemGrabResult>
#include <QSortFilterProxyModel>
#include <QString>
#include <ZXing/ZXingCpp.h>

struct FieldItem {
    double X = 50;
    double Y = 50;
    double width = 150;
    double height = 150;
    QString content = "null";
    QString codeType = "QRCode";
};

class FieldListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(qreal canvasWidthMM READ canvasWidthMM WRITE setCanvasWidthMM NOTIFY canvasWidthMMChanged)
    Q_PROPERTY(qreal canvasHeightMM READ canvasHeightMM WRITE setCanvasHeightMM NOTIFY canvasHeightMMChanged)

  public:
    enum Roles {
        XRole = Qt::UserRole + 1,
        YRole,
        WidthRole,
        HeightRole,
        ContentRole,
        CodeTypeRole,

    };
    Q_ENUM(Roles);

    explicit FieldListModel(QObject *parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    Q_INVOKABLE bool setData(int row, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE QVariant data(int row, int role = Qt::DisplayRole) const;
    Q_INVOKABLE void addEmptyItem(void);
    Q_INVOKABLE void deleteItem(int row);

    Q_INVOKABLE bool updatePosition(int row, double x, double y);
    Q_INVOKABLE bool updateSize(int row, double w, double h);

    Q_INVOKABLE bool loadFromJson(const QString path);
    Q_INVOKABLE bool saveToJson(const QString path);
    Q_INVOKABLE QString GenerateBarcode(int row, QString barcodetext, QString barcodeformat);
    Q_INVOKABLE void print(QQuickItemGrabResult *pr);
    qreal canvasWidthMM() const { return m_canvasWidthMM; }
     qreal canvasHeightMM() const { return m_canvasHeightMM; }

    
  private:
    QList<FieldItem> m_items;
    QJsonObject m_fullJson;
    qreal m_canvasWidthMM;
    qreal m_canvasHeightMM;
    QString m_svg;

  signals:
    void canvasWidthMMChanged();
    void canvasHeightMMChanged();

  public slots:     
    void setCanvasWidthMM(qreal val);
    void setCanvasHeightMM(qreal val);
     

};
