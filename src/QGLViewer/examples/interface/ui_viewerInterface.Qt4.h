/********************************************************************************
** Form generated from reading UI file 'viewerInterface.Qt4.ui'
**
** Created: Fri Aug 31 15:20:06 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIEWERINTERFACE_H
#define UI_VIEWERINTERFACE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include "interface.h"

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QVBoxLayout *vboxLayout;
    Viewer *viewer;
    QHBoxLayout *hboxLayout;
    QCheckBox *FPSCheckBox;
    QCheckBox *GridCheckBox;
    QCheckBox *AxisCheckBox;
    QSpacerItem *spacerItem;
    QPushButton *cancelButton;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QString::fromUtf8("Dialog"));
        Dialog->resize(650, 468);
        vboxLayout = new QVBoxLayout(Dialog);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
        vboxLayout->setContentsMargins(8, 8, 8, 8);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        viewer = new Viewer(Dialog);
        viewer->setObjectName(QString::fromUtf8("viewer"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(1);
        sizePolicy.setHeightForWidth(viewer->sizePolicy().hasHeightForWidth());
        viewer->setSizePolicy(sizePolicy);

        vboxLayout->addWidget(viewer);

        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        FPSCheckBox = new QCheckBox(Dialog);
        FPSCheckBox->setObjectName(QString::fromUtf8("FPSCheckBox"));

        hboxLayout->addWidget(FPSCheckBox);

        GridCheckBox = new QCheckBox(Dialog);
        GridCheckBox->setObjectName(QString::fromUtf8("GridCheckBox"));

        hboxLayout->addWidget(GridCheckBox);

        AxisCheckBox = new QCheckBox(Dialog);
        AxisCheckBox->setObjectName(QString::fromUtf8("AxisCheckBox"));

        hboxLayout->addWidget(AxisCheckBox);

        spacerItem = new QSpacerItem(141, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        cancelButton = new QPushButton(Dialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        vboxLayout->addLayout(hboxLayout);


        retranslateUi(Dialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), Dialog, SLOT(reject()));
        QObject::connect(FPSCheckBox, SIGNAL(clicked(bool)), viewer, SLOT(setFPSIsDisplayed(bool)));
        QObject::connect(AxisCheckBox, SIGNAL(clicked(bool)), viewer, SLOT(setAxisIsDrawn(bool)));
        QObject::connect(GridCheckBox, SIGNAL(clicked(bool)), viewer, SLOT(setGridIsDrawn(bool)));
        QObject::connect(viewer, SIGNAL(gridIsDrawnChanged(bool)), GridCheckBox, SLOT(setChecked(bool)));
        QObject::connect(viewer, SIGNAL(axisIsDrawnChanged(bool)), AxisCheckBox, SLOT(setChecked(bool)));
        QObject::connect(viewer, SIGNAL(FPSIsDisplayedChanged(bool)), FPSCheckBox, SLOT(setChecked(bool)));

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Interface", 0, QApplication::UnicodeUTF8));
        FPSCheckBox->setText(QApplication::translate("Dialog", "FPS", 0, QApplication::UnicodeUTF8));
        GridCheckBox->setText(QApplication::translate("Dialog", "Grid", 0, QApplication::UnicodeUTF8));
        AxisCheckBox->setText(QApplication::translate("Dialog", "Axis", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Dialog", "Quit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIEWERINTERFACE_H
