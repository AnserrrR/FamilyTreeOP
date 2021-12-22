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
    void on_createDbButton_clicked();
    void on_openDbButton_clicked();
    void on_saveDbButton_clicked();
    void on_saveAsDbButton_clicked();

private:
    Ui::MainWindow *ui;
    FamilyTreeDB recordsDB;
    QMessageBox messageBox;
    QValidator *fioValidator;
    QString m_fileName;

    unsigned int getRecordID(int pos);
    const QString createListWidgetField(const FamilyTreeRecord& record);
    bool correctRecordCheck(const FamilyTreeRecord& tmpRec, bool newRecord = false);
    void updateButtonsActivity();
};
#endif // MAINWINDOW_H
