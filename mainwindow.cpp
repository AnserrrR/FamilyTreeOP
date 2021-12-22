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
    int pos = ui->recordsListWidget->currentRow(); //Индекс строки где находилась изменяемая запись
    unsigned int recordID = getRecordID(pos);
    FamilyTreeRecord previosVersion;
    recordsDB.record(recordID, previosVersion);

    //Текущее состояние полей
    FamilyTreeRecord tmpRec;
    tmpRec.setFIO(ui->FioEdit->text());
    tmpRec.setFatherName(ui->comboBoxFatherFio->currentText());
    tmpRec.setMatherName(ui->comboBoxMatherFio->currentText());
    tmpRec.setDateOfBirth(ui->dateEditBirthday->date());
    tmpRec.setIsAlive(ui->checkIsAlive->isChecked());
    tmpRec.setDateOfDeath(ui->dateEditDeathday->date());
    tmpRec.setGender(static_cast<Gender>(ui->comboBoxGender->currentIndex()));
    tmpRec.setCitizenship(static_cast<Country>(ui->comboBoxCitizenship->currentIndex()));
    tmpRec.setWasInMilitaryService(ui->checkSlughil->isChecked());
    tmpRec.setID(recordID);

    if(!correctRecordCheck(tmpRec))
        return;

//    if(newPos - previousIndex == 1)//Расположение записи не изменилось
//        newPos = previousIndex;

    if(tmpRec.getFIO() != previosVersion.getFIO()) //Если изменилось имя, обновить его в списке родителей
    {
        //Изменить ФИО списке родителей
        if(previosVersion.getGender() == Gender::MALE)
        {
            int fioIndex = ui->comboBoxFatherFio->findText(previosVersion.getFIO());
            ui->comboBoxFatherFio->setItemText(fioIndex, tmpRec.getFIO());
        }
        else
        {
            int fioIndex = ui->comboBoxMatherFio->findText(previosVersion.getFIO());
            ui->comboBoxMatherFio->setItemText(fioIndex, tmpRec.getFIO());
        }
    }
    if(tmpRec.getGender() != previosVersion.getGender()) //Если изменился пол переместить в список другого пола
    {
        if(previosVersion.getGender() == Gender::MALE)
        {
            //Переместить в другой комбобокс
            int fioIndex = ui->comboBoxFatherFio->findText(tmpRec.getFIO());
            ui->comboBoxFatherFio->removeItem(fioIndex);
            ui->comboBoxMatherFio->addItem(tmpRec.getFIO());
        }
        else
        {
            int fioIndex = ui->comboBoxMatherFio->findText(tmpRec.getFIO());
            ui->comboBoxMatherFio->removeItem(fioIndex);
            ui->comboBoxFatherFio->addItem(tmpRec.getFIO());
        }
    }

    QList<QString> childrenFields; //Контейнер с копиями полей детей для последующего поиска их позиций в браузере
    for(QList<unsigned int>::iterator i = tmpRec.getChilds().begin(); i != tmpRec.getChilds().end(); i++)
    {
        FamilyTreeRecord child;
        recordsDB.record(*i, child);
        childrenFields.push_back(createListWidgetField(child));
    }

    int newPos = recordsDB.update(tmpRec);
    recordsDB.record(recordID, tmpRec);

    ui->recordsListWidget->takeItem(pos);
    ui->recordsListWidget->insertItem(newPos, createListWidgetField(tmpRec));
    ui->recordsListWidget->setCurrentRow(newPos);

    for(QList<unsigned int>::iterator i = tmpRec.getChilds().begin(); i != tmpRec.getChilds().end(); i++)
    {
        FamilyTreeRecord child;
        recordsDB.record(*i, child);
        int childPos = ui->recordsListWidget->row(ui->recordsListWidget->findItems(childrenFields.takeFirst(), Qt::MatchFlag::MatchExactly).first());
        ui->recordsListWidget->takeItem(childPos);
        ui->recordsListWidget->insertItem(childPos, createListWidgetField(child));
    }
}

void MainWindow::show_record(int i) //Отображение выбранной записи
{
    FamilyTreeRecord record;
    if(i < recordsDB.count() && i >= 0) //Выбрана корректная строка
    {
        recordsDB.record(getRecordID(i), record);
        int parentIndex = ui->comboBoxFatherFio->findText(record.getParents().fatherName);
        if (parentIndex > 0) ui->comboBoxFatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxFatherFio->setCurrentIndex(0);
        parentIndex = ui->comboBoxMatherFio->findText(record.getParents().matherName);
        if (parentIndex > 0) ui->comboBoxMatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxMatherFio->setCurrentIndex(0);

        ui->FioEdit->setText(record.getFIO());
        ui->dateEditBirthday->setDate(record.getDateOfBirth());
        ui->checkIsAlive->setChecked(record.isAlive());
        disableDeathDate();
        ui->dateEditDeathday->setDate(record.getDateOfDeath());
        ui->comboBoxGender->setCurrentIndex(static_cast<int>(record.getGender()));
        ui->comboBoxCitizenship->setCurrentIndex(static_cast<int>(record.getCitizenship()));
        ui->checkSlughil->setChecked(record.WasInMilitaryService());
    }
}

