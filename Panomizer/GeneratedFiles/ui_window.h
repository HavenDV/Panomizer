/********************************************************************************
** Form generated from reading UI file 'window.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WINDOW_H
#define UI_WINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QProgressBar *progressBar;
    QGroupBox *groupBox;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLabel *label;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton_11;
    QPushButton *pushButton_16;
    QGroupBox *groupBox_2;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton_2;
    QLabel *label_2;
    QLineEdit *lineEdit_3;
    QPushButton *pushButton_12;
    QPushButton *pushButton_17;
    QGroupBox *groupBox_3;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton_3;
    QLabel *label_3;
    QLineEdit *lineEdit_5;
    QPushButton *pushButton_13;
    QPushButton *pushButton_18;
    QGroupBox *groupBox_4;
    QLineEdit *lineEdit_10;
    QPushButton *pushButton_7;
    QLabel *label_5;
    QLineEdit *lineEdit_11;
    QPushButton *pushButton_14;
    QPushButton *pushButton_19;
    QLabel *label_4;
    QLineEdit *lineEdit_7;
    QPushButton *pushButton_6;
    QGroupBox *groupBox_5;
    QLabel *label_6;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton;
    QLabel *label_10;
    QLineEdit *lineEdit_9;
    QPushButton *pushButton_5;
    QCheckBox *checkBox;
    QPushButton *pushButton_10;
    QCheckBox *checkBox_2;
    QPushButton *pushButton_21;
    QPushButton *pushButton_8;
    QGroupBox *groupBox_6;
    QPushButton *pushButton_4;
    QPushButton *pushButton_9;
    QLabel *cstatus;
    QLabel *status;
    QPlainTextEdit *error;
    QPushButton *pushButton_15;
    QPushButton *pushButton_20;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(774, 577);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(10, 540, 701, 23));
        progressBar->setValue(0);
        progressBar->setTextVisible(true);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 331, 91));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        groupBox->setFont(font);
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(10, 30, 311, 20));
        QFont font1;
        font1.setPointSize(8);
        font1.setBold(false);
        font1.setWeight(50);
        lineEdit->setFont(font1);
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(260, 60, 60, 24));
        pushButton->setFont(font1);
        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 60, 201, 20));
        label->setFont(font1);
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(100, 60, 51, 20));
        lineEdit_2->setFont(font1);
        pushButton_11 = new QPushButton(groupBox);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        pushButton_11->setGeometry(QRect(150, 60, 21, 24));
        pushButton_11->setFont(font1);
        pushButton_16 = new QPushButton(groupBox);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        pushButton_16->setGeometry(QRect(80, 60, 21, 24));
        pushButton_16->setFont(font1);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 110, 331, 91));
        groupBox_2->setFont(font);
        lineEdit_4 = new QLineEdit(groupBox_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(10, 30, 311, 20));
        lineEdit_4->setFont(font1);
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(260, 60, 60, 24));
        pushButton_2->setFont(font1);
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 60, 191, 20));
        label_2->setFont(font1);
        lineEdit_3 = new QLineEdit(groupBox_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(100, 60, 51, 20));
        lineEdit_3->setFont(font1);
        pushButton_12 = new QPushButton(groupBox_2);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        pushButton_12->setGeometry(QRect(150, 60, 21, 24));
        pushButton_12->setFont(font1);
        pushButton_17 = new QPushButton(groupBox_2);
        pushButton_17->setObjectName(QStringLiteral("pushButton_17"));
        pushButton_17->setGeometry(QRect(80, 60, 21, 24));
        pushButton_17->setFont(font1);
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 210, 331, 91));
        groupBox_3->setFont(font);
        lineEdit_6 = new QLineEdit(groupBox_3);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(10, 30, 311, 20));
        lineEdit_6->setFont(font1);
        pushButton_3 = new QPushButton(groupBox_3);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(260, 60, 60, 24));
        pushButton_3->setFont(font1);
        pushButton_3->setAutoFillBackground(false);
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 60, 191, 20));
        label_3->setFont(font1);
        lineEdit_5 = new QLineEdit(groupBox_3);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(100, 60, 51, 20));
        lineEdit_5->setFont(font1);
        pushButton_13 = new QPushButton(groupBox_3);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        pushButton_13->setGeometry(QRect(150, 60, 21, 24));
        pushButton_13->setFont(font1);
        pushButton_18 = new QPushButton(groupBox_3);
        pushButton_18->setObjectName(QStringLiteral("pushButton_18"));
        pushButton_18->setGeometry(QRect(80, 60, 21, 24));
        pushButton_18->setFont(font1);
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 310, 331, 91));
        groupBox_4->setFont(font);
        lineEdit_10 = new QLineEdit(groupBox_4);
        lineEdit_10->setObjectName(QStringLiteral("lineEdit_10"));
        lineEdit_10->setGeometry(QRect(10, 30, 311, 20));
        lineEdit_10->setFont(font1);
        pushButton_7 = new QPushButton(groupBox_4);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        pushButton_7->setGeometry(QRect(260, 60, 60, 24));
        pushButton_7->setFont(font1);
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(10, 60, 191, 20));
        label_5->setFont(font1);
        lineEdit_11 = new QLineEdit(groupBox_4);
        lineEdit_11->setObjectName(QStringLiteral("lineEdit_11"));
        lineEdit_11->setGeometry(QRect(100, 60, 51, 20));
        lineEdit_11->setFont(font1);
        pushButton_14 = new QPushButton(groupBox_4);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        pushButton_14->setGeometry(QRect(150, 60, 21, 24));
        pushButton_14->setFont(font1);
        pushButton_19 = new QPushButton(groupBox_4);
        pushButton_19->setObjectName(QStringLiteral("pushButton_19"));
        pushButton_19->setGeometry(QRect(80, 60, 21, 24));
        pushButton_19->setFont(font1);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(20, 410, 231, 20));
        label_4->setFont(font1);
        lineEdit_7 = new QLineEdit(centralWidget);
        lineEdit_7->setObjectName(QStringLiteral("lineEdit_7"));
        lineEdit_7->setGeometry(QRect(130, 410, 51, 20));
        lineEdit_7->setFont(font1);
        pushButton_6 = new QPushButton(centralWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        pushButton_6->setEnabled(false);
        pushButton_6->setGeometry(QRect(730, 540, 31, 23));
        QFont font2;
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setWeight(75);
        pushButton_6->setFont(font2);
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(360, 310, 201, 171));
        groupBox_5->setFont(font);
        label_6 = new QLabel(groupBox_5);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 60, 61, 20));
        label_6->setFont(font1);
        radioButton_2 = new QRadioButton(groupBox_5);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(130, 60, 61, 16));
        radioButton_2->setFont(font1);
        radioButton_2->setChecked(true);
        radioButton = new QRadioButton(groupBox_5);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(70, 60, 61, 17));
        radioButton->setFont(font1);
        label_10 = new QLabel(groupBox_5);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(10, 30, 191, 16));
        label_10->setFont(font1);
        lineEdit_9 = new QLineEdit(groupBox_5);
        lineEdit_9->setObjectName(QStringLiteral("lineEdit_9"));
        lineEdit_9->setGeometry(QRect(90, 30, 81, 20));
        lineEdit_9->setFont(font1);
        pushButton_5 = new QPushButton(groupBox_5);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        pushButton_5->setGeometry(QRect(10, 140, 141, 23));
        pushButton_5->setFont(font1);
        checkBox = new QCheckBox(groupBox_5);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(20, 80, 71, 20));
        checkBox->setFont(font1);
        checkBox->setLayoutDirection(Qt::RightToLeft);
        checkBox->setCheckable(true);
        checkBox->setTristate(false);
        pushButton_10 = new QPushButton(groupBox_5);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        pushButton_10->setGeometry(QRect(160, 140, 31, 24));
        pushButton_10->setFont(font1);
        checkBox_2 = new QCheckBox(groupBox_5);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(100, 80, 91, 20));
        checkBox_2->setFont(font1);
        checkBox_2->setLayoutDirection(Qt::RightToLeft);
        pushButton_21 = new QPushButton(groupBox_5);
        pushButton_21->setObjectName(QStringLiteral("pushButton_21"));
        pushButton_21->setGeometry(QRect(10, 110, 181, 23));
        pushButton_21->setFont(font1);
        pushButton_8 = new QPushButton(centralWidget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        pushButton_8->setGeometry(QRect(20, 440, 161, 23));
        pushButton_8->setFont(font1);
        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(560, 310, 201, 151));
        groupBox_6->setFont(font);
        pushButton_4 = new QPushButton(groupBox_6);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(20, 30, 141, 24));
        pushButton_4->setFont(font1);
        pushButton_9 = new QPushButton(groupBox_6);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        pushButton_9->setGeometry(QRect(170, 30, 21, 24));
        pushButton_9->setFont(font1);
        cstatus = new QLabel(groupBox_6);
        cstatus->setObjectName(QStringLiteral("cstatus"));
        cstatus->setGeometry(QRect(20, 60, 161, 71));
        cstatus->setFont(font);
        cstatus->setAlignment(Qt::AlignCenter);
        cstatus->setWordWrap(true);
        status = new QLabel(centralWidget);
        status->setObjectName(QStringLiteral("status"));
        status->setGeometry(QRect(10, 500, 751, 31));
        status->setFont(font);
        status->setAlignment(Qt::AlignCenter);
        status->setWordWrap(true);
        error = new QPlainTextEdit(centralWidget);
        error->setObjectName(QStringLiteral("error"));
        error->setGeometry(QRect(350, 20, 411, 281));
        error->setReadOnly(true);
        pushButton_15 = new QPushButton(centralWidget);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));
        pushButton_15->setGeometry(QRect(180, 410, 21, 24));
        pushButton_15->setFont(font1);
        pushButton_20 = new QPushButton(centralWidget);
        pushButton_20->setObjectName(QStringLiteral("pushButton_20"));
        pushButton_20->setGeometry(QRect(110, 410, 21, 24));
        pushButton_20->setFont(font1);
        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Panomizer", 0));
        progressBar->setFormat(QApplication::translate("MainWindow", "\320\237\321\200\320\276\320\263\321\200\320\265\321\201\321\201: %p%", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 1", 0));
        pushButton->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0));
        label->setText(QApplication::translate("MainWindow", "\320\241\320\274\320\265\321\211\320\265\320\275\320\270\320\265                                     \320\272\320\260\320\264\321\200", 0));
        lineEdit_2->setText(QApplication::translate("MainWindow", "0", 0));
        pushButton_11->setText(QApplication::translate("MainWindow", "+", 0));
        pushButton_16->setText(QApplication::translate("MainWindow", "-", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 2", 0));
        pushButton_2->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0));
        label_2->setText(QApplication::translate("MainWindow", "\320\241\320\274\320\265\321\211\320\265\320\275\320\270\320\265                                     \320\272\320\260\320\264\321\200", 0));
        lineEdit_3->setText(QApplication::translate("MainWindow", "0", 0));
        pushButton_12->setText(QApplication::translate("MainWindow", "+", 0));
        pushButton_17->setText(QApplication::translate("MainWindow", "-", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 3", 0));
        pushButton_3->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0));
        label_3->setText(QApplication::translate("MainWindow", "\320\241\320\274\320\265\321\211\320\265\320\275\320\270\320\265                                     \320\272\320\260\320\264\321\200", 0));
        lineEdit_5->setText(QApplication::translate("MainWindow", "0", 0));
        pushButton_13->setText(QApplication::translate("MainWindow", "+", 0));
        pushButton_18->setText(QApplication::translate("MainWindow", "-", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273 4", 0));
        pushButton_7->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0));
        label_5->setText(QApplication::translate("MainWindow", "\320\241\320\274\320\265\321\211\320\265\320\275\320\270\320\265                                     \320\272\320\260\320\264\321\200", 0));
        lineEdit_11->setText(QApplication::translate("MainWindow", "0", 0));
        pushButton_14->setText(QApplication::translate("MainWindow", "+", 0));
        pushButton_19->setText(QApplication::translate("MainWindow", "-", 0));
        label_4->setText(QApplication::translate("MainWindow", "\320\236\320\261\321\211\320\265\320\265 \321\201\320\274\320\265\321\211\320\265\320\275\320\270\320\265                                      \320\272\320\260\320\264\321\200", 0));
        lineEdit_7->setText(QApplication::translate("MainWindow", "0", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_6->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\320\224\320\273\321\217 \320\275\320\260\321\207\320\260\320\273\320\260 \320\275\321\203\320\266\320\275\320\276 \321\203\320\272\320\260\320\267\320\260\321\202\321\214 4 \320\270\321\201\321\205\320\276\320\264\320\275\321\213\321\205 \321\204\320\260\320\271\320\273\320\260. </p><p>\320\244\320\260\320\271\320\273 1 \320\264\320\273\321\217 \320\273\320\265\320\262\320\276\320\263\320\276 \320\262\320\270\320\264\320\265\320\276, \320\244\320\260\320\271\320\273 2 \320\264\320\273\321\217 \321\206\320\265\320\275\321\202\321\200\320\260\320\273\321\214\320\275\320\276\320\263\320\276, \320\244\320\260\320\271\320\273 3 \320\264\320\273\321\217 \320\277\321\200\320\260\320\262\320\276\320\263\320\276 \320\270 \320\244\320\260\320\271\320\273 4 \320\264\320\273\321\217 \320\267\320\260\320\264\320\275\320\265\320\263\320\276. </p><p>\320\224\320\260\320\273\320\265\320\265 \320\274\320\276\320\266\320\275\320\276 \321\203\320\272\320\260\320\267\320\260\321\202\321\214 \321\201\320\274"
                        "\320\265\321\211\320\265\320\275\320\270\321\217, \321\200\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202 \320\274\320\263\320\275\320\276\320\262\320\265\320\275\320\275\320\276 \320\276\321\202\320\276\320\261\321\200\320\260\320\267\320\270\321\202\321\214\321\201\321\217 \320\262 \321\201\320\277\320\265\321\206\320\270\320\260\320\273\321\214\320\275\320\276\320\274 \320\276\320\272\320\275\320\265.</p><p>\320\236\320\261\321\211\320\265\320\265 \321\201\320\274\320\265\321\211\320\265\320\275\320\270\320\265 \320\264\320\276\320\261\320\260\320\262\320\273\321\217\320\265\321\202\321\201\321\217 \321\201\321\200\320\260\320\267\321\203 \320\272\320\276 \320\262\321\201\320\265\320\274 \321\204\320\260\320\271\320\273\320\260\320\274, \320\261\321\203\320\264\321\214\321\202\320\265 \320\262\320\275\320\270\320\274\320\260\321\202\320\265\320\273\321\214\320\275\321\213. </p><p>\320\237\320\276\321\201\320\273\320\265 \321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\272\320\270"
                        "/\320\277\320\276\320\264\320\263\320\276\320\275\320\272\320\270 \320\262\321\201\320\265\321\205 \320\277\320\260\321\200\320\260\320\274\320\265\321\202\321\200\320\276\320\262, \321\203\320\272\320\260\320\267\321\213\320\262\320\260\320\265\320\274 \320\264\320\273\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214 \320\270 \320\272\320\260\321\207\320\265\321\201\321\202\320\262\320\276 \320\262\320\270\320\264\320\265\320\276, \320\270 \320\275\320\260\320\266\320\270\320\274\320\260\320\265\320\274 \320\272\320\275\320\276\320\277\320\272\321\203 &quot;\320\241\320\276\321\205\321\200\320\260\320\275\320\270\321\202\321\214&quot;. \320\222\321\213\320\261\320\270\321\200\320\260\320\265\320\274 \320\262 \320\272\320\260\320\272\321\203\321\216 \320\277\320\260\320\277\320\272\321\203 \320\270 \321\201 \320\272\320\260\320\272\320\270\320\274 \320\270\320\274\320\265\320\275\320\265\320\274 \321\201\320\276\321\205\321\200\320\260\320\275\321\217\321\202\321\214, \320\266\320"
                        "\274\320\265\320\274 \320\276\320\272\320\265\320\271, \320\265\321\201\320\273\320\270 \320\275\320\265\321\202 \320\276\321\210\320\270\320\261\320\276\320\272 - \320\266\320\264\320\265\320\274, \320\265\321\201\320\273\320\270 \320\265\321\201\321\202\321\214 - \320\270\321\201\320\277\321\200\320\260\320\262\320\273\321\217\320\265\320\274 \320\270 \320\277\320\276\320\262\321\202\320\276\321\200\321\217\320\265\320\274 \320\267\320\260\320\275\320\276\320\263\320\276.</p><p>\320\222\320\262\320\270\320\264\321\203 \320\261\320\276\320\273\321\214\321\210\320\270\321\205 \321\200\320\260\320\267\320\274\320\265\321\200\320\276\320\262 \320\270\321\202\320\276\320\263\320\276\320\262\320\276\320\263\320\276 \321\204\320\260\320\271\320\273\320\260(\320\276\320\272\320\276\320\273\320\276 1\320\223\320\221/\320\274\320\270\320\275\321\203\321\202\320\260), \320\265\321\201\321\202\321\214 \320\262\320\276\320\267\320\274\320\276\320\266\320\275\320\276\321\201\321\202\321\214 \321\201\320\266\320\260\321\202"
                        "\321\214 \320\265\320\263\320\276 \320\262 3-4 \321\200\320\260\320\267\320\260. \320\222\321\213\320\261\320\270\321\200\320\260\320\265\320\274 \321\201\320\276\320\276\321\202\320\262\320\265\321\202\321\201\321\202\320\262\321\203\321\216\321\211\321\203\321\216 \321\204\321\203\320\275\320\272\321\206\320\270\321\216, \320\262\321\213\320\261\320\270\321\200\320\260\320\265\320\274 \321\204\320\260\320\271\320\273 \320\270 \321\203\320\272\320\260\320\267\321\213\320\262\320\260\320\265\320\274 \321\204\320\260\320\271\320\273 \320\275\320\260\320\267\320\275\320\260\321\207\320\265\320\275\320\270\321\217.</p><p>\320\225\321\201\320\273\320\270 \320\262\321\201\320\265 \320\277\320\273\320\276\321\205\320\276, \320\267\320\262\320\276\320\275\320\270\321\202\320\265 +79141541132.</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pushButton_6->setText(QApplication::translate("MainWindow", "?", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindow", "\320\230\321\202\320\276\320\263\320\276\320\262\321\213\320\271 \321\204\320\260\320\271\320\273", 0));
        label_6->setText(QApplication::translate("MainWindow", "\320\232\320\260\321\207\320\265\321\201\321\202\320\262\320\276:", 0));
#ifndef QT_NO_TOOLTIP
        radioButton_2->setToolTip(QApplication::translate("MainWindow", "\320\236\320\261\321\213\321\207\320\275\320\276\320\265 \320\272\320\260\321\207\320\265\321\201\321\202\320\262\320\276 \320\262\321\205\320\276\320\264\320\275\320\276\320\263\320\276 \320\262\320\270\320\264\320\265\320\276", 0));
#endif // QT_NO_TOOLTIP
        radioButton_2->setText(QApplication::translate("MainWindow", "\320\222\321\213\321\201\320\276\320\272\320\276\320\265", 0));
#ifndef QT_NO_TOOLTIP
        radioButton->setToolTip(QApplication::translate("MainWindow", "\320\222 4 \321\200\320\260\320\267\320\260 \321\205\321\203\320\266\320\265 \320\276\320\261\321\213\321\207\320\275\320\276\320\263\320\276", 0));
#endif // QT_NO_TOOLTIP
        radioButton->setText(QApplication::translate("MainWindow", "\320\235\320\270\320\267\320\272\320\276\320\265", 0));
        label_10->setText(QApplication::translate("MainWindow", "\320\224\320\273\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\321\214:                            \320\272\320\260\320\264\321\200", 0));
#ifndef QT_NO_TOOLTIP
        lineEdit_9->setToolTip(QApplication::translate("MainWindow", "\320\235\320\265 \321\203\320\272\320\260\320\267\321\213\320\262\320\260\320\271\321\202\320\265 \320\270\320\273\320\270 0 - \320\264\320\273\321\217 \320\274\320\260\320\272\321\201\320\270\320\274\320\260\320\273\321\214\320\275\320\276\320\271 \320\264\320\273\320\270\321\202\320\265\320\273\321\214\320\275\320\276\321\201\321\202\320\270", 0));
#endif // QT_NO_TOOLTIP
        lineEdit_9->setText(QApplication::translate("MainWindow", "0", 0));
        pushButton_5->setText(QApplication::translate("MainWindow", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214", 0));
#ifndef QT_NO_TOOLTIP
        checkBox->setToolTip(QApplication::translate("MainWindow", "\320\237\320\276\321\201\320\273\320\265 \321\201\320\276\320\267\320\264\320\260\320\275\320\270\320\265 \321\204\320\260\320\271\320\273 \320\261\321\203\320\264\320\265\321\202 \320\260\320\262\321\202\320\276\320\274\320\260\321\202\320\270\321\207\320\265\321\201\320\272\320\270 \321\201\320\266\320\260\321\202.", 0));
#endif // QT_NO_TOOLTIP
        checkBox->setText(QApplication::translate("MainWindow", "\320\241\320\266\320\260\321\202\321\214: ", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_10->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214 \321\201\320\276\320\267\320\264\320\260\320\275\320\270\320\265</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pushButton_10->setText(QApplication::translate("MainWindow", "\321\205", 0));
#ifndef QT_NO_TOOLTIP
        checkBox_2->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        checkBox_2->setText(QApplication::translate("MainWindow", "\320\221\320\265\320\267 \320\267\320\262\321\203\320\272\320\260:", 0));
        pushButton_21->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\265\320\264\320\277\321\200\320\276\321\201\320\274\320\276\321\202\321\200", 0));
        pushButton_8->setText(QApplication::translate("MainWindow", "\320\237\321\200\320\270\320\274\320\265\320\275\320\270\321\202\321\214 \321\201\320\274\320\265\321\211\320\265\320\275\320\270\321\217", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindow", "\320\241\320\266\320\260\321\202\321\214", 0));
        pushButton_4->setText(QApplication::translate("MainWindow", "\320\236\321\202\320\272\321\200\321\213\321\202\321\214", 0));
#ifndef QT_NO_TOOLTIP
        pushButton_9->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p>\320\236\321\202\320\274\320\265\320\275\320\270\321\202\321\214 \321\201\320\266\320\260\321\202\320\270\320\265</p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pushButton_9->setText(QApplication::translate("MainWindow", "\321\205", 0));
        cstatus->setText(QString());
        status->setText(QString());
        pushButton_15->setText(QApplication::translate("MainWindow", "+", 0));
        pushButton_20->setText(QApplication::translate("MainWindow", "-", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WINDOW_H
