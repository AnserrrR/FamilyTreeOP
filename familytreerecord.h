#ifndef FAMILYTREERECORD_H
#define FAMILYTREERECORD_H


#include "QTextStream"
#include <QDate>
#include <QDataStream>

enum Gender
{
    MALE,
    FEMALE
};

enum Country
{
    RUSSIA,
    UKRAINE,
    POLAND,
    GERMANY,
    USA,
    ISRAEL,
    JAPAN,
    CYPRUS
};

struct Parents
{
    QString fatherName;
    QString matherName;
};

class FamilyTreeRecord
{
private:
    QString m_FIO;
    Parents m_parents;
    QDate m_DateOfBirth;
    bool m_isAlive;
    QDate m_DateOfDeath;
    Gender m_gender;
    Country m_citizenship;
    bool m_wasInMilitaryService;
    QList<unsigned int> m_childs;
    unsigned int m_id;


public:
    FamilyTreeRecord();

    //TO DO: Сделать перегрузку операторов сравнения, присвоения ...
    bool operator<(const FamilyTreeRecord& other) const;
    bool operator>(const FamilyTreeRecord& other) const;
    void operator=(const FamilyTreeRecord& other);
    friend QDataStream& operator<<(QDataStream& out, const FamilyTreeRecord& record);
    friend QDataStream& operator>>(QDataStream& in, FamilyTreeRecord& record);

    const QString& getFIO() const;
    const Parents& getParents() const;
    const QDate& getDateOfBirth() const;
    const bool& isAlive() const;
    const QDate& getDateOfDeath() const;
    const Gender& getGender() const;
    const Country& getCitizenship() const;
    const bool& WasInMilitaryService() const;
    const unsigned int getID() const;
    QList<unsigned int>& getChilds();

    void setFIO(QString);
    void setFatherName(QString);
    void setMatherName(QString);
    void setDateOfBirth(QDate);
    void setIsAlive(bool);
    void setDateOfDeath(QDate);
    void setGender(Gender);
    void setCitizenship(Country);
    void setWasInMilitaryService(bool);
    void setID(unsigned int id);

    ~FamilyTreeRecord();
};



#endif // FAMILYTREERECORD_H
