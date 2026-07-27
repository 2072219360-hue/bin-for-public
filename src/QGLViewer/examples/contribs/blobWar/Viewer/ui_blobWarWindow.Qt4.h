/********************************************************************************
** Form generated from reading UI file 'blobWarWindow.Qt4.ui'
**
** Created: Fri Aug 31 15:22:35 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLOBWARWINDOW_H
#define UI_BLOBWARWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QWidget>
#include "blobWarViewer.h"

QT_BEGIN_NAMESPACE

class Ui_BlobWarWindow
{
public:
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *fileExitAction;
    QAction *gameUndoAction;
    QAction *gameRedoAction;
    QAction *helpRulesAction;
    QAction *helpAboutAction;
    QAction *gameNewGameAction;
    QAction *toggleAnimationAction;
    QAction *togglePossibleMoveAction;
    QAction *gameRedIsHumanAction;
    QAction *gameBlueIsHumanAction;
    QAction *gameConfigureRedPlayerAction;
    QAction *gameConfigureBluePlayerAction;
    QWidget *widget;
    QGridLayout *gridLayout;
    BlobWarViewer *blobWarViewer;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *PopupMenu;
    QMenu *helpMenu;
    QMenu *editMenu;

    void setupUi(QMainWindow *BlobWarWindow)
    {
        if (BlobWarWindow->objectName().isEmpty())
            BlobWarWindow->setObjectName(QString::fromUtf8("BlobWarWindow"));
        BlobWarWindow->resize(600, 482);
        fileOpenAction = new QAction(BlobWarWindow);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        fileSaveAction = new QAction(BlobWarWindow);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileSaveAsAction = new QAction(BlobWarWindow);
        fileSaveAsAction->setObjectName(QString::fromUtf8("fileSaveAsAction"));
        fileExitAction = new QAction(BlobWarWindow);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        gameUndoAction = new QAction(BlobWarWindow);
        gameUndoAction->setObjectName(QString::fromUtf8("gameUndoAction"));
        gameRedoAction = new QAction(BlobWarWindow);
        gameRedoAction->setObjectName(QString::fromUtf8("gameRedoAction"));
        helpRulesAction = new QAction(BlobWarWindow);
        helpRulesAction->setObjectName(QString::fromUtf8("helpRulesAction"));
        helpAboutAction = new QAction(BlobWarWindow);
        helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
        gameNewGameAction = new QAction(BlobWarWindow);
        gameNewGameAction->setObjectName(QString::fromUtf8("gameNewGameAction"));
        toggleAnimationAction = new QAction(BlobWarWindow);
        toggleAnimationAction->setObjectName(QString::fromUtf8("toggleAnimationAction"));
        toggleAnimationAction->setCheckable(true);
        toggleAnimationAction->setChecked(true);
        togglePossibleMoveAction = new QAction(BlobWarWindow);
        togglePossibleMoveAction->setObjectName(QString::fromUtf8("togglePossibleMoveAction"));
        togglePossibleMoveAction->setCheckable(true);
        togglePossibleMoveAction->setChecked(true);
        gameRedIsHumanAction = new QAction(BlobWarWindow);
        gameRedIsHumanAction->setObjectName(QString::fromUtf8("gameRedIsHumanAction"));
        gameRedIsHumanAction->setCheckable(true);
        gameRedIsHumanAction->setChecked(false);
        gameBlueIsHumanAction = new QAction(BlobWarWindow);
        gameBlueIsHumanAction->setObjectName(QString::fromUtf8("gameBlueIsHumanAction"));
        gameBlueIsHumanAction->setCheckable(true);
        gameBlueIsHumanAction->setChecked(true);
        gameConfigureRedPlayerAction = new QAction(BlobWarWindow);
        gameConfigureRedPlayerAction->setObjectName(QString::fromUtf8("gameConfigureRedPlayerAction"));
        gameConfigureRedPlayerAction->setEnabled(true);
        gameConfigureBluePlayerAction = new QAction(BlobWarWindow);
        gameConfigureBluePlayerAction->setObjectName(QString::fromUtf8("gameConfigureBluePlayerAction"));
        gameConfigureBluePlayerAction->setEnabled(false);
        widget = new QWidget(BlobWarWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        blobWarViewer = new BlobWarViewer(widget);
        blobWarViewer->setObjectName(QString::fromUtf8("blobWarViewer"));

        gridLayout->addWidget(blobWarViewer, 0, 0, 1, 1);

        BlobWarWindow->setCentralWidget(widget);
        menubar = new QMenuBar(BlobWarWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 600, 22));
        fileMenu = new QMenu(menubar);
        fileMenu->setObjectName(QString::fromUtf8("fileMenu"));
        fileMenu->setGeometry(QRect(0, 0, 131, 134));
        PopupMenu = new QMenu(menubar);
        PopupMenu->setObjectName(QString::fromUtf8("PopupMenu"));
        PopupMenu->setGeometry(QRect(0, 0, 178, 84));
        helpMenu = new QMenu(menubar);
        helpMenu->setObjectName(QString::fromUtf8("helpMenu"));
        helpMenu->setGeometry(QRect(0, 0, 147, 84));
        editMenu = new QMenu(menubar);
        editMenu->setObjectName(QString::fromUtf8("editMenu"));
        editMenu->setGeometry(QRect(397, 279, 181, 215));
        BlobWarWindow->setMenuBar(menubar);

        menubar->addAction(fileMenu->menuAction());
        menubar->addAction(editMenu->menuAction());
        menubar->addAction(PopupMenu->menuAction());
        menubar->addAction(helpMenu->menuAction());
        fileMenu->addAction(fileOpenAction);
        fileMenu->addAction(fileSaveAction);
        fileMenu->addAction(fileSaveAsAction);
        fileMenu->addSeparator();
        fileMenu->addAction(fileExitAction);
        PopupMenu->addAction(togglePossibleMoveAction);
        PopupMenu->addAction(toggleAnimationAction);
        helpMenu->addAction(helpRulesAction);
        helpMenu->addAction(helpAboutAction);
        editMenu->addAction(gameNewGameAction);
        editMenu->addSeparator();
        editMenu->addAction(gameUndoAction);
        editMenu->addAction(gameRedoAction);
        editMenu->addSeparator();
        editMenu->addAction(gameBlueIsHumanAction);
        editMenu->addAction(gameConfigureBluePlayerAction);
        editMenu->addSeparator();
        editMenu->addAction(gameRedIsHumanAction);
        editMenu->addAction(gameConfigureRedPlayerAction);

        retranslateUi(BlobWarWindow);

        QMetaObject::connectSlotsByName(BlobWarWindow);
    } // setupUi

    void retranslateUi(QMainWindow *BlobWarWindow)
    {
        BlobWarWindow->setWindowTitle(QApplication::translate("BlobWarWindow", "BlobWar", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setIconText(QApplication::translate("BlobWarWindow", "Load game", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileOpenAction->setToolTip(QApplication::translate("BlobWarWindow", "Load a saved game", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fileOpenAction->setShortcut(QApplication::translate("BlobWarWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setIconText(QApplication::translate("BlobWarWindow", "Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setShortcut(QApplication::translate("BlobWarWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setIconText(QApplication::translate("BlobWarWindow", "Save As", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setShortcut(QString());
        fileExitAction->setIconText(QApplication::translate("BlobWarWindow", "Exit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setShortcut(QApplication::translate("BlobWarWindow", "Esc", 0, QApplication::UnicodeUTF8));
        gameUndoAction->setIconText(QApplication::translate("BlobWarWindow", "Undo", 0, QApplication::UnicodeUTF8));
        gameUndoAction->setShortcut(QApplication::translate("BlobWarWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        gameRedoAction->setIconText(QApplication::translate("BlobWarWindow", "Redo", 0, QApplication::UnicodeUTF8));
        gameRedoAction->setShortcut(QApplication::translate("BlobWarWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        helpRulesAction->setIconText(QApplication::translate("BlobWarWindow", "Rules of BlobWar", 0, QApplication::UnicodeUTF8));
        helpRulesAction->setShortcut(QString());
        helpAboutAction->setIconText(QApplication::translate("BlobWarWindow", "About", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setShortcut(QString());
        gameNewGameAction->setIconText(QApplication::translate("BlobWarWindow", "New game", 0, QApplication::UnicodeUTF8));
        gameNewGameAction->setShortcut(QApplication::translate("BlobWarWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        toggleAnimationAction->setIconText(QApplication::translate("BlobWarWindow", "Animate moves", 0, QApplication::UnicodeUTF8));
        togglePossibleMoveAction->setText(QApplication::translate("BlobWarWindow", "Show possible moves", 0, QApplication::UnicodeUTF8));
        togglePossibleMoveAction->setIconText(QApplication::translate("BlobWarWindow", "Show possible moves", 0, QApplication::UnicodeUTF8));
        gameRedIsHumanAction->setIconText(QApplication::translate("BlobWarWindow", "Red player is human", 0, QApplication::UnicodeUTF8));
        gameBlueIsHumanAction->setIconText(QApplication::translate("BlobWarWindow", "Blue player is human", 0, QApplication::UnicodeUTF8));
        gameConfigureRedPlayerAction->setText(QApplication::translate("BlobWarWindow", "Configure red player", 0, QApplication::UnicodeUTF8));
        gameConfigureRedPlayerAction->setIconText(QApplication::translate("BlobWarWindow", "Configure red player", 0, QApplication::UnicodeUTF8));
        gameConfigureBluePlayerAction->setText(QApplication::translate("BlobWarWindow", "Configure blue player", 0, QApplication::UnicodeUTF8));
        gameConfigureBluePlayerAction->setIconText(QApplication::translate("BlobWarWindow", "Configure blue player", 0, QApplication::UnicodeUTF8));
        fileMenu->setTitle(QApplication::translate("BlobWarWindow", "&File", 0, QApplication::UnicodeUTF8));
        PopupMenu->setTitle(QApplication::translate("BlobWarWindow", "&Display", 0, QApplication::UnicodeUTF8));
        helpMenu->setTitle(QApplication::translate("BlobWarWindow", "&Help", 0, QApplication::UnicodeUTF8));
        editMenu->setTitle(QApplication::translate("BlobWarWindow", "&Game", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class BlobWarWindow: public Ui_BlobWarWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLOBWARWINDOW_H
