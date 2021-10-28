#include "familytree.h"

FamilyTree::FamilyTree()
{
    m_FIO = "none";
    m_parents.fatherName = "none";
    m_parents.matherName = "none";
    m_DateOfBirth.setDate(2000, 1, 1);
    m_isAlive = true;
    m_DateOfDeath.setDate(2000, 1, 1);
    m_gender = MALE;
    m_citizenship = RUSSIA;
    m_wasInMilitaryService = true;
}

FamilyTree::~FamilyTree()
{
}

bool FamilyTree::operator<(const FamilyTree &other) const
{
    if(m_DateOfBirth < other.m_DateOfBirth)
        return true;
    if(m_DateOfBirth == other.m_DateOfBirth)
        if(m_FIO < other.m_FIO)
            return true;
    return false;
}

void FamilyTree::operator=(const FamilyTree &other)
{
    m_FIO = other.m_FIO;
    m_DateOfBirth = other.m_DateOfBirth;
    m_isAlive = other.m_isAlive;
    m_DateOfDeath = other.m_DateOfDeath;
    m_gender = other.m_gender;
    m_citizenship = other.m_citizenship;
    m_wasInMilitaryService = other.m_wasInMilitaryService;
}

const QString& FamilyTree::getFIO()const{ return m_FIO;}
const Parents& FamilyTree::getParents()const{return m_parents;}
const QDate& FamilyTree::getDateOfBirth()const{return m_DateOfBirth;}
const bool& FamilyTree::isAlive()const{return m_isAlive;}
const QDate& FamilyTree::getDateOfDeath()const{return m_DateOfDeath;}
const Gender& FamilyTree::getGender()const{return m_gender;}
const Country& FamilyTree::getCitizenship()const{return m_citizenship;}
const bool& FamilyTree::WasInMilitaryService()const{return m_wasInMilitaryService;}
QList<FamilyTree*>& FamilyTree::getChilds(){ return m_childs; }

void FamilyTree::setFIO(QString newData){ m_FIO = newData;}
void FamilyTree::setFatherName(QString newData){m_parents.fatherName = newData;}
void FamilyTree::setMatherName(QString newData){m_parents.matherName = newData;}
void FamilyTree::setDateOfBirth(QDate newData){m_DateOfBirth = newData;}
void FamilyTree::setIsAlive(bool newData){m_isAlive = newData;}
void FamilyTree::setDateOfDeath(QDate newData){m_DateOfDeath = newData;}
void FamilyTree::setGender(Gender newData){m_gender = newData;}
void FamilyTree::setCitizenship(Country newData){m_citizenship = newData;}
void FamilyTree::setWasInMilitaryService(bool newData){m_wasInMilitaryService = newData;}




