#include "mainwindow.h"
#include "ui_mainwindow.h"

#define MAX_FIO_LENGTH 45

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    messageBox.setFixedSize(500,200);

    //Валидатор для ФИО
    QRegularExpression rx("^[A-ЯЁ][а-яё]+ [A-ЯЁ][а-яё]+ [A-ЯЁ][а-яё]+$");
    fioValidator = new QRegularExpressionValidator(rx, this);
    ui->FioEdit->setValidator(fioValidator);
    ui->FioEdit->setMaxLength(MAX_FIO_LENGTH);

    //Подсоединение сигналов
    connect(ui->pushButtonCreate, SIGNAL(clicked()), this, SLOT(create_record()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(save_record()));
    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(delete_record()));
    connect(ui->recordsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(show_record(int)));
    connect(ui->checkIsAlive, SIGNAL(clicked()), this, SLOT(disableDeathDate()));

    //Перевод в стандартное состояние видимости
    ui->checkIsAlive->setChecked(true);
    updateButtonsActivity();
    disableDeathDate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save_record() //Изменение существующей записи
{
    int previousIndex = ui->recordsListWidget->currentRow(); //Индекс строки где находилась измениемая запись

    //Поиск новой позиции
    FamilyTree curRec;
    curRec.setFIO(ui->FioEdit->text());
    curRec.setDateOfBirth(ui->dateEditBirthday->date());
    int newPos = searchInsertPos(curRec);

    if(newPos - previousIndex == 1)//Расположение записи не изменилось
        newPos = previousIndex;

    records.move(previousIndex, newPos); //Изменение расположения записи в списке

    if(_save(newPos)) //Если удалось сохранить, переместить на новую позицию в listWidget
    {
        ui->recordsListWidget->takeItem(previousIndex);
        addRecInListWidget(newPos);

        ui->recordsListWidget->setCurrentRow(newPos);
    }
    else
        records.move(newPos, previousIndex); //Если не удалось сохранить, откатить измение
}

void MainWindow::show_record(int i) //Отображение выбранной записи
{
    if(i < records.size() && i >= 0) //Выбрана корректная строка
    {
        int parentIndex = ui->comboBoxFatherFio->findText(records[i].getParents().fatherName);
        if (parentIndex > 0) ui->comboBoxFatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxFatherFio->setCurrentIndex(0);
        parentIndex = ui->comboBoxMatherFio->findText(records[i].getParents().matherName);
        if (parentIndex > 0) ui->comboBoxMatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxMatherFio->setCurrentIndex(0);

        ui->FioEdit->setText(records[i].getFIO());
        ui->dateEditBirthday->setDate(records[i].getDateOfBirth());
        ui->checkIsAlive->setChecked(records[i].isAlive());
        disableDeathDate();
        ui->dateEditDeathday->setDate(records[i].getDateOfDeath());
        ui->comboBoxGender->setCurrentIndex(static_cast<int>(records[i].getGender()));
        ui->comboBoxCitizenship->setCurrentIndex(static_cast<int>(records[i].getCitizenship()));
        ui->checkSlughil->setChecked(records[i].WasInMilitaryService());
    }
}

void MainWindow::create_record() //Создание новой записи
{
   FamilyTree curRec; //Временный объект для поиска расположения
   curRec.setFIO(ui->FioEdit->text());
   curRec.setDateOfBirth(ui->dateEditBirthday->date());
   int insertPos = searchInsertPos(curRec); //Позиция вставки

   //Вставка новой записи в список
   records.insert(insertPos, curRec);

   if(_save(insertPos, true)) //Если удачное сохранение записи, то добавить в listWidget
   {
       addRecInListWidget(insertPos);
       ui->recordsListWidget->setCurrentRow(insertPos);
   }
   else //Если сохранение не удалось, то освободить выделенную запись в списке
       records.removeAt(insertPos);


   updateButtonsActivity();
}

