#ifndef PRINTTABLEMODEL_H
#define PRINTTABLEMODEL_H
#include <QAbstractTableModel>
#include <QQmlEngine>  // Нужен для QML-регистрации
#include <QTimer>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

struct FieldItem {
    FieldItem() = default;
    double X=0;
    double Y=0;
    double width=20;
    double height=20;
    QString content = "null";
    QString codeType = "QRCode";
};

class FieldTableModel : public QAbstractTableModel
{
    Q_OBJECT
   
public:
    explicit PrintTableModel(QObject *parent = nullptr);

  
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE bool loadFromJson(const QString path);
    Q_INVOKABLE bool saveToJson(const QString path);

    static QObject *createSingleton(QQmlEngine *engine, QJSEngine *scriptEngine) {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        return new PrintTableModel();
    }
    
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void timerHit();

private: 
    QList<FieldItem> m_items;
    QJsonObject m_fullJson;

    
    QTimer *m_timer;

};










#endif
