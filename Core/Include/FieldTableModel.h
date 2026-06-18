// FieldTableModel.h
#pragma once
#include <QAbstractTableModel>
#include <QVector>

struct FieldItem {
    double X = 0;
    double Y = 0;
    double width = 20;
    double height = 20;
    QString content = "null";
    QString codeType = "QRCode";
};

class FieldTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    enum Roles {
        XRole = Qt::UserRole + 1,
        YRole,
        WidthRole,
        HeightRole,
        ContentRole,
        CodeTypeRole
    };

    explicit FieldTableModel(QObject *parent = nullptr);

    Q_INVOKABLE int rowCount(const QModelIndex &parent = QModelIndex()) const override { Q_UNUSED(parent); return m_items.size(); }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override { Q_UNUSED(parent); return 6; }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
Q_INVOKABLE bool setFieldXY(int row, int x, int y) {
    if (row < 0 || row >= m_items.size()) return false;
    QModelIndex idx = index(row, 0);
    bool ok1 = setData(idx, x, XRole);
    bool ok2 = setData(idx, y, YRole);
    return ok1 && ok2;
    
}
    Q_INVOKABLE void addRow();
    Q_INVOKABLE void removeRow(int row);
    Q_INVOKABLE void clear();
    Q_INVOKABLE QVariantMap getItem(int row) const;

private:
    QVector<FieldItem> m_items;
};
