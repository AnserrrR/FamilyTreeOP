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

//    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(save_record_1()));
//    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(save_record_2()));
//    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(show_record_1()));
//    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(show_record_2()));
//    connect(ui->checkBox, SIGNAL(clicked()), this, SLOT(disableDeathDate()));
    ui->checkIsAlive->setChecked(true);
    disableDeathDate();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save_record()
{
    int i = 0;
    _save(i);
}

void show_record()
{

}

void create_record()
{

}

void delete_record()
{

}

void MainWindow::_save(int i)
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


    if(!ui->FioEdit->hasAcceptableInput())
        {
            messageBox.critical(0,"Error","ФИО неполное! ");
            return;
        }

        //Проверка на уникальность имени
        for(int iii = 0; iii < records.size(); iii++)
        {
            if(iii == i) continue;
            if(records[iii].getFIO() == tmpRec.getFIO())
            {
                messageBox.critical(0,"Error","ФИО должно быть уникальным! ");
                return;
            }
        }
        //Запись не может быть себе отцом или сыном
        if(records[i].getFIO() == tmpRec.getParents().matherName ||  tmpRec.getFIO() == tmpRec.getParents().matherName)
        {
            messageBox.critical(0,"Error","Неправильно заданно поле матери! ");
            return;
        }
        if(records[i].getFIO() == tmpRec.getParents().fatherName ||  tmpRec.getFIO() == tmpRec.getParents().fatherName)
        {
            messageBox.critical(0,"Error","Неправильно заданно поле отца! ");
            return;
        }
        //Проверка на правильную дату рождения
        QDate testDate = tmpRec.getDateOfBirth().addYears(-15);
        if(tmpRec.getDateOfBirth()>QDate::currentDate())
        {
            messageBox.critical(0,"Error","Некоректная дата рождения! ");
            return;
        }
        for (int i = 0; i < records[i].getChilds().size(); i++)
        {
            if(records[i].getChilds()[i]->getDateOfBirth() < testDate)
            {
                qDebug() << records[i].getChilds()[i]->getFIO();
                messageBox.critical(0,"Error","Некоректная дата рождения! ");
                return;
            }
        }
        if(this->getRecordsByFio(tmpRec.getParents().fatherName) != nullptr)
        {
            if(this->getRecordsByFio(tmpRec.getParents().fatherName)->getDateOfBirth() > testDate)
            {
                messageBox.critical(0,"Error","Некоректная дата рождения! ");
                return;
            }
        }
        if(this->getRecordsByFio(tmpRec.getParents().matherName) != nullptr)
        {
            if(this->getRecordsByFio(tmpRec.getParents().matherName)->getDateOfBirth() > testDate)
            {
                messageBox.critical(0,"Error","Некоректная дата рождения! ");
                return;
            }
        }
        //Проверка на правильную дату смерти
        if(!(tmpRec.isAlive()) && (tmpRec.getDateOfDeath()>QDate::currentDate() || tmpRec.getDateOfDeath() < tmpRec.getDateOfBirth()))
        {
            messageBox.critical(0,"Error","Некоректная дата смерти! ");
            return;
        }



        //Добавление новой записи в составное поле
        if(!records[i].isCreated)
        {
            if(tmpRec.getGender() == Gender::MALE)
                ui->comboBoxFatherFio->addItem(tmpRec.getFIO());
            else
                ui->comboBoxMatherFio->addItem(tmpRec.getFIO());

        }
        //Запись уже существует - изменить ФИО/Пол
        else if (!(getRecordsByFio(tmpRec.getFIO())) || !(records[i].getGender() == tmpRec.getGender()))
        {
            QString newFio = tmpRec.getFIO();
            int fioIndex;

            if (records[i].getGender() == tmpRec.getGender()) //Изменилось ФИО
            {

                if(records[i].getGender() == Gender::MALE)
                {
                    for(int i = 0; i < records[i].getChilds().size(); i ++)
                        records[i].getChilds()[i]->setFatherName(newFio);
                    fioIndex = ui->comboBoxFatherFio->findText(records[i].getFIO());
                    ui->comboBoxFatherFio->setItemText(fioIndex, newFio);
                }
                else
                {
                    for(int i = 0; i < records[i].getChilds().size(); i ++)
                        records[i].getChilds()[i]->setMatherName(newFio);
                    fioIndex = ui->comboBoxMatherFio->findText(records[i].getFIO());
                    ui->comboBoxMatherFio->setItemText(fioIndex, newFio);
                }
            }
            else
            {
                if(records[i].getGender() == Gender::MALE) //Поменялся пол
                {
                    fioIndex = ui->comboBoxFatherFio->findText(records[i].getFIO());
                    ui->comboBoxFatherFio->removeItem(fioIndex);
                    ui->comboBoxMatherFio->addItem(newFio);
                }
                else
                {
                    fioIndex = ui->comboBoxMatherFio->findText(records[i].getFIO());
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
            records[i].setFatherName(parentName);

            //Сохранение этой записи как ребёнка у другой
            bool newChild = true;
            FamilyTree* parent = getRecordsByFio(parentName);
            for (int i = 0 ; i < parent->getChilds().size();)
                newChild = (&records[i] == parent->getChilds().at(i));
            if (newChild) parent->getChilds().push_back(&records[i]);
        }
        if(ui->comboBoxMatherFio->currentIndex()) //Была выбрана мать из списка
        {
            parentName = tmpRec.getParents().matherName;
            records[i].setMatherName(parentName);

            //Сохранение этой записи как ребёнка у другой
            bool newChild = true;
            FamilyTree* parent = getRecordsByFio(parentName);
            for (int i = 0 ; i < parent->getChilds().size();)
                newChild = (&records[i] == parent->getChilds().at(i));
            if (newChild) parent->getChilds().push_back(&records[i]);
        }

        //Считывание и сохранение остальных полей
        records[i].setFIO(tmpRec.getFIO());
        records[i].setDateOfBirth(tmpRec.getDateOfBirth());
        records[i].setIsAlive(tmpRec.isAlive());
        records[i].setDateOfDeath(tmpRec.getDateOfDeath());
        records[i].setGender(tmpRec.getGender());
        records[i].setCitizenship(tmpRec.getCitizenship());
        records[i].setWasInMilitaryService(tmpRec.WasInMilitaryService());
        records[i].isCreated = true;
}


void MainWindow::_show(int i)
{
    if(records[i].isCreated)
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



