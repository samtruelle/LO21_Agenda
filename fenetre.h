/********************************************************************************
** Form generated from reading UI file 'designerEk4228.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DESIGNEREK4228_H
#define DESIGNEREK4228_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCalendarWidget>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>

QT_BEGIN_NAMESPACE

class Ui_Agenda
{
public:
    QPushButton *pushButton;
    QTextEdit *textEdit;
    QPlainTextEdit *plainTextEdit;
    QCalendarWidget *calendarWidget;

    void setupUi(QDialog *Agenda)
    {
        if (Agenda->objectName().isEmpty())
            Agenda->setObjectName(QStringLiteral("Agenda"));
        Agenda->resize(574, 724);
        pushButton = new QPushButton(Agenda);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(390, 450, 75, 23));
        textEdit = new QTextEdit(Agenda);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(100, 50, 401, 41));
        plainTextEdit = new QPlainTextEdit(Agenda);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(100, 120, 401, 31));
        calendarWidget = new QCalendarWidget(Agenda);
        calendarWidget->setObjectName(QStringLiteral("calendarWidget"));
        calendarWidget->setGeometry(QRect(90, 200, 411, 201));
        calendarWidget->setLayoutDirection(Qt::LeftToRight);
        calendarWidget->setAutoFillBackground(true);
        calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
        calendarWidget->setDateEditAcceptDelay(1500);
        QWidget::setTabOrder(calendarWidget, plainTextEdit);
        QWidget::setTabOrder(plainTextEdit, pushButton);
        QWidget::setTabOrder(pushButton, textEdit);

        retranslateUi(Agenda);

        QMetaObject::connectSlotsByName(Agenda);
    } // setupUi

    void retranslateUi(QDialog *Agenda)
    {
        Agenda->setWindowTitle(QApplication::translate("Agenda", "Dialog", 0));
        pushButton->setText(QApplication::translate("Agenda", "OK", 0));
        textEdit->setHtml(QApplication::translate("Agenda", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt;\">Votre Agenda</span></p></body></html>", 0));
        plainTextEdit->setPlainText(QApplication::translate("Agenda", "Veuillez selectionner une date :", 0));
    } // retranslateUi

};

namespace Ui {
    class Agenda: public Ui_Agenda {};
} // namespace Ui

QT_END_NAMESPACE

#endif // DESIGNEREK4228_H
