#include "mainwindow.h"
#include "ui_mainwindow.h"

#define MAX_FIO_LENGTH 45

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    messageBox.setFixedSize(500,200);

    QRegularExpression rx("^[A-ЯЁ][а-яё]+ [A-ЯЁ][а-яё]+ [A-ЯЁ][а-яё]+$");
    fioValidator = new QRegularExpressionValidator(rx, this);
    ui->FioEdit->setValidator(fioValidator);
    ui->FioEdit->setMaxLength(MAX_FIO_LENGTH);

    connect(ui->pushButtonCreate, SIGNAL(clicked()), this, SLOT(create_record()));
    connect(ui->pushButtonSave, SIGNAL(clicked()), this, SLOT(save_record()));
    connect(ui->pushButtonDelete, SIGNAL(clicked()), this, SLOT(delete_record()));
    connect(ui->recordsListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(show_record(int)));
    connect(ui->checkIsAlive, SIGNAL(clicked()), this, SLOT(disableDeathDate()));

    ui->checkIsAlive->setChecked(true);
    updateButtonsActivity();
    disableDeathDate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save_record()
{
    int previousIndex = ui->recordsListWidget->currentRow();
    FamilyTree curRec;
    curRec.setFIO(ui->FioEdit->text());
    curRec.setDateOfBirth(ui->dateEditBirthday->date());
    int newPos = searchInsertPos(curRec);
    records.move(previousIndex, newPos);
    if(_save(newPos))
    {
        ui->recordsListWidget->takeItem(previousIndex);
        addRecInListWidget(newPos);

        ui->recordsListWidget->setCurrentRow(newPos);
    }
    else
        records.move(newPos, previousIndex);
}

void MainWindow::show_record(int i)
{
    if(i < records.size() && i >= 0)
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

void MainWindow::create_record()
{
   FamilyTree curRec;
   curRec.setFIO(ui->FioEdit->text());
   curRec.setDateOfBirth(ui->dateEditBirthday->date());
   int insertPos = searchInsertPos(curRec);
   //Вставка новой записи в список
   records.insert(insertPos, curRec);

   if(_save(insertPos, true))
   {
       addRecInListWidget(insertPos);
       ui->recordsListWidget->setCurrentRow(insertPos);
   }
   else
       records.removeAt(insertPos);


   updateButtonsActivity();
}

void MainWindow::delete_record()
{
    int deleteIndex = ui->recordsListWidget->currentRow();

    for(int i = 0; i < records[deleteIndex].getChilds().size(); i++)
    {
        if(records[deleteIndex].getGender() == MALE)
            records[deleteIndex].getChilds()[i]->setFatherName("none");
        else
            records[deleteIndex].getChilds()[i]->setMatherName("none");
    }
    if(records[deleteIndex].getGender() == MALE)
        ui->comboBoxFatherFio->removeItem(ui->comboBoxFatherFio->findText(records[deleteIndex].getFIO()));
    else
        ui->comboBoxMatherFio->removeItem(ui->comboBoxMatherFio->findText(records[deleteIndex].getFIO()));

    records.removeAt(deleteIndex);
    ui->recordsListWidget->takeItem(deleteIndex);
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
    //Запись уже существует - изменить ФИО/Пол
    else if (!(getRecordsByFio(tmpRec.getFIO())) || !(records[recordIndex].getGender() == tmpRec.getGender()))
    {
        QString newFio = tmpRec.getFIO();
        int fioIndex;

        if (records[recordIndex].getGender() == tmpRec.getGender()) //Изменилось ФИО
        {

            if(records[recordIndex].getGender() == Gender::MALE)
            {
                for(int i = 0; i < records[recordIndex].getChilds().size(); i ++)
                    records[recordIndex].getChilds()[i]->setFatherName(newFio);
                fioIndex = ui->comboBoxFatherFio->findText(records[recordIndex].getFIO());
                ui->comboBoxFatherFio->setItemText(fioIndex, newFio);
            }
            else
            {
                for(int i = 0; i < records[recordIndex].getChilds().size(); i ++)
                    records[recordIndex].getChilds()[i]->setMatherName(newFio);
                fioIndex = ui->comboBoxMatherFio->findText(records[recordIndex].getFIO());
                ui->comboBoxMatherFio->setItemText(fioIndex, newFio);
            }
        }
        else
        {
            if(records[recordIndex].getGender() == Gender::MALE) //Поменялся пол
            {
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
        }
    }

    //Сохранение составного поля родителей
    QString parentName;
    if(ui->comboBoxFatherFio->currentIndex()) //Был выбран отец из списка
    {
        parentName = tmpRec.getParents().fatherName;
        records[recordIndex].setFatherName(parentName);

        //Сохранение этой записи как ребёнка у другой
        bool newChild = true;
        FamilyTree* parent = getRecordsByFio(parentName);
        for (int i = 0 ; i < parent->getChilds().size(); i++)
            newChild *= (&records[recordIndex] != parent->getChilds().at(i));
        if (newChild) parent->getChilds().push_back(&records[recordIndex]);
    }
    if(ui->comboBoxMatherFio->currentIndex()) //Была выбрана мать из списка
    {
        parentName = tmpRec.getParents().matherName;
        records[recordIndex].setMatherName(parentName);

        //Сохранение этой записи как ребёнка у другой
        bool newChild = true;
        FamilyTree* parent = getRecordsByFio(parentName);
        for (int i = 0 ; i < parent->getChilds().size(); i++)
            newChild *= (&records[recordIndex] != parent->getChilds().at(i));
        if (newChild) parent->getChilds().push_back(&records[recordIndex]);
    }

    //Сохранение записи
    records[recordIndex] = tmpRec;
    return true;
}

int MainWindow::searchInsertPos(const FamilyTree &curRec)
{
    int insertPos = 0;
    while (insertPos < records.size()) {
        if(curRec <= records[insertPos])
            break;
        insertPos++;
    }
    return insertPos;
}

void MainWindow::disableDeathDate()
{
    ui->dateEditDeathday->setEnabled(!ui->checkIsAlive->isChecked());
}

FamilyTree* MainWindow::getRecordsByFio(const QString& fio)
{
    for(int i = 0; i < records.size(); i++ )
        if(records[i].getFIO() == fio)
            return &records[i];
    return nullptr;
}

bool MainWindow::correctRecordCheck(int recordIndex, const FamilyTree &tmpRec, bool newRecord)
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

void MainWindow::updateButtonsActivity()
{
    ui->pushButtonSave->setEnabled(records.size() >= 1);
    ui->pushButtonDelete->setEnabled(records.size() >= 1);
}

void MainWindow::addRecInListWidget(int insertPos)
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
