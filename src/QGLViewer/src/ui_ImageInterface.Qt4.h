/********************************************************************************
** Form generated from reading UI file 'ImageInterface.Qt4.ui'
**
** Created: Fri Sep 7 06:05:38 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEINTERFACE_H
#define UI_IMAGEINTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ImageInterface
{
public:
    QVBoxLayout *vboxLayout;
    QHBoxLayout *hboxLayout;
    QLabel *label_2;
    QSpinBox *imgWidth;
    QSpacerItem *spacerItem;
    QLabel *label_3;
    QSpinBox *imgHeight;
    QHBoxLayout *hboxLayout1;
    QLabel *label_4;
    QSpinBox *imgQuality;
    QSpacerItem *spacerItem1;
    QHBoxLayout *hboxLayout2;
    QLabel *label;
    QDoubleSpinBox *oversampling;
    QSpacerItem *spacerItem2;
    QCheckBox *whiteBackground;
    QCheckBox *expandFrustum;
    QSpacerItem *spacerItem3;
    QHBoxLayout *hboxLayout3;
    QSpacerItem *spacerItem4;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ImageInterface)
    {
        if (ImageInterface->objectName().isEmpty())
            ImageInterface->setObjectName(QString::fromUtf8("ImageInterface"));
        ImageInterface->resize(298, 204);
        QIcon icon;
        icon.addFile(QString::fromUtf8("../../../../../../Documents and Settings/debunne/Bureau/libQGLViewer-2.2.0-1/doc/images/qglviewer.icon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ImageInterface->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(ImageInterface);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        label_2 = new QLabel(ImageInterface);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        hboxLayout->addWidget(label_2);

        imgWidth = new QSpinBox(ImageInterface);
        imgWidth->setObjectName(QString::fromUtf8("imgWidth"));
        imgWidth->setMinimum(1);
        imgWidth->setMaximum(32000);

        hboxLayout->addWidget(imgWidth);

        spacerItem = new QSpacerItem(20, 22, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        label_3 = new QLabel(ImageInterface);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        hboxLayout->addWidget(label_3);

        imgHeight = new QSpinBox(ImageInterface);
        imgHeight->setObjectName(QString::fromUtf8("imgHeight"));
        imgHeight->setMinimum(1);
        imgHeight->setMaximum(32000);

        hboxLayout->addWidget(imgHeight);


        vboxLayout->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label_4 = new QLabel(ImageInterface);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        hboxLayout1->addWidget(label_4);

        imgQuality = new QSpinBox(ImageInterface);
        imgQuality->setObjectName(QString::fromUtf8("imgQuality"));
        imgQuality->setMinimum(0);
        imgQuality->setMaximum(100);

        hboxLayout1->addWidget(imgQuality);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);


        vboxLayout->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout2->setSpacing(6);
#endif
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        label = new QLabel(ImageInterface);
        label->setObjectName(QString::fromUtf8("label"));

        hboxLayout2->addWidget(label);

        oversampling = new QDoubleSpinBox(ImageInterface);
        oversampling->setObjectName(QString::fromUtf8("oversampling"));
        oversampling->setDecimals(1);
        oversampling->setMinimum(0.1);
        oversampling->setMaximum(64);
        oversampling->setSingleStep(1);
        oversampling->setValue(1);

        hboxLayout2->addWidget(oversampling);

        spacerItem2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout2->addItem(spacerItem2);


        vboxLayout->addLayout(hboxLayout2);

        whiteBackground = new QCheckBox(ImageInterface);
        whiteBackground->setObjectName(QString::fromUtf8("whiteBackground"));

        vboxLayout->addWidget(whiteBackground);

        expandFrustum = new QCheckBox(ImageInterface);
        expandFrustum->setObjectName(QString::fromUtf8("expandFrustum"));

        vboxLayout->addWidget(expandFrustum);

        spacerItem3 = new QSpacerItem(20, 16, QSizePolicy::Minimum, QSizePolicy::Expanding);

        vboxLayout->addItem(spacerItem3);

        hboxLayout3 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout3->setSpacing(6);
#endif
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        spacerItem4 = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout3->addItem(spacerItem4);

        okButton = new QPushButton(ImageInterface);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout3->addWidget(okButton);

        cancelButton = new QPushButton(ImageInterface);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout3->addWidget(cancelButton);


        vboxLayout->addLayout(hboxLayout3);


        retranslateUi(ImageInterface);
        QObject::connect(okButton, SIGNAL(clicked()), ImageInterface, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), ImageInterface, SLOT(reject()));

        QMetaObject::connectSlotsByName(ImageInterface);
    } // setupUi

    void retranslateUi(QDialog *ImageInterface)
    {
        ImageInterface->setWindowTitle(QCoreApplication::translate("ImageInterface", "Image settings", 0));
        label_2->setText(QCoreApplication::translate("ImageInterface", "Width", 0));
#ifndef QT_NO_TOOLTIP
        imgWidth->setToolTip(QCoreApplication::translate("ImageInterface", "Width of the image (in pixels)", 0));
#endif // QT_NO_TOOLTIP
        imgWidth->setSuffix(QCoreApplication::translate("ImageInterface", " px", 0));
        label_3->setText(QCoreApplication::translate("ImageInterface", "Height", 0));
#ifndef QT_NO_TOOLTIP
        imgHeight->setToolTip(QCoreApplication::translate("ImageInterface", "Height of the image (in pixels)", 0));
#endif // QT_NO_TOOLTIP
        imgHeight->setSuffix(QCoreApplication::translate("ImageInterface", " px", 0));
        label_4->setText(QCoreApplication::translate("ImageInterface", "Image quality", 0));
#ifndef QT_NO_TOOLTIP
        imgQuality->setToolTip(QCoreApplication::translate("ImageInterface", "Between 0 (smallest files) and 100 (highest quality)", 0));
#endif // QT_NO_TOOLTIP
        label->setText(QCoreApplication::translate("ImageInterface", "Oversampling", 0));
#ifndef QT_NO_TOOLTIP
        oversampling->setToolTip(QCoreApplication::translate("ImageInterface", "Antialiases image (when larger then 1.0)", 0));
#endif // QT_NO_TOOLTIP
        oversampling->setPrefix(QCoreApplication::translate("ImageInterface", "x ", 0));
#ifndef QT_NO_TOOLTIP
        whiteBackground->setToolTip(QCoreApplication::translate("ImageInterface", "Use white as background color", 0));
#endif // QT_NO_TOOLTIP
        whiteBackground->setText(QCoreApplication::translate("ImageInterface", "Use white background", 0));
#ifndef QT_NO_TOOLTIP
        expandFrustum->setToolTip(QCoreApplication::translate("ImageInterface", "When image aspect ratio differs from viewer's one, expand frustum as needed. Fits inside current frustum otherwise.", 0));
#endif // QT_NO_TOOLTIP
        expandFrustum->setText(QCoreApplication::translate("ImageInterface", "Expand frustum if needed", 0));
        okButton->setText(QCoreApplication::translate("ImageInterface", "OK", 0));
        cancelButton->setText(QCoreApplication::translate("ImageInterface", "Cancel", 0));
    } // retranslateUi

};

namespace Ui {
    class ImageInterface: public Ui_ImageInterface {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEINTERFACE_H
