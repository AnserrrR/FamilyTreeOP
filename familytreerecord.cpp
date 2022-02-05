#include "familytreerecord.h"

FamilyTreeRecord::FamilyTreeRecord() //Конструктор по умолчанию
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

FamilyTreeRecord::~FamilyTreeRecord()
{
}

bool FamilyTreeRecord::operator<(const FamilyTreeRecord &other) const //Перегрузка оператора меньше
{
    if(m_DateOfBirth < other.m_DateOfBirth) //Сравнение дат рождения
        return true;
    if(m_DateOfBirth == other.m_DateOfBirth)
        if(m_FIO < other.m_FIO) //Сравнение фио, если даты рождения одинаковы
            return true;
    return false;
}

bool FamilyTreeRecord::operator>(const FamilyTreeRecord &other) const //Перегрузка оператора больше
{
    if(m_DateOfBirth > other.m_DateOfBirth) //Сравнение дат рождения
            return true;
    if(m_DateOfBirth == other.m_DateOfBirth)
        if(m_FIO > other.m_FIO) //Сравнение фио, если даты рождения одинаковы
            return true;
    return false;
}

void FamilyTreeRecord::operator=(const FamilyTreeRecord &other) //Перегрузка оператора присвоения
{
    m_FIO = other.m_FIO;
    m_DateOfBirth = other.m_DateOfBirth;
    m_isAlive = other.m_isAlive;
    m_DateOfDeath = other.m_DateOfDeath;
    m_gender = other.m_gender;
    m_citizenship = other.m_citizenship;
    m_wasInMilitaryService = other.m_wasInMilitaryService;
    m_id = other.m_id;
    m_parents.fatherName = other.m_parents.fatherName;
    m_parents.matherName = other.m_parents.matherName;
    m_childs = other.m_childs;
}

QDataStream& operator<<(QDataStream& out, const FamilyTreeRecord& record) //Перегрузка оператора вывода в поток
{
    out << record.m_FIO << record.m_parents.fatherName << record.m_parents.matherName
        << record.m_DateOfBirth << record.m_isAlive << record.m_DateOfDeath
        << record.m_wasInMilitaryService << record.m_id << record.m_childs;
    int tmp = static_cast<int>(record.m_citizenship);
    out << tmp;
    tmp = static_cast<int>(record.m_gender);
    out << tmp;
    return out;
}

QDataStream& operator>>(QDataStream& in, FamilyTreeRecord& record) //Перегрузка оператора ввода в поток
{
    in >> record.m_FIO >> record.m_parents.fatherName >> record.m_parents.matherName
       >> record.m_DateOfBirth >> record.m_isAlive >> record.m_DateOfDeath
       >> record.m_wasInMilitaryService >> record.m_id >> record.m_childs;
    int tmp;
    in >> tmp;
    record.m_citizenship = static_cast<Country>(tmp);
    in >> tmp;
    record.m_gender = static_cast<Gender>(tmp);
    return in;
}

//Определение гетеров
const QString& FamilyTreeRecord::getFIO()const{ return m_FIO;}
const Parents& FamilyTreeRecord::getParents()const{return m_parents;}
const QDate& FamilyTreeRecord::getDateOfBirth()const{return m_DateOfBirth;}
const bool& FamilyTreeRecord::isAlive()const{return m_isAlive;}
const QDate& FamilyTreeRecord::getDateOfDeath()const{return m_DateOfDeath;}
const Gender& FamilyTreeRecord::getGender()const{return m_gender;}
const Country& FamilyTreeRecord::getCitizenship()const{return m_citizenship;}
const bool& FamilyTreeRecord::WasInMilitaryService()const{return m_wasInMilitaryService;}
const unsigned int FamilyTreeRecord::getID() const { return m_id;}
QList<unsigned int>& FamilyTreeRecord::getChilds(){ return m_childs;}
//Определение сэттеров
void FamilyTreeRecord::setFIO(QString newData){ m_FIO = newData;}
void FamilyTreeRecord::setFatherName(QString newData){m_parents.fatherName = newData;}
void FamilyTreeRecord::setMatherName(QString newData){m_parents.matherName = newData;}
void FamilyTreeRecord::setDateOfBirth(QDate newData){m_DateOfBirth = newData;}
void FamilyTreeRecord::setIsAlive(bool newData){m_isAlive = newData;}
void FamilyTreeRecord::setDateOfDeath(QDate newData){m_DateOfDeath = newData;}
void FamilyTreeRecord::setGender(Gender newData){m_gender = newData;}
void FamilyTreeRecord::setID(unsigned int id) {m_id = id;}
void FamilyTreeRecord::setCitizenship(Country newData){m_citizenship = newData;}
void FamilyTreeRecord::setWasInMilitaryService(bool newData){m_wasInMilitaryService = newData;}
