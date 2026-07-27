/********************************************************************************
** Form generated from reading UI file 'computerPlayerInterface.Qt4.ui'
**
** Created: Fri Aug 31 15:22:35 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPUTERPLAYERINTERFACE_H
#define UI_COMPUTERPLAYERINTERFACE_H

#include <Qt3Support/Q3MimeSourceFactory>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ComputerPlayerInterface
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *programText;
    QLineEdit *programNameLineEdit;
    QPushButton *browseButton;
    QHBoxLayout *hboxLayout1;
    QLabel *maxTimeText;
    QSpinBox *allowedTimeSpinBox;
    QHBoxLayout *hboxLayout2;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QSpacerItem *spacerItem1;
    QPushButton *cancelButton;
    QSpacerItem *spacerItem2;

    void setupUi(QDialog *ComputerPlayerInterface)
    {
        if (ComputerPlayerInterface->objectName().isEmpty())
            ComputerPlayerInterface->setObjectName(QString::fromUtf8("ComputerPlayerInterface"));
        ComputerPlayerInterface->resize(303, 133);
        vboxLayout = new QVBoxLayout(ComputerPlayerInterface);
        vboxLayout->setSpacing(6);
        vboxLayout->setContentsMargins(10, 10, 10, 10);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        programText = new QLabel(ComputerPlayerInterface);
        programText->setObjectName(QString::fromUtf8("programText"));

        hboxLayout->addWidget(programText);

        programNameLineEdit = new QLineEdit(ComputerPlayerInterface);
        programNameLineEdit->setObjectName(QString::fromUtf8("programNameLineEdit"));

        hboxLayout->addWidget(programNameLineEdit);

        browseButton = new QPushButton(ComputerPlayerInterface);
        browseButton->setObjectName(QString::fromUtf8("browseButton"));

        hboxLayout->addWidget(browseButton);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setSpacing(6);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        maxTimeText = new QLabel(ComputerPlayerInterface);
        maxTimeText->setObjectName(QString::fromUtf8("maxTimeText"));

        hboxLayout1->addWidget(maxTimeText);

        allowedTimeSpinBox = new QSpinBox(ComputerPlayerInterface);
        allowedTimeSpinBox->setObjectName(QString::fromUtf8("allowedTimeSpinBox"));
        allowedTimeSpinBox->setMinimum(1);
        allowedTimeSpinBox->setMaximum(999);
        allowedTimeSpinBox->setValue(1);

        hboxLayout1->addWidget(allowedTimeSpinBox);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setSpacing(6);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        spacerItem = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem);

        okButton = new QPushButton(ComputerPlayerInterface);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout2->addWidget(okButton);

        spacerItem1 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem1);

        cancelButton = new QPushButton(ComputerPlayerInterface);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout2->addWidget(cancelButton);

        spacerItem2 = new QSpacerItem(10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem2);


        vboxLayout->addLayout(hboxLayout2);


        retranslateUi(ComputerPlayerInterface);
        QObject::connect(okButton, SIGNAL(released()), ComputerPlayerInterface, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(released()), ComputerPlayerInterface, SLOT(reject()));

        QMetaObject::connectSlotsByName(ComputerPlayerInterface);
    } // setupUi

    void retranslateUi(QDialog *ComputerPlayerInterface)
    {
        ComputerPlayerInterface->setWindowTitle(QApplication::translate("ComputerPlayerInterface", "Player configuration", 0, QApplication::UnicodeUTF8));
        programText->setText(QApplication::translate("ComputerPlayerInterface", "Program", 0, QApplication::UnicodeUTF8));
        browseButton->setText(QApplication::translate("ComputerPlayerInterface", "Browse", 0, QApplication::UnicodeUTF8));
        maxTimeText->setText(QApplication::translate("ComputerPlayerInterface", "Allowed time", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        allowedTimeSpinBox->setToolTip(QApplication::translate("ComputerPlayerInterface", "Maximum computer program relexion time", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        allowedTimeSpinBox->setSpecialValueText(QApplication::translate("ComputerPlayerInterface", "1 second", 0, QApplication::UnicodeUTF8));
        allowedTimeSpinBox->setSuffix(QApplication::translate("ComputerPlayerInterface", " seconds", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("ComputerPlayerInterface", "Ok", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("ComputerPlayerInterface", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ComputerPlayerInterface: public Ui_ComputerPlayerInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPUTERPLAYERINTERFACE_H
