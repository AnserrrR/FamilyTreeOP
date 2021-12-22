#include "familytreedb.h"

bool FamilyTreeDB::modifiedFlag;

FamilyTreeDB::FamilyTreeDB()
{

}

int FamilyTreeDB::count() const //Узнать кол-во записей в базе
{
    return DB.size();
}

int FamilyTreeDB::append(FamilyTreeRecord &record) //Добавить запись, вернуть позицию в браузере
{
    unsigned int recordID = idCounter++;
    record.setID(recordID);
    DB.insert(recordID, record);

    if(record.getParents().fatherName != "не задан") //Была выбрана запись
    {
        QString parentName;
        parentName = record.getParents().fatherName;
        FamilyTreeRecord& parent = DB[getRecordByFio(parentName)];

        //Сохранение этой записи как ребёнка у другой
        parent.getChilds().push_back(recordID);
    }
    if(record.getParents().matherName != "не задана") //Была выбрана запись
    {
        QString parentName;
        parentName = record.getParents().matherName;
        FamilyTreeRecord& parent = DB[getRecordByFio(parentName)];

        //Сохранение этой записи как ребёнка у другой
        parent.getChilds().push_back(recordID);
    }
    if(DB[recordID].getParents().matherName == "не задана")
        DB[recordID].setMatherName("none");
    if(DB[recordID].getParents().fatherName == "не задан")
        DB[recordID].setFatherName("none");

    int insertPos = 0;
    QHash<unsigned int, FamilyTreeRecord>::const_iterator i;
    for(i = DB.constBegin(); i != DB.constEnd(); i++)
        if(record > i.value())
            insertPos++;

    modifiedFlag = true;
    return insertPos;
}

void FamilyTreeDB::remove(unsigned int id) //Удалить запись
{
    //Удалить имя записи из записей детей
    for(QList<unsigned int>::iterator i = DB[id].getChilds().begin(); i != DB[id].getChilds().end(); i++)
    {
        if(DB[*i].getGender() == MALE)
            DB[*i].setFatherName("none");
        else
            DB[*i].setMatherName("none");

    }
    DB.remove(id);
    modifiedFlag = true;
}

int FamilyTreeDB::update(const FamilyTreeRecord &record) //Обновить запись, вернуть новую позицию
{
    unsigned int recordID = record.getID();

    if(record.getGender() != DB[recordID].getGender())
    {
        //Удалить имя записи из записей детей
        for(QList<unsigned int>::iterator i = DB[recordID].getChilds().begin(); i != DB[recordID].getChilds().end(); i++)
        {
            if(DB[recordID].getGender() == MALE)
                DB[*i].setFatherName("none");
            else
                DB[*i].setMatherName("none");
        }
        DB[recordID].getChilds().clear();
    }
    else if(record.getFIO() != DB[recordID].getFIO())
    {
        for(QList<unsigned int>::iterator i = DB[recordID].getChilds().begin(); i != DB[recordID].getChilds().end(); i++)
        {
            if(DB[recordID].getGender() == MALE)
                DB[*i].setFatherName(record.getFIO());
            else
                DB[*i].setMatherName(record.getFIO());
        }
    }

    //Сохранение составного поля родителей, если оно измененно
    if(record.getParents().fatherName != DB[recordID].getParents().fatherName)
    {
        QString parentName;

        if(DB[recordID].getParents().fatherName != "none") //Если до этого был выбран другой отец удалить запись из его списка детей
        {
            parentName = DB[recordID].getParents().fatherName;
            unsigned int parentID = getRecordByFio(parentName);
            DB[parentID].getChilds().removeAll(recordID);
        }
        if(record.getParents().fatherName != "не задан") //Была выбрана запись
        {
            parentName = record.getParents().fatherName;
            FamilyTreeRecord& parent = DB[getRecordByFio(parentName)];

            //Сохранение этой записи как ребёнка у другой
            if (!parent.getChilds().contains(recordID)) parent.getChilds().push_back(recordID);
        }
    }
    if(record.getParents().matherName != DB[recordID].getParents().matherName)
    {
        QString parentName;

        if(DB[recordID].getParents().matherName != "none") //Если до этого была выбрана другая мать удалить запись из её списка детей
        {
            parentName = DB[recordID].getParents().matherName;
            unsigned int parentID = getRecordByFio(parentName);
            DB[parentID].getChilds().removeAll(recordID);
        }
        if(record.getParents().matherName != "не задана") //Была выбрана запись
        {
            parentName = record.getParents().matherName;
            FamilyTreeRecord& parent = DB[getRecordByFio(parentName)];

            //Сохранение этой записи как ребёнка у другой
            if (!parent.getChilds().contains(recordID)) parent.getChilds().push_back(recordID);
        }
    }

    DB[recordID] = record;
    if(DB[recordID].getParents().matherName == "не задана")
        DB[recordID].setMatherName("none");
    if(DB[recordID].getParents().fatherName == "не задан")
        DB[recordID].setFatherName("none");

    int insertPos = 0;
    QHash<unsigned int, FamilyTreeRecord>::const_iterator i;
    for(i = DB.constBegin(); i != DB.constEnd(); i++)
        if(record > i.value())
            insertPos++;

    modifiedFlag = true;
    return insertPos;
}

