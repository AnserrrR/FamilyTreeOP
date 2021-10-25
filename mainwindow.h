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

    void _save(int i);
    void _show(int i);
    FamilyTree* getRecordsByFio(const QString& fio);

private slots:

    void save_record();
    void show_record();
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
