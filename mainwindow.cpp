#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStyleFactory>

#define MAX_FIO_LENGTH 45

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    messageBox.setFixedSize(500,200);
    setWindowIcon(QIcon(":/img/icons8-active-directory-100.png"));

    //Валидатор для ФИО
    ui->FioEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[А-Я][а-я]+(-[А-Я][а-я]+)? [А-Я][а-я]+ [А-Я][а-я]+$")));
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

    qApp->setStyle(QStyleFactory::create("Fusion"));
    on_darkTheme_triggered();

    //10 готовых записей для тестирования
    testRecords = {
                   {"Корнилов Мечислав Нетисович", QDate(1801, 5, 3), QDate(1873, 5, 3), "none", "none", false, true, Gender::MALE, Country::RUSSIA},
                   {"Михайлова Мила Богуславовна", QDate(1806, 12, 10), QDate(1884, 05, 17), "none", "none", false, false, Gender::FEMALE, Country::UKRAINE},
                   {"Корнилова Каторина Мечиславовна", QDate(1836, 1, 18), QDate(1903, 2, 19), "Корнилов Мечислав Нетисович", "Михайлова Мила Богуславовна", false, false, Gender::FEMALE, Country::RUSSIA},
                   {"Корнилов Артур Мечиславович", QDate(1834, 2, 1), QDate(1917, 2, 1), "Корнилов Мечислав Нетисович", "Михайлова Мила Богуславовна", false, true, Gender::MALE, Country::RUSSIA},
                   {"Корнилов Елисей Мечиславович", QDate(1838, 5, 3), QDate(1921, 9, 9), "Корнилов Мечислав Нетисович", "Михайлова Мила Богуславовна", false, true, Gender::MALE, Country::RUSSIA},
                   {"Соколова Арьяна Олеговна", QDate(1839, 7, 6), QDate(1925, 10, 7), "none", "none", false, false, Gender::FEMALE, Country::POLAND},
                   {"Корнилова Ульяна Артуровна", QDate(1884, 3, 27), QDate(1944, 8, 3), "Корнилов Артур Мечиславович", "Соколова Арьяна Олеговна", false, false, Gender::FEMALE, Country::GERMANY},
                   {"Корнилов Даниил Елисеевич", QDate(1861, 3, 3), QDate(1951, 8, 3), "Корнилов Елисей Мечиславович", "none", false, true, Gender::FEMALE, Country::RUSSIA},
                   {"Сафонова Александрина Макаровна", QDate(1849, 4, 13), QDate(1930, 1, 6), "none", "none", false, false, Gender::FEMALE, Country::UKRAINE},
                   {"Корнилов Богдан Даниилович", QDate(1890, 7, 15), QDate(1966, 8, 26), "Корнилов Даниил Елисеевич", "Сафонова Александрина Макаровна", false, false, Gender::MALE, Country::RUSSIA}
                  };
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::save_record() //Изменение существующей записи
{
    int pos = ui->recordsListWidget->currentRow(); //Индекс строки где находилась изменяемая запись
    unsigned int recordID = getRecordID(pos); //Получить айди записи
    FamilyTreeRecord previosVersion;
    recordsDB.record(recordID, previosVersion); //Получить предыдущую версию записи

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

    if(!correctRecordCheck(tmpRec)) //Проверка на корректность новой версии записи
        return;

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
    for(QList<unsigned int>::iterator i = previosVersion.getChilds().begin(); i != previosVersion.getChilds().end(); i++)
    {
        FamilyTreeRecord child;
        recordsDB.record(*i, child);
        childrenFields.push_back(createListWidgetField(child));
    }

    int newPos = recordsDB.update(tmpRec); //Получить новую позицию
    recordsDB.record(recordID, tmpRec);

    //Поставить в новую позицию
    ui->recordsListWidget->takeItem(pos);
    ui->recordsListWidget->insertItem(newPos, createListWidgetField(tmpRec));
    ui->recordsListWidget->setCurrentRow(newPos);

    //Обновить поля детей в браузере
    for(QList<unsigned int>::iterator i = previosVersion.getChilds().begin(); i != previosVersion.getChilds().end(); i++)
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
        //Установить комбобокс родителей
        int parentIndex = ui->comboBoxFatherFio->findText(record.getParents().fatherName);
        if (parentIndex > 0) ui->comboBoxFatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxFatherFio->setCurrentIndex(0);
        parentIndex = ui->comboBoxMatherFio->findText(record.getParents().matherName);
        if (parentIndex > 0) ui->comboBoxMatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxMatherFio->setCurrentIndex(0);

        //Заполнить оставшиеся поля
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
    //Заполнить временную запись
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

    if(!correctRecordCheck(tmpRec, true)) //Проверка корректности записи
        return;

    //Добавить в комбобокс родителей
    if(tmpRec.getGender() == Gender::MALE)
        ui->comboBoxFatherFio->addItem(tmpRec.getFIO());
    else
        ui->comboBoxMatherFio->addItem(tmpRec.getFIO());

    //Сохранить в контейнер
   int insertPos = recordsDB.append(tmpRec);
   recordsDB.record(tmpRec.getID(), tmpRec);
    //Добавить в браузер
   ui->recordsListWidget->insertItem(insertPos, createListWidgetField(tmpRec));
   ui->recordsListWidget->setCurrentRow(insertPos);

   updateButtonsActivity();
}


void MainWindow::delete_record() //Удаление записи
{
    int deleteIndex = ui->recordsListWidget->currentRow();
    if (deleteIndex == -1) return;
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

    //Обновить поля браузера детей
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
    if(maxFioLength < record.getFIO().size()) maxFioLength = record.getFIO().size();
    rec.append(record.getFIO().leftJustified(maxFioLength, ' '));
    rec.append(" \t");
    rec.append(record.getDateOfBirth().toString("dd.MM.yyyy"));
    rec.append(" - ");
    if(record.isAlive())
    {
        rec.append(QString("--.--.----"));
        rec.append("\t\t");
    }
    else
    {
        rec.append(record.getDateOfDeath().toString("dd.MM.yyyy"));
        rec.append("\t\t");
    }
    if(record.getParents().fatherName != "none")
        rec.append(record.getParents().fatherName);
    if(record.getParents().fatherName != "none" && record.getParents().matherName != "none")
        rec.append(", ");
    if(record.getParents().matherName != "none")
        rec.append(record.getParents().matherName);

    return rec;
}

unsigned int MainWindow::getRecordID(int pos) //получение ID записи по её позиции в браузере
{
    QString tmpStr = ui->recordsListWidget->item(pos)->text();
    return tmpStr.split('.').first().toInt();
}

void MainWindow::clearAllFields() //Очстить все поля редактора
{
    ui->FioEdit->clear();
    ui->dateEditBirthday->setDate(QDate(2000, 1, 1));
    ui->checkIsAlive->setChecked(true);
    disableDeathDate();
    ui->dateEditDeathday->setDate(QDate(2000, 1, 1));
    ui->comboBoxGender->setCurrentIndex(0);
    ui->comboBoxCitizenship->setCurrentIndex(0);
    ui->checkSlughil->setChecked(false);
    ui->recordsListWidget->clear();
}

void MainWindow::closeEvent(QCloseEvent *event) //Обработка выхода из приложения
{
    if(recordsDB.isModified()) //Сохранить БД, если необходимо
        if(QMessageBox::question(this, "Выход...", "Хотите сохранить файл перед выходом?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            on_SaveDB_triggered();
}

void MainWindow::on_fill_clicked() //Заполнить десятью готовыми записями
{
    QList<BrowserFields>::const_iterator i;
    for(i = testRecords.begin(); i != testRecords.end(); i++)
    {
        int parentIndex = ui->comboBoxFatherFio->findText(i->father_fio); //Заполнение полей из массива готовых записей
        if (parentIndex > 0) ui->comboBoxFatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxFatherFio->setCurrentIndex(0);
        parentIndex = ui->comboBoxMatherFio->findText(i->mather_fio);
        if (parentIndex > 0) ui->comboBoxMatherFio->setCurrentIndex(parentIndex);
        else ui->comboBoxMatherFio->setCurrentIndex(0);

        ui->FioEdit->setText(i->fio);
        ui->dateEditBirthday->setDate(i->dateOfBirth);
        ui->checkIsAlive->setChecked(i->isAlive);
        disableDeathDate();
        ui->dateEditDeathday->setDate(i->dateOfDeath);
        ui->comboBoxGender->setCurrentIndex(static_cast<int>(i->gender));
        ui->comboBoxCitizenship->setCurrentIndex(static_cast<int>(i->country));
        ui->checkSlughil->setChecked(i->slughil);

        create_record(); //создание записи на основе полей
    }

}

void MainWindow::on_CreateDB_triggered()
{
    if(recordsDB.isModified()) //сохранить предыдущее дерево
        if(QMessageBox::question(this, "Сохранение", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            on_SaveDB_triggered();
    recordsDB.clear(); //очистить поля и браузер
    clearAllFields();
}

void MainWindow::on_OpenDB_triggered()
{
    if(recordsDB.isModified()) //Сохранить текущую, если нужно
        if(QMessageBox::question(this, "Сохранение", "Сохранить текущий файл?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
            on_SaveDB_triggered();

    m_fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("Двоичный файл (*.bin)"));
    if(recordsDB.load(m_fileName)) //загрузить в контейнер
    {
        //зполнить поля бруезра
        clearAllFields();
        QVector<BrowserField> fields = recordsDB.records();
        for (QVector<BrowserField>::const_iterator i = fields.constBegin(); i != fields.constEnd(); i++)
        {
            QString rec;
            rec.append(QString::number(i->id));
            rec.append(". ");
            if(maxFioLength < i->fio.size()) maxFioLength = i->fio.size();
            rec.append(i->fio.leftJustified(maxFioLength, ' '));
            rec.append(" \t");
            rec.append(i->dateOfBirth.toString("dd.MM.yyyy"));
            rec.append(" - ");
            if(i->isAlive)
            {
                rec.append(QString("--.--.----"));
                rec.append("\t\t");
            }
            else
            {
                rec.append(i->dateOfDeath.toString("dd.MM.yyyy"));
                rec.append("\t\t");
            }
            if(i->parents.fatherName != "none")
                rec.append(i->parents.fatherName);
            if(i->parents.fatherName != "none" && (i->parents.matherName != "none"))
                rec.append(", ");
            if(i->parents.matherName != "none")
                rec.append(i->parents.matherName);
            ui->recordsListWidget->addItem(rec);

            if(i->gender == Gender::MALE)
                ui->comboBoxFatherFio->addItem(i->fio);
            else
                ui->comboBoxMatherFio->addItem(i->fio);
        }

    }
    updateButtonsActivity();
}

void MainWindow::on_SaveDB_triggered()
{
    if(!recordsDB.save(m_fileName)) //если не выбран файл - сохранить как
        on_SaveAsDB_triggered();
}

void MainWindow::on_SaveAsDB_triggered()
{
    m_fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"), "", tr("Двоичный файл (*.bin)"));
    recordsDB.save(m_fileName);
}

void MainWindow::on_lightTheme_triggered()
{
    // Для возврата к светлой палитре достаточно
    // будет установить стандартную палитру из темы оформления
    qApp->setPalette(style()->standardPalette());
}


void MainWindow::on_darkTheme_triggered()
{
    // Создаём палитру для тёмной темы оформления
    QPalette darkPalette;

    // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25, 25, 25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    // Устанавливаем данную палитру
    qApp->setPalette(darkPalette);
}

