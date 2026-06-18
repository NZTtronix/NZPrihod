#ifndef PERSONMODEL_H
#define PERSONMODEL_H

#include <QAbstractListModel>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct Person {
    Person() = default;
    Person(const QString &_name, const int &_age);
    QString name = "";
    int age = 0;
};

class PersonModel : public QAbstractListModel {
    Q_OBJECT
  public:
    enum PersonRoles { NameRole = Qt::UserRole + 1, AgeRole };

    PersonModel(QObject *parent = nullptr);
    Q_INVOKABLE void addEmptyPerson();
    Q_INVOKABLE void deletePerson( int row);
    Q_INVOKABLE bool loadFromJson(const QString path );
    Q_INVOKABLE bool saveToJson(const QString path);

    void addPerson(const Person &person);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  protected:
    QHash<int, QByteArray> roleNames() const;

  private:
    QString fileName;
    QJsonObject qJsonObjLoaded;
    QList<Person> people;
};

#endif