void FamilyTreeDB::record(unsigned int id, FamilyTreeRecord &record) const //Получить запись по айди
{
    record = DB.value(id);
}

const QVector<BrowserField> FamilyTreeDB::records() const //Получить список записей для браузера
{
    QVector<BrowserField> fields;

    QHash<unsigned int, FamilyTreeRecord>::const_iterator i;
    for(i = DB.constBegin(); i != DB.constEnd(); i++)
    {
        BrowserField field;
        field.fio = i->getFIO();
        field.dateOfBirth = i->getDateOfBirth();
        field.dateOfDeath = i->getDateOfDeath();
        field.parents = i->getParents();
        field.id = i.key();

        QVector<BrowserField>::iterator j;
        if(fields.size() == 0)
            fields.push_back(field);
        for(j = fields.begin(); j != fields.end(); j++)
        {
            if(field.dateOfBirth < j->dateOfBirth)
            {
                j = fields.insert(j, field);
                break;
            }
            else if(field.dateOfBirth == j->dateOfBirth && field.fio < j->fio)
            {
                j = fields.insert(j, field);
                break;
            }
            else if(j == fields.end() - 1)
                fields.push_back(field);
        }
    }
    return fields;
}

bool FamilyTreeDB::save(QString filename) const //Сохранить базу в файл
{
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if(file.open(QIODevice::WriteOnly))
        {
            QDataStream out(&file);
            out << DB << idCounter;
            modifiedFlag = false;
            return true;
        }
    }
    return false;
}

bool FamilyTreeDB::load(QString filename) //Загрузить базу из файла
{
    DB.clear();
    modifiedFlag = false;
    if(!filename.isEmpty())
    {
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly))
        {
            QDataStream in(&file);
            in >> DB >> idCounter;
            return true;
        }
    }
    return false;
}

void FamilyTreeDB::clear() //Очистить базу
{
    DB.clear();
    idCounter = 1;
}

bool FamilyTreeDB::isModified() //Обновлялась ли база
{
    return modifiedFlag;
}

const QString FamilyTreeDB::recordVerification(const FamilyTreeRecord &verifiableRecord, bool newRecord)
{
    unsigned int recordID = 0;
    if(!newRecord)
        recordID = verifiableRecord.getID();
    //Проверка на уникальность имени
    for(QHash<unsigned int, FamilyTreeRecord>::const_iterator i = DB.constBegin(); i != DB.constEnd(); i++)
    {
        if(i.value().getID() == recordID) continue;
        if(i.value().getFIO() == verifiableRecord.getFIO())
            return QString("ФИО должно быть уникальным! ");
    }

    if(!newRecord)
    {
        //Запись не может быть себе отцом или сыном
        if(DB[recordID].getFIO() == verifiableRecord.getParents().matherName ||  verifiableRecord.getFIO() == verifiableRecord.getParents().matherName)
            return QString("Неправильно заданно поле матери! ");
        if(DB[recordID].getFIO() == verifiableRecord.getParents().fatherName ||  verifiableRecord.getFIO() == verifiableRecord.getParents().fatherName)
            return QString("Неправильно заданно поле отца! ");
    }

    //Проверка на правильную дату рождения
    if(verifiableRecord.getDateOfBirth()>QDate::currentDate())
        return QString("Некоректная дата рождения! ");
    //Проверка на корректную разницу с датой рождения всех детей
    QDate testDate = verifiableRecord.getDateOfBirth().addYears(-15);
    if(!newRecord)
        for (QList<unsigned int>::iterator i = DB[recordID].getChilds().begin(); i != DB[recordID].getChilds().end(); i++)
            if(DB[*i].getDateOfBirth() < testDate)
                return QString("Некоректная дата рождения! ");

    //Проверка на корректную разницу с датой рождения родителей
    if(getRecordByFio(verifiableRecord.getParents().fatherName))
    {
        unsigned int tmpID = getRecordByFio(verifiableRecord.getParents().fatherName);
        if(DB[tmpID].getDateOfBirth() > testDate)
            return QString("Некоректная дата рождения! ");
    }
    if(getRecordByFio(verifiableRecord.getParents().matherName))
    {
        unsigned int tmpID = getRecordByFio(verifiableRecord.getParents().matherName);
        if(DB[tmpID].getDateOfBirth() > testDate)
            return QString("Некоректная дата рождения! ");
    }

    //Проверка на правильную дату смерти
    if(!(verifiableRecord.isAlive()) && (verifiableRecord.getDateOfDeath()>QDate::currentDate() || verifiableRecord.getDateOfDeath() < verifiableRecord.getDateOfBirth()))
        return QString("Некоректная дата смерти! ");

    return  QString();
}

unsigned int FamilyTreeDB::getRecordByFio(const QString &fio) const
{
    for(QHash<unsigned int, FamilyTreeRecord>::const_iterator i = DB.constBegin(); i != DB.constEnd(); i++)
        if(i.value().getFIO() == fio)
            return i.key();
    return 0;
}
