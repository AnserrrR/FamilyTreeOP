/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QDate>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *CreateDB;
    QAction *OpenDB;
    QAction *SaveDB;
    QAction *SaveAsDB;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QHBoxLayout *horizontalLayout_3;
    QListWidget *recordsListWidget;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_2;
    QLabel *labelFio;
    QLineEdit *FioEdit;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *labelFather;
    QComboBox *comboBoxFatherFio;
    QLabel *labelMather;
    QComboBox *comboBoxMatherFio;
    QLabel *labelBirthday;
    QDateEdit *dateEditBirthday;
    QCheckBox *checkIsAlive;
    QLabel *labelDeathday;
    QDateEdit *dateEditDeathday;
    QLabel *labelGender;
    QComboBox *comboBoxGender;
    QLabel *labelCitizenship;
    QComboBox *comboBoxCitizenship;
    QCheckBox *checkSlughil;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButtonCreate;
    QPushButton *pushButtonDelete;
    QPushButton *pushButtonSave;
    QPushButton *fill;
    QMenuBar *menubar;
    QMenu *menu;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1020, 496);
        MainWindow->setMinimumSize(QSize(0, 0));
        MainWindow->setDocumentMode(false);
        CreateDB = new QAction(MainWindow);
        CreateDB->setObjectName(QString::fromUtf8("CreateDB"));
        OpenDB = new QAction(MainWindow);
        OpenDB->setObjectName(QString::fromUtf8("OpenDB"));
        SaveDB = new QAction(MainWindow);
        SaveDB->setObjectName(QString::fromUtf8("SaveDB"));
        SaveAsDB = new QAction(MainWindow);
        SaveAsDB->setObjectName(QString::fromUtf8("SaveAsDB"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        recordsListWidget = new QListWidget(centralwidget);
        recordsListWidget->setObjectName(QString::fromUtf8("recordsListWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(9);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(recordsListWidget->sizePolicy().hasHeightForWidth());
        recordsListWidget->setSizePolicy(sizePolicy);
        recordsListWidget->setMinimumSize(QSize(600, 0));

        horizontalLayout_3->addWidget(recordsListWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setLabelAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        formLayout_2->setFormAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelFio = new QLabel(centralwidget);
        labelFio->setObjectName(QString::fromUtf8("labelFio"));
        labelFio->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_2->setWidget(0, QFormLayout::LabelRole, labelFio);

        FioEdit = new QLineEdit(centralwidget);
        FioEdit->setObjectName(QString::fromUtf8("FioEdit"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, FioEdit);

        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        labelFather = new QLabel(groupBox);
        labelFather->setObjectName(QString::fromUtf8("labelFather"));

        formLayout->setWidget(0, QFormLayout::LabelRole, labelFather);

        comboBoxFatherFio = new QComboBox(groupBox);
        comboBoxFatherFio->addItem(QString());
        comboBoxFatherFio->setObjectName(QString::fromUtf8("comboBoxFatherFio"));
        comboBoxFatherFio->setMinimumSize(QSize(200, 0));

        formLayout->setWidget(0, QFormLayout::FieldRole, comboBoxFatherFio);

        labelMather = new QLabel(groupBox);
        labelMather->setObjectName(QString::fromUtf8("labelMather"));

        formLayout->setWidget(1, QFormLayout::LabelRole, labelMather);

        comboBoxMatherFio = new QComboBox(groupBox);
        comboBoxMatherFio->addItem(QString());
        comboBoxMatherFio->setObjectName(QString::fromUtf8("comboBoxMatherFio"));

        formLayout->setWidget(1, QFormLayout::FieldRole, comboBoxMatherFio);


        formLayout_2->setWidget(1, QFormLayout::FieldRole, groupBox);

        labelBirthday = new QLabel(centralwidget);
        labelBirthday->setObjectName(QString::fromUtf8("labelBirthday"));
        labelBirthday->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_2->setWidget(2, QFormLayout::LabelRole, labelBirthday);

        dateEditBirthday = new QDateEdit(centralwidget);
        dateEditBirthday->setObjectName(QString::fromUtf8("dateEditBirthday"));
        dateEditBirthday->setMinimumDate(QDate(1752, 9, 14));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, dateEditBirthday);

        checkIsAlive = new QCheckBox(centralwidget);
        checkIsAlive->setObjectName(QString::fromUtf8("checkIsAlive"));

        formLayout_2->setWidget(3, QFormLayout::FieldRole, checkIsAlive);

        labelDeathday = new QLabel(centralwidget);
        labelDeathday->setObjectName(QString::fromUtf8("labelDeathday"));
        labelDeathday->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_2->setWidget(4, QFormLayout::LabelRole, labelDeathday);

        dateEditDeathday = new QDateEdit(centralwidget);
        dateEditDeathday->setObjectName(QString::fromUtf8("dateEditDeathday"));

        formLayout_2->setWidget(4, QFormLayout::FieldRole, dateEditDeathday);

        labelGender = new QLabel(centralwidget);
        labelGender->setObjectName(QString::fromUtf8("labelGender"));
        labelGender->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_2->setWidget(5, QFormLayout::LabelRole, labelGender);

        comboBoxGender = new QComboBox(centralwidget);
        comboBoxGender->addItem(QString());
        comboBoxGender->addItem(QString());
        comboBoxGender->setObjectName(QString::fromUtf8("comboBoxGender"));

        formLayout_2->setWidget(5, QFormLayout::FieldRole, comboBoxGender);

        labelCitizenship = new QLabel(centralwidget);
        labelCitizenship->setObjectName(QString::fromUtf8("labelCitizenship"));
        labelCitizenship->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        formLayout_2->setWidget(6, QFormLayout::LabelRole, labelCitizenship);

        comboBoxCitizenship = new QComboBox(centralwidget);
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->addItem(QString());
        comboBoxCitizenship->setObjectName(QString::fromUtf8("comboBoxCitizenship"));

        formLayout_2->setWidget(6, QFormLayout::FieldRole, comboBoxCitizenship);

        checkSlughil = new QCheckBox(centralwidget);
        checkSlughil->setObjectName(QString::fromUtf8("checkSlughil"));

        formLayout_2->setWidget(7, QFormLayout::FieldRole, checkSlughil);


        verticalLayout->addLayout(formLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        pushButtonCreate = new QPushButton(centralwidget);
        pushButtonCreate->setObjectName(QString::fromUtf8("pushButtonCreate"));

        horizontalLayout_4->addWidget(pushButtonCreate);

        pushButtonDelete = new QPushButton(centralwidget);
        pushButtonDelete->setObjectName(QString::fromUtf8("pushButtonDelete"));

        horizontalLayout_4->addWidget(pushButtonDelete);

        pushButtonSave = new QPushButton(centralwidget);
        pushButtonSave->setObjectName(QString::fromUtf8("pushButtonSave"));

        horizontalLayout_4->addWidget(pushButtonSave);

        fill = new QPushButton(centralwidget);
        fill->setObjectName(QString::fromUtf8("fill"));

        horizontalLayout_4->addWidget(fill);


        verticalLayout->addLayout(horizontalLayout_4);


        horizontalLayout_3->addLayout(verticalLayout);


        horizontalLayout->addLayout(horizontalLayout_3);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1020, 20));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menu->addAction(CreateDB);
        menu->addAction(OpenDB);
        menu->addAction(SaveDB);
        menu->addAction(SaveAsDB);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\320\241\320\265\320\274\320\265\320\271\320\275\320\276\320\265 \320\264\321\200\320\265\320\262\320\276", nullptr));
        CreateDB->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", nullptr));
        OpenDB->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", nullptr));
        SaveDB->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\275\320\270\321\202\321\214", nullptr));
        SaveAsDB->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214 \320\272\320\260\320\272", nullptr));
        labelFio->setText(QCoreApplication::translate("MainWindow", "\320\244\320\230\320\236:", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\320\240\320\276\320\264\320\270\321\202\320\265\320\273\320\270", nullptr));
        labelFather->setText(QCoreApplication::translate("MainWindow", "\320\236\321\202\320\265\321\206:", nullptr));
        comboBoxFatherFio->setItemText(0, QCoreApplication::translate("MainWindow", "\320\275\320\265 \320\267\320\260\320\264\320\260\320\275", nullptr));

        labelMather->setText(QCoreApplication::translate("MainWindow", "\320\234\320\260\321\202\321\214:", nullptr));
        comboBoxMatherFio->setItemText(0, QCoreApplication::translate("MainWindow", "\320\275\320\265 \320\267\320\260\320\264\320\260\320\275\320\260", nullptr));

        labelBirthday->setText(QCoreApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260 \321\200\320\276\320\266\320\264\320\265\320\275\320\270\321\217:", nullptr));
        checkIsAlive->setText(QCoreApplication::translate("MainWindow", "\320\226\320\270\320\262", nullptr));
        labelDeathday->setText(QCoreApplication::translate("MainWindow", "\320\224\320\260\321\202\320\260 \321\201\320\274\320\265\321\200\321\202\320\270:", nullptr));
        labelGender->setText(QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273:", nullptr));
        comboBoxGender->setItemText(0, QCoreApplication::translate("MainWindow", "\320\234\321\203\320\266.", nullptr));
        comboBoxGender->setItemText(1, QCoreApplication::translate("MainWindow", "\320\226\320\265\320\275.", nullptr));

        labelCitizenship->setText(QCoreApplication::translate("MainWindow", "\320\223\321\200\320\260\320\266\320\264\320\260\320\275\321\201\321\202\320\262\320\276:", nullptr));
        comboBoxCitizenship->setItemText(0, QCoreApplication::translate("MainWindow", "\320\240\320\276\321\201\321\201\320\270\321\217", nullptr));
        comboBoxCitizenship->setItemText(1, QCoreApplication::translate("MainWindow", "\320\243\320\272\321\200\320\260\320\270\320\275\320\260", nullptr));
        comboBoxCitizenship->setItemText(2, QCoreApplication::translate("MainWindow", "\320\237\320\276\320\273\321\214\321\210\320\260", nullptr));
        comboBoxCitizenship->setItemText(3, QCoreApplication::translate("MainWindow", "\320\223\320\265\321\200\320\274\320\260\320\275\320\270\321\217", nullptr));
        comboBoxCitizenship->setItemText(4, QCoreApplication::translate("MainWindow", "\320\241\320\250\320\220", nullptr));
        comboBoxCitizenship->setItemText(5, QCoreApplication::translate("MainWindow", "\320\230\320\267\321\200\320\260\320\270\320\273\321\214", nullptr));
        comboBoxCitizenship->setItemText(6, QCoreApplication::translate("MainWindow", "\320\257\320\277\320\276\320\275\320\270\321\217", nullptr));
        comboBoxCitizenship->setItemText(7, QCoreApplication::translate("MainWindow", "\320\232\320\270\320\277\321\200", nullptr));

        checkSlughil->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\201\321\202\320\276\321\217\320\273(\320\260) \320\275\320\260 \320\262\320\276\320\265\320\275\320\275\320\276\320\271 \321\201\320\273\321\203\320\266\320\261\320\265", nullptr));
        pushButtonCreate->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \320\267\320\260\320\277\320\270\321\201\321\214", nullptr));
        pushButtonDelete->setText(QCoreApplication::translate("MainWindow", "\320\243\320\264\320\260\320\273\320\270\321\202\321\214", nullptr));
        pushButtonSave->setText(QCoreApplication::translate("MainWindow", "\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214", nullptr));
        fill->setText(QCoreApplication::translate("MainWindow", "\320\227\320\260\320\277\320\276\320\273\320\275\320\270\321\202\321\214", nullptr));
        menu->setTitle(QCoreApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
