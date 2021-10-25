#include "familytree.h"

FamilyTree::FamilyTree()
{
}

FamilyTree::~FamilyTree()
{
}

const QString& FamilyTree::getFIO(){ return m_FIO;}
const Parents& FamilyTree::getParents(){return m_parents;}
const QDate& FamilyTree::getDateOfBirth(){return m_DateOfBirth;}
const bool& FamilyTree::isAlive(){return m_isAlive;}
const QDate& FamilyTree::getDateOfDeath(){return m_DateOfDeath;}
const Gender& FamilyTree::getGender(){return m_gender;}
const Country& FamilyTree::getCitizenship(){return m_citizenship;}
const bool& FamilyTree::WasInMilitaryService(){return m_wasInMilitaryService;}

void FamilyTree::setFIO(QString newData){ m_FIO = newData;}
void FamilyTree::setFatherName(QString newData){m_parents.fatherName = newData;}
void FamilyTree::setMatherName(QString newData){m_parents.matherName = newData;}
void FamilyTree::setDateOfBirth(QDate newData){m_DateOfBirth = newData;}
void FamilyTree::setIsAlive(bool newData){m_isAlive = newData;}
void FamilyTree::setDateOfDeath(QDate newData){m_DateOfDeath = newData;}
void FamilyTree::setGender(Gender newData){m_gender = newData;}
void FamilyTree::setCitizenship(Country newData){m_citizenship = newData;}
void FamilyTree::setWasInMilitaryService(bool newData){m_wasInMilitaryService = newData;}

QList<FamilyTree*>& FamilyTree::getChilds(){ return m_childs; }


