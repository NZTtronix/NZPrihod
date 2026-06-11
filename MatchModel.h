

#pragma once

#include <QAbstractListModel>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QTime>
#include <QUrl>
#include <iostream>

struct MatchItem {
    MatchItem() = default;
    int CodeID = 0;

    QString Keyword = "";

    bool CheckCodeType = false;
    bool MatchStart = false;
    bool MatchMiddle = false;
    bool MatchEnd = false;
    bool MatchSplit = false;
    bool MatchMinLength = false;
    bool MatchMaxLength = false;
    bool CaseSensitive = false;
    bool MatchISNumber = false;
    bool OnlyTakeBeNumber = false;
    bool Characteristic = false;

    QString CodeType = "";
    QString StartText = "";
    QString MiddleText = "";
    QString EndText = "";
    QString SplitText = "";

    int MinLength = 0;
    int MaxLength = 0;
    int MatchMiddleType = -1;
    int MiddleTextCount = 1;

    int SubstringStart = 0;
    int SubstringLength = -1;
    int SplitPart = 1;
};

class MatchModel : public QAbstractListModel {
    Q_OBJECT
    public: 
    enum MatchRoles {
        CodeIDRole = Qt::UserRole + 1,
        KeywordRole,
        CheckCodeTypeRole,
        MatchStartRole,
        MatchMiddleRole,
        MatchEndRole,
        MatchSplitRole,
        MatchMinLengthRole,
        MatchMaxLengthRole,
        CaseSensitiveRole,
        MatchISNumberRole,
        OnlyTakeBeNumberRole,
        CharacteristicRole,

        CodeTypeRole,
        StartTextRole,
        MiddleTextRole,
        EndTextRole,
        SplitTextRole,
        
        MinLengthRole,
        MaxLengthRole,
        MatchMiddleTypeRole,
        MiddleTextCountRole,
        SubstringStartRole,
        SubstringLengthRole,
        SplitPartRole

    };
    explicit MatchModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Q_INVOKABLE bool loadFromJson(const QString &filePath);
    Q_INVOKABLE bool saveToJson(const QString &filePath);

    private: 
    QList<MatchItem> m_items;
    QJsonObject m_fullJson;



};



