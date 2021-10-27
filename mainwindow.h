#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QRegularExpressionValidator>
#include <QMessageBox>
#include "familytree.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    bool _save(int recordIndex, bool newRecord = false);
    int searchInsertPos(const FamilyTree& curRec);
    FamilyTree* getRecordsByFio(const QString& fio);
    bool correctRecordCheck(int recordIndex, const FamilyTree& tmpRec, bool newRecord = false);
    void updateButtonsActivity();
    void addRecInListWidget(int insertPos);

private slots:

    void save_record();
    void show_record(int i);
    void create_record();
    void delete_record();

    void disableDeathDate();

private:
    Ui::MainWindow *ui;
    QList<FamilyTree> records;
    QMessageBox messageBox;
    QValidator *fioValidator;
};
#endif // MAINWINDOW_H