void MainWindow::delete_record() //Удаление записи
{
    int deleteIndex = ui->recordsListWidget->currentRow();

    //Удалить имя записи из записей детей
    for(int i = 0; i < records[deleteIndex].getChilds().size(); i++)
    {
        int childIndex = getRecordIndexByFio(records[deleteIndex].getChilds()[i]->getFIO());
        if(records[deleteIndex].getGender() == MALE)
            records[childIndex].setFatherName("none");
        else
            records[childIndex].setMatherName("none");

        //Убрать имя родителей из listWidget
        ui->recordsListWidget->takeItem(childIndex);
        addRecInListWidget(childIndex);
    }

    //Убрать из списка родителей
    if(records[deleteIndex].getGender() == MALE)
        ui->comboBoxFatherFio->removeItem(ui->comboBoxFatherFio->findText(records[deleteIndex].getFIO()));
    else
        ui->comboBoxMatherFio->removeItem(ui->comboBoxMatherFio->findText(records[deleteIndex].getFIO()));

    //Удалить из списка и listWidget'а
    records.removeAt(deleteIndex);
    ui->recordsListWidget->takeItem(deleteIndex);

    if(records.size() > deleteIndex)
        ui->recordsListWidget->currentRowChanged(deleteIndex);

    updateButtonsActivity();
}

bool MainWindow::_save(int recordIndex, bool newRecord)
{
    //Текущее состояние полей
    FamilyTree tmpRec;
    tmpRec.setFIO(ui->FioEdit->text());
    tmpRec.setFatherName(ui->comboBoxFatherFio->currentText());
    tmpRec.setMatherName(ui->comboBoxMatherFio->currentText());
    tmpRec.setDateOfBirth(ui->dateEditBirthday->date());
    tmpRec.setIsAlive(ui->checkIsAlive->isChecked());
    tmpRec.setDateOfDeath(ui->dateEditDeathday->date());
    tmpRec.setGender(static_cast<Gender>(ui->comboBoxGender->currentIndex()));
    tmpRec.setCitizenship(static_cast<Country>(ui->comboBoxCitizenship->currentIndex()));
    tmpRec.setWasInMilitaryService(ui->checkSlughil->isChecked());

    //Проверка записи
    if(!correctRecordCheck(recordIndex, tmpRec, newRecord))
        return false;

    //Добавление новой записи в составное поле
    if(newRecord)
    {
        if(tmpRec.getGender() == Gender::MALE)
            ui->comboBoxFatherFio->addItem(tmpRec.getFIO());
        else
            ui->comboBoxMatherFio->addItem(tmpRec.getFIO());

    }
    //Запись уже существует, поменялся ФИО или Пол
    else if (!(getRecordsByFio(tmpRec.getFIO())) || !(records[recordIndex].getGender() == tmpRec.getGender()))
    {
        QString newFio = tmpRec.getFIO();
        int fioIndex;

        if (records[recordIndex].getGender() == tmpRec.getGender()) //Пол не изменился
        {
            //Изменить ФИО списке родителей
            if(records[recordIndex].getGender() == Gender::MALE)
            {
                fioIndex = ui->comboBoxFatherFio->findText(records[recordIndex].getFIO());
                ui->comboBoxFatherFio->setItemText(fioIndex, newFio);
            }
            else
            {
                fioIndex = ui->comboBoxMatherFio->findText(records[recordIndex].getFIO());
                ui->comboBoxMatherFio->setItemText(fioIndex, newFio);
            }
            //Изменить ФИО у детей
            for(int i = 0; i < records[recordIndex].getChilds().size(); i ++)
            {
                if(records[recordIndex].getGender() == MALE)
                    records[recordIndex].getChilds()[i]->setFatherName(newFio);
                else
                    records[recordIndex].getChilds()[i]->setMatherName(newFio);

               int childIndex = getRecordIndexByFio(records[recordIndex].getChilds()[i]->getFIO());
               ui->recordsListWidget->takeItem(childIndex);
               addRecInListWidget(childIndex);
            }
        }
        else //Поменялся пол
        {
            if(records[recordIndex].getGender() == Gender::MALE)
            {
                //Переместить в другой комбобокс
                fioIndex = ui->comboBoxFatherFio->findText(records[recordIndex].getFIO());
                ui->comboBoxFatherFio->removeItem(fioIndex);
                ui->comboBoxMatherFio->addItem(newFio);
            }
            else
            {
                fioIndex = ui->comboBoxMatherFio->findText(records[recordIndex].getFIO());
                ui->comboBoxMatherFio->removeItem(fioIndex);
                ui->comboBoxFatherFio->addItem(newFio);
            }            
            //Удалить имя записи из записей детей
            for(int i = 0; i < records[recordIndex].getChilds().size(); i++)
            {
                int childIndex = getRecordIndexByFio(records[recordIndex].getChilds()[i]->getFIO());
                if(records[recordIndex].getGender() == MALE)
                    records[childIndex].setFatherName("none");
                else
                    records[childIndex].setMatherName("none");

                //Убрать имя родителя из listWidget
                ui->recordsListWidget->takeItem(childIndex);
                addRecInListWidget(childIndex);
            }
            records[recordIndex].getChilds().clear();
        }
    }

    //Сохранение составного поля родителей, если оно измененно
    if(tmpRec.getParents().fatherName != records[recordIndex].getParents().fatherName)
    {

        QString parentName;


        if(records[recordIndex].getParents().fatherName != "none") //Если до этого был выбран другой отец удалить запись из его списка детей
        {
            parentName = records[recordIndex].getParents().fatherName;
            FamilyTree* parent = getRecordsByFio(parentName);
            for(int i = 0; i < parent->getChilds().size(); i++)
                if(parent->getChilds().at(i)->getFIO() == tmpRec.getFIO())
                    parent->getChilds().removeAt(i);
        }
        if(ui->comboBoxFatherFio->currentIndex()) //Была выбрана запись
        {
            parentName = tmpRec.getParents().fatherName;
            records[recordIndex].setFatherName(parentName);
            FamilyTree* parent = getRecordsByFio(parentName);

            //Сохранение этой записи как ребёнка у другой
            bool newChild = true;
            for (int i = 0 ; i < parent->getChilds().size(); i++)
                newChild *= (&records[recordIndex] != parent->getChilds().at(i));
            if (newChild) parent->getChilds().push_back(&records[recordIndex]);
        }
        else
            records[recordIndex].setFatherName("none");
    }
    if(tmpRec.getParents().matherName != records[recordIndex].getParents().matherName)
    {
        QString parentName;

        if(records[recordIndex].getParents().matherName != "none") //Если до этого был выбрана другая мать удалить запись из её списка детей
        {
            parentName = tmpRec.getParents().matherName;
            FamilyTree* parent = getRecordsByFio(parentName);
            for(int i = 0; i < parent->getChilds().size(); i++)
                if(parent->getChilds().at(i)->getFIO() == tmpRec.getFIO())
                    parent->getChilds().removeAt(i);
        }
        if(ui->comboBoxMatherFio->currentIndex()) //Была выбрана запись
        {
            parentName = tmpRec.getParents().matherName;
            records[recordIndex].setMatherName(parentName);
            FamilyTree* parent = getRecordsByFio(parentName);

            //Сохранение этой записи как ребёнка у другой
            bool newChild = true;
            for (int i = 0 ; i < parent->getChilds().size(); i++)
                newChild *= (&records[recordIndex] != parent->getChilds().at(i));
            if (newChild) parent->getChilds().push_back(&records[recordIndex]);
        }
        else
            records[recordIndex].setMatherName("none");
    }

    //Сохранение записи
    records[recordIndex] = tmpRec;
    return true;
}

