#include "PersonModel.h"
// Person::Person(const QString _name, const int &_age)
//     : m_name(name), m_age(age) {}
 
// std::string Person::name() const { return m_name; }
 
// int Person::age() const { return m_age; }

Person::Person(const QString &_name, const int &_age): name(_name), age(_age){};


PersonModel::PersonModel(QObject *parent)
    : QAbstractListModel(parent) { }

void PersonModel::addPerson(const Person &Person)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    people << Person;
    endInsertRows();
}
void PersonModel::addEmptyPerson()
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    Person emptyperson("name", 0);
    people << emptyperson;
    endInsertRows();
}
void PersonModel::deletePerson(int row)
{
    beginRemoveRows(QModelIndex(),row,row);
    people.remove(row,1);
     endRemoveRows();
}
bool PersonModel::saveToJson(const QString path)  {
    QFile qjsonfile(path);
    QJsonArray jsonArray;

    for (int i = 0; i< people.size(); i++){
        QJsonObject obj;
        Person currentPerson = people[i];
        obj["name"]  = currentPerson.name;
        obj["age"]  = currentPerson.age;
        jsonArray.append(obj);
    }
     qJsonObjLoaded.insert("Match",jsonArray);
    QJsonDocument qjsondoc;
    qjsondoc.setObject(qJsonObjLoaded);
    qjsonfile.open(QIODevice::WriteOnly| QIODevice::Truncate);
    qjsonfile.write(qjsondoc.toJson(QJsonDocument::Indented));
    qjsonfile.close();
    return true;
}
bool PersonModel::loadFromJson(const QString path)  {
    QFile qjsonfile(path);
    qDebug()<<"loadFromJson("<< path<<")";
    if (!qjsonfile.open(QIODevice::ReadOnly)) {
        qDebug()<< "cant open file";
        return false;
    }
    const QByteArray qbytearrayfiledata = qjsonfile.readAll();
        qjsonfile.close();

    QJsonParseError err;
    QJsonDocument qjsondoc = QJsonDocument::fromJson(qbytearrayfiledata,&err);
    QJsonObject qjsonobj = qjsondoc.object();
    if (!qjsonobj.contains("Match") || !qjsonobj.value("Match").isArray()) {
        qjsonobj.insert("Match", QJsonArray());
        qjsondoc.setObject(qjsonobj);
        qjsonfile.open(QIODevice::WriteOnly| QIODevice::Truncate);
        qjsonfile.write(qjsondoc.toJson(QJsonDocument::Indented));
        qjsonfile.close();
    }
    qJsonObjLoaded = qjsonobj;
    QJsonArray qjsonarraymatch = qjsonobj["Match"].toArray();

    people.clear();
    beginResetModel();
    for (int i = 0; i< qjsonarraymatch.size(); i++) {
        QJsonObject currentObject = qjsonarraymatch.at(i).toObject();
        Person currentPerson;
        currentPerson.name = currentObject["name"].toString();
        currentPerson.age =  currentObject["age"].toInt();
        people.append(currentPerson);
    }
    endResetModel();
    return true;

}   


int PersonModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return people.count();
}

QVariant PersonModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= people.count()) return QVariant();
 
    const Person &Person = people[index.row()];
    if (role == NameRole)
        return QVariant(Person.name);
    else if (role == AgeRole)
        return QVariant(Person.age);
    return QVariant();
}

QHash<int, QByteArray> PersonModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[AgeRole] = "age";
    return roles;
}