#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "familytreedb.h"
#include <QFileDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct BrowserFields //Структура полей; нужна для кнопки заполнить
{
    QString fio;
    QDate dateOfBirth;
    QDate dateOfDeath;
    QString father_fio;
    QString mather_fio;
    bool isAlive;
    bool slughil;
    Gender gender;
    Country country;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void save_record();
    void show_record(int i);
    void create_record();
    void delete_record();
    void disableDeathDate();
    void closeEvent(QCloseEvent *event);
    void on_fill_clicked();
    void on_CreateDB_triggered();
    void on_OpenDB_triggered();
    void on_SaveDB_triggered();
    void on_SaveAsDB_triggered();

    void on_lightTheme_triggered();

    void on_darkTheme_triggered();

private:
    Ui::MainWindow *ui;
    FamilyTreeDB recordsDB;
    QMessageBox messageBox;
    QString m_fileName;
    int maxFioLength = 0;
    QList<BrowserFields> testRecords;

    unsigned int getRecordID(int pos);
    const QString createListWidgetField(const FamilyTreeRecord& record);
    bool correctRecordCheck(const FamilyTreeRecord& tmpRec, bool newRecord = false);
    void updateButtonsActivity();
    void clearAllFields();
};
#endif // MAINWINDOW_H
