/********************************************************************************
** Form generated from reading UI file 'sshkeycreationdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SSHKEYCREATIONDIALOG_H
#define UI_SSHKEYCREATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

namespace QSsh {

class Ui_SshKeyCreationDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *keyAlgo;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *rsa;
    QRadioButton *dsa;
    QRadioButton *ecdsa;
    QSpacerItem *horizontalSpacer_3;
    QLabel *keySize;
    QHBoxLayout *horizontalLayout_3;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_2;
    QLabel *privateKeyFileLabel;
    QHBoxLayout *horizontalLayout_4;
    QLabel *privateKeyFileValueLabel;
    QPushButton *privateKeyFileButton;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label;
    QLabel *publicKeyFileLabel;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *generateButton;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QDialog *QSsh__SshKeyCreationDialog)
    {
        if (QSsh__SshKeyCreationDialog->objectName().isEmpty())
            QSsh__SshKeyCreationDialog->setObjectName(QStringLiteral("QSsh__SshKeyCreationDialog"));
        QSsh__SshKeyCreationDialog->setEnabled(true);
        QSsh__SshKeyCreationDialog->resize(380, 231);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(QSsh__SshKeyCreationDialog->sizePolicy().hasHeightForWidth());
        QSsh__SshKeyCreationDialog->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(QSsh__SshKeyCreationDialog);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(QSsh__SshKeyCreationDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        formLayout = new QFormLayout(groupBox);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        keyAlgo = new QLabel(groupBox);
        keyAlgo->setObjectName(QStringLiteral("keyAlgo"));

        formLayout->setWidget(0, QFormLayout::LabelRole, keyAlgo);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        rsa = new QRadioButton(groupBox);
        rsa->setObjectName(QStringLiteral("rsa"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(rsa->sizePolicy().hasHeightForWidth());
        rsa->setSizePolicy(sizePolicy1);
        rsa->setChecked(true);

        horizontalLayout_2->addWidget(rsa);

        dsa = new QRadioButton(groupBox);
        dsa->setObjectName(QStringLiteral("dsa"));
        sizePolicy1.setHeightForWidth(dsa->sizePolicy().hasHeightForWidth());
        dsa->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(dsa);

        ecdsa = new QRadioButton(groupBox);
        ecdsa->setObjectName(QStringLiteral("ecdsa"));

        horizontalLayout_2->addWidget(ecdsa);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_3);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);

        keySize = new QLabel(groupBox);
        keySize->setObjectName(QStringLiteral("keySize"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(keySize->sizePolicy().hasHeightForWidth());
        keySize->setSizePolicy(sizePolicy2);

        formLayout->setWidget(1, QFormLayout::LabelRole, keySize);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        comboBox = new QComboBox(groupBox);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_3->addWidget(comboBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        formLayout->setLayout(1, QFormLayout::FieldRole, horizontalLayout_3);

        privateKeyFileLabel = new QLabel(groupBox);
        privateKeyFileLabel->setObjectName(QStringLiteral("privateKeyFileLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, privateKeyFileLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        privateKeyFileValueLabel = new QLabel(groupBox);
        privateKeyFileValueLabel->setObjectName(QStringLiteral("privateKeyFileValueLabel"));

        horizontalLayout_4->addWidget(privateKeyFileValueLabel);

        privateKeyFileButton = new QPushButton(groupBox);
        privateKeyFileButton->setObjectName(QStringLiteral("privateKeyFileButton"));

        horizontalLayout_4->addWidget(privateKeyFileButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        formLayout->setLayout(2, QFormLayout::FieldRole, horizontalLayout_4);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(3, QFormLayout::LabelRole, label);

        publicKeyFileLabel = new QLabel(groupBox);
        publicKeyFileLabel->setObjectName(QStringLiteral("publicKeyFileLabel"));

        formLayout->setWidget(3, QFormLayout::FieldRole, publicKeyFileLabel);


        verticalLayout->addWidget(groupBox);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        generateButton = new QPushButton(QSsh__SshKeyCreationDialog);
        generateButton->setObjectName(QStringLiteral("generateButton"));
        sizePolicy.setHeightForWidth(generateButton->sizePolicy().hasHeightForWidth());
        generateButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(generateButton);

        closeButton = new QPushButton(QSsh__SshKeyCreationDialog);
        closeButton->setObjectName(QStringLiteral("closeButton"));
        sizePolicy.setHeightForWidth(closeButton->sizePolicy().hasHeightForWidth());
        closeButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(closeButton);

        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

#ifndef QT_NO_SHORTCUT
        keySize->setBuddy(comboBox);
#endif // QT_NO_SHORTCUT

        retranslateUi(QSsh__SshKeyCreationDialog);
        QObject::connect(closeButton, SIGNAL(clicked()), QSsh__SshKeyCreationDialog, SLOT(close()));

        QMetaObject::connectSlotsByName(QSsh__SshKeyCreationDialog);
    } // setupUi

    void retranslateUi(QDialog *QSsh__SshKeyCreationDialog)
    {
        QSsh__SshKeyCreationDialog->setWindowTitle(QApplication::translate("QSsh::SshKeyCreationDialog", "SSH Key Configuration", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("QSsh::SshKeyCreationDialog", "Options", Q_NULLPTR));
        keyAlgo->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "Key algorithm:", Q_NULLPTR));
        rsa->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "&RSA", Q_NULLPTR));
        dsa->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "&DSA", Q_NULLPTR));
        ecdsa->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "ECDSA", Q_NULLPTR));
        keySize->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "Key &size:", Q_NULLPTR));
        privateKeyFileLabel->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "Private key file:", Q_NULLPTR));
        privateKeyFileValueLabel->setText(QString());
        privateKeyFileButton->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "Browse...", Q_NULLPTR));
        label->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "Public key file:", Q_NULLPTR));
        publicKeyFileLabel->setText(QString());
        generateButton->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "&Generate And Save Key Pair", Q_NULLPTR));
        closeButton->setText(QApplication::translate("QSsh::SshKeyCreationDialog", "&Cancel", Q_NULLPTR));
    } // retranslateUi

};

} // namespace QSsh

namespace QSsh {
namespace Ui {
    class SshKeyCreationDialog: public Ui_SshKeyCreationDialog {};
} // namespace Ui
} // namespace QSsh

#endif // UI_SSHKEYCREATIONDIALOG_H