int MainWindow::searchInsertPos(const FamilyTree &curRec) //Поиск позиции в списке для вставки записи
{
    int insertPos = 0;
    while (insertPos < records.size()) {
        if(curRec < records[insertPos])
            break;
        insertPos++;
    }

    return insertPos;
}

void MainWindow::disableDeathDate() //Обновление видимости
{
    ui->dateEditDeathday->setEnabled(!ui->checkIsAlive->isChecked());
}

FamilyTree* MainWindow::getRecordsByFio(const QString& fio) //Получить указатель на запись по фио
{
    for(int i = 0; i < records.size(); i++ )
        if(records[i].getFIO() == fio)
            return &records[i];
    return nullptr;
}

int MainWindow::getRecordIndexByFio(const QString& fio) //Получить индекс записи в списке по фио
{
    for(int i = 0; i < records.size(); i++ )
        if(records[i].getFIO() == fio)
            return i;
    return -1;
}

bool MainWindow::correctRecordCheck(int recordIndex, const FamilyTree &tmpRec, bool newRecord) //Провека записи
{
    if(!ui->FioEdit->hasAcceptableInput())
    {
        messageBox.critical(0,"Error","ФИО неполное! ");
        return false;
    }

    //Проверка на уникальность имени
    for(int iii = 0; iii < records.size(); iii++)
    {
        if(iii == recordIndex) continue;
        if(records.at(iii).getFIO() == tmpRec.getFIO())
        {
            messageBox.critical(0,"Error","ФИО должно быть уникальным! ");
            return false;
        }
    }

    if(!newRecord)
    {
        //Запись не может быть себе отцом или сыном
        if(records.at(recordIndex).getFIO() == tmpRec.getParents().matherName ||  tmpRec.getFIO() == tmpRec.getParents().matherName)
        {
            messageBox.critical(0,"Error","Неправильно заданно поле матери! ");
            return false;
        }
        if(records.at(recordIndex).getFIO() == tmpRec.getParents().fatherName ||  tmpRec.getFIO() == tmpRec.getParents().fatherName)
        {
            messageBox.critical(0,"Error","Неправильно заданно поле отца! ");
            return false;
        }
    }

    //Проверка на правильную дату рождения
    if(tmpRec.getDateOfBirth()>QDate::currentDate())
    {
        messageBox.critical(0,"Error","Некоректная дата рождения! ");
        return false;
    }
    //Проверка на корректную разницу с датой рождения всех детей
    QDate testDate = tmpRec.getDateOfBirth().addYears(-15);
    if(!newRecord)
    {
        for (int i = 0; i < records[recordIndex].getChilds().size(); i++)
        {
            if(records[recordIndex].getChilds()[i]->getDateOfBirth() < testDate)
            {
                //qDebug() << records[i].getChilds()[i]->getFIO();
                messageBox.critical(0,"Error","Некоректная дата рождения! ");
                return false;
            }
        }
    }
    //Проверка на корректную разницу с датой рождения родителей
    if(getRecordsByFio(tmpRec.getParents().fatherName) != nullptr)
    {
        if(getRecordsByFio(tmpRec.getParents().fatherName)->getDateOfBirth() > testDate)
        {
            messageBox.critical(0,"Error","Некоректная дата рождения! ");
            return false;
        }
    }
    if(getRecordsByFio(tmpRec.getParents().matherName) != nullptr)
    {
        if(getRecordsByFio(tmpRec.getParents().matherName)->getDateOfBirth() > testDate)
        {
            messageBox.critical(0,"Error","Некоректная дата рождения! ");
            return false;
        }
    }

    //Проверка на правильную дату смерти
    if(!(tmpRec.isAlive()) && (tmpRec.getDateOfDeath()>QDate::currentDate() || tmpRec.getDateOfDeath() < tmpRec.getDateOfBirth()))
    {
        messageBox.critical(0,"Error","Некоректная дата смерти! ");
        return false;
    }

    return  true;
}

void MainWindow::updateButtonsActivity() //Обновление состояние активности кнопок удаление и создания
{
    ui->pushButtonSave->setEnabled(records.size() >= 1);
    ui->pushButtonDelete->setEnabled(records.size() >= 1);
}

void MainWindow::addRecInListWidget(int insertPos) //Добовляет запись в listWidget
{
    QString rec;
    rec.append(records[insertPos].getFIO());
    rec.append('\t');
    rec.append(records[insertPos].getDateOfBirth().toString("dd.MM.yyyy"));
    rec.append('\t');
    if(records[insertPos].isAlive())
    {
        rec.append(QString("--.--.----"));
        rec.append('\t');
    }
    else
    {
        rec.append(records[insertPos].getDateOfDeath().toString("dd.MM.yyyy"));
        rec.append('\t');
    }
    if(records[insertPos].getParents().fatherName != "none")
        rec.append(records[insertPos].getParents().fatherName);
    if(records[insertPos].getParents().fatherName != "none" && records[insertPos].getParents().matherName != "none")
        rec.append(", ");
    if(records[insertPos].getParents().matherName != "none")
        rec.append(records[insertPos].getParents().matherName);
    ui->recordsListWidget->insertItem(insertPos, rec);

}
