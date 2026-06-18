#ifndef BARCODESMODEL_H
#define BARCODESMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QSortFilterProxyModel>


struct BarcodeItem {
    BarcodeItem() = default;
    double width=0;
    double height=0;
    double centerX=0;
    double centerY=0;
    double angle=0;
    double area=0;
    int idx = 0;
    QString content = "";
    QString codeType = "";
};


class BarcodesModel : public QAbstractListModel {
    Q_OBJECT
  public:
    enum BarcodesRoles {
        WidthRole = Qt::UserRole + 1,
        HeightRole,
        CenterXRole,
        CenterYRole,
        AngleRole,
        AreaRole,
        IdxRole,
        ContentRole,
        CodeTypeRole,
        

    };
    BarcodesModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);


    Q_INVOKABLE bool loadFromJson(const QString path);
    Q_INVOKABLE bool saveToJson(const QString path);
    // Q_INVOKABLE void addEmptyItem();
    // Q_INVOKABLE void deleteItem(int row);
  private:

    QList<BarcodeItem> barcodeItems;
    QJsonObject qJsonObjLoaded;
};









#endif