void MainWindow::create_record() //Создание новой записи
{
    FamilyTreeRecord tmpRec;
    tmpRec.setFIO(ui->FioEdit->text());
    tmpRec.setFatherName(ui->comboBoxFatherFio->currentText());
    tmpRec.setMatherName(ui->comboBoxMatherFio->currentText());
    tmpRec.setDateOfBirth(ui->dateEditBirthday->date());
    tmpRec.setIsAlive(ui->checkIsAlive->isChecked());
    tmpRec.setDateOfDeath(ui->dateEditDeathday->date());
    tmpRec.setGender(static_cast<Gender>(ui->comboBoxGender->currentIndex()));
    tmpRec.setCitizenship(static_cast<Country>(ui->comboBoxCitizenship->currentIndex()));
    tmpRec.setWasInMilitaryService(ui->checkSlughil->isChecked());

    if(!correctRecordCheck(tmpRec, true))
        return;

    if(tmpRec.getGender() == Gender::MALE)
        ui->comboBoxFatherFio->addItem(tmpRec.getFIO());
    else
        ui->comboBoxMatherFio->addItem(tmpRec.getFIO());

   int insertPos = recordsDB.append(tmpRec);
   recordsDB.record(tmpRec.getID(), tmpRec);

   ui->recordsListWidget->insertItem(insertPos, createListWidgetField(tmpRec));
   ui->recordsListWidget->setCurrentRow(insertPos);

   updateButtonsActivity();
}


void MainWindow::delete_record() //Удаление записи
{
    int deleteIndex = ui->recordsListWidget->currentRow();
    unsigned int recordID = getRecordID(deleteIndex);
    FamilyTreeRecord tmpRec;
    recordsDB.record(recordID, tmpRec);

    //Убрать из списка родителей
    if(tmpRec.getGender() == MALE)
        ui->comboBoxFatherFio->removeItem(ui->comboBoxFatherFio->findText(tmpRec.getFIO()));
    else
        ui->comboBoxMatherFio->removeItem(ui->comboBoxMatherFio->findText(tmpRec.getFIO()));

    //Удалить из listWidget'а
    delete ui->recordsListWidget->currentItem();

    QList<QString> childrenFields; //Контейнер с копиями полей детей для последующего поиска их позиций в браузере
    for(QList<unsigned int>::iterator i = tmpRec.getChilds().begin(); i != tmpRec.getChilds().end(); i++)
    {
        FamilyTreeRecord child;
        recordsDB.record(*i, child);
        childrenFields.push_back(createListWidgetField(child));
    }

    recordsDB.remove(recordID);
    QList<unsigned int> childrens = tmpRec.getChilds();

    for(QList<unsigned int>::iterator i = childrens.begin(); i != childrens.end(); i++)
    {
        FamilyTreeRecord child;
        recordsDB.record(*i, child);
        int childPos = ui->recordsListWidget->row(ui->recordsListWidget->findItems(childrenFields.takeFirst(), Qt::MatchFlag::MatchExactly).first());
        ui->recordsListWidget->takeItem(childPos);
        ui->recordsListWidget->insertItem(childPos, createListWidgetField(child));
    }

    if(recordsDB.count() > deleteIndex)
        ui->recordsListWidget->currentRowChanged(deleteIndex);

    updateButtonsActivity();
}

void MainWindow::disableDeathDate() //Обновление видимости
{
    ui->dateEditDeathday->setEnabled(!ui->checkIsAlive->isChecked());
}

bool MainWindow::correctRecordCheck(const FamilyTreeRecord &tmpRec, bool newRecord) //Провека записи
{
    if(!ui->FioEdit->hasAcceptableInput())
    {
        messageBox.critical(0,"Error","ФИО неполное! ");
        return false;
    }
    QString errorStr = recordsDB.recordVerification(tmpRec, newRecord);
    if(errorStr.isEmpty())
        return true;

    messageBox.critical(0,"Error",errorStr);
    return false;
}

void MainWindow::updateButtonsActivity() //Обновление состояние активности кнопок удаление и создания
{
    ui->pushButtonSave->setEnabled(recordsDB.count() >= 1);
    ui->pushButtonDelete->setEnabled(recordsDB.count() >= 1);
}

const QString MainWindow::createListWidgetField(const FamilyTreeRecord& record) //Cоздаёт строку которую можно вставить в браузер
{
    QString rec;
    rec.append(QString::number(record.getID()));
    rec.append(". ");
    rec.append(record.getFIO());
    rec.append('\t');
    rec.append(record.getDateOfBirth().toString("dd.MM.yyyy"));
    rec.append('\t');
    if(record.isAlive())
    {
        rec.append(QString("--.--.----"));
        rec.append('\t');
    }
    else
    {
        rec.append(record.getDateOfDeath().toString("dd.MM.yyyy"));
        rec.append('\t');
    }
    if(record.getParents().fatherName != "none")
        rec.append(record.getParents().fatherName);
    if(record.getParents().fatherName != "none" && record.getParents().matherName != "none")
        rec.append(", ");
    if(record.getParents().matherName != "none")
        rec.append(record.getParents().matherName);

    return rec;
}

unsigned int MainWindow::getRecordID(int pos)
{
    QString tmpStr = ui->recordsListWidget->item(pos)->text();
    return tmpStr.split('.').first().toInt();
}

void MainWindow::on_createDbButton_clicked()
{

}

void MainWindow::on_openDbButton_clicked()
{

}

void MainWindow::on_saveDbButton_clicked()
{

}

void MainWindow::on_saveAsDbButton_clicked()
{

}
