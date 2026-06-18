#ifndef KEYWORDSMODEL_H
#define KEYWORDSMODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QSortFilterProxyModel>

struct KeywordItem {
    KeywordItem() = default;
    KeywordItem(QString _ID, QString _Name, bool _Enabled);
    QString ID = "keyword ID";
    QString Name = "keyword name";
    bool Enabled = true;
};

class KeywordsModel : public QAbstractListModel {
    Q_OBJECT
  public:
    enum KeywordRoles {
        IDRole = Qt::UserRole + 1,
        NameRole,
        EnabledRole,
    };
    KeywordsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);


    Q_INVOKABLE bool loadFromJson(const QString path);
    Q_INVOKABLE bool saveToJson(const QString path);
    Q_INVOKABLE void addEmptyItem();
    Q_INVOKABLE void deleteItem(int row);
  private:

    QList<KeywordItem> keywordslist;
    QJsonObject qJsonObjLoaded;
};



class EnabledProxyModel : public QSortFilterProxyModel {
    Q_OBJECT
public:
    explicit EnabledProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override {
        QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
        // роль имя в модели: "EnabledRole" → роль число: roleNames() или роль enum в KeywordsModel
        QVariant enabled = sourceModel()->data(idx,  KeywordsModel::EnabledRole);
        // если unsure, можно проверять по имени роли через roleNames()
        return enabled.toBool();
    }
};










#endif

