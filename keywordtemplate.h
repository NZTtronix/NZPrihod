#ifndef KEYWORDTEMPLATE_H
#define KEYWORDTEMPLATE_H
#include <string>
#include <vector>
#include <QJsonArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

struct keyworditem{
    std::string fieldIdentifier;
    std::string displayName;
};

class KeywordTemplate
{
public:
    std::vector<keyworditem> keywordslist;

    bool loadFromFile(const QString &filePath) ;
    bool saveToFile(const QString &filePath) const;

};

#endif // KEYWORDTEMPLATE_H
