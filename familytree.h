#ifndef FAMILYTREE_H
#define FAMILYTREE_H

#include "QTextStream"
#include <QDate>

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

class FamilyTree
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
    QList<FamilyTree*> m_childs;


public:
    FamilyTree();

    //TO DO: Сделать перегрузку операторов сравнения, присвоения ...

    bool isCreated = false; //После того как разделю save на save и create можно убрать

    const QString& getFIO();
    const Parents& getParents();
    const QDate& getDateOfBirth();
    const bool& isAlive();
    const QDate& getDateOfDeath();
    const Gender& getGender();
    const Country& getCitizenship();
    const bool& WasInMilitaryService();
    QList<FamilyTree*>& getChilds();

    void setFIO(QString);
    void setFatherName(QString);
    void setMatherName(QString);
    void setDateOfBirth(QDate);
    void setIsAlive(bool);
    void setDateOfDeath(QDate);
    void setGender(Gender);
    void setCitizenship(Country);
    void setWasInMilitaryService(bool);
    void setChild(FamilyTree*, int);
    void addChild(FamilyTree*);

    ~FamilyTree();
};


#endif // FAMILYTREE_H
