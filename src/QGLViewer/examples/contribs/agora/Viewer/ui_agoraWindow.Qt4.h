/********************************************************************************
** Form generated from reading UI file 'agoraWindow.Qt4.ui'
**
** Created: Fri Aug 31 15:21:52 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AGORAWINDOW_H
#define UI_AGORAWINDOW_H

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
#include "agoraViewer.h"

QT_BEGIN_NAMESPACE

class Ui_AgoraWindow
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
    QAction *gameWhiteIsHumanAction;
    QAction *gameBlackIsHumanAction;
    QAction *gameConfigureWhitePlayerAction;
    QAction *gameConfigureBlackPlayerAction;
    QWidget *widget;
    QGridLayout *gridLayout;
    AgoraViewer *agoraViewer;
    QMenuBar *menubar;
    QMenu *fileMenu;
    QMenu *PopupMenu;
    QMenu *helpMenu;
    QMenu *editMenu;

    void setupUi(QMainWindow *AgoraWindow)
    {
        if (AgoraWindow->objectName().isEmpty())
            AgoraWindow->setObjectName(QString::fromUtf8("AgoraWindow"));
        AgoraWindow->resize(600, 482);
        fileOpenAction = new QAction(AgoraWindow);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        fileSaveAction = new QAction(AgoraWindow);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileSaveAsAction = new QAction(AgoraWindow);
        fileSaveAsAction->setObjectName(QString::fromUtf8("fileSaveAsAction"));
        fileExitAction = new QAction(AgoraWindow);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        gameUndoAction = new QAction(AgoraWindow);
        gameUndoAction->setObjectName(QString::fromUtf8("gameUndoAction"));
        gameRedoAction = new QAction(AgoraWindow);
        gameRedoAction->setObjectName(QString::fromUtf8("gameRedoAction"));
        helpRulesAction = new QAction(AgoraWindow);
        helpRulesAction->setObjectName(QString::fromUtf8("helpRulesAction"));
        helpAboutAction = new QAction(AgoraWindow);
        helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
        gameNewGameAction = new QAction(AgoraWindow);
        gameNewGameAction->setObjectName(QString::fromUtf8("gameNewGameAction"));
        toggleAnimationAction = new QAction(AgoraWindow);
        toggleAnimationAction->setObjectName(QString::fromUtf8("toggleAnimationAction"));
        toggleAnimationAction->setCheckable(true);
        toggleAnimationAction->setChecked(true);
        togglePossibleMoveAction = new QAction(AgoraWindow);
        togglePossibleMoveAction->setObjectName(QString::fromUtf8("togglePossibleMoveAction"));
        togglePossibleMoveAction->setCheckable(true);
        togglePossibleMoveAction->setChecked(true);
        gameWhiteIsHumanAction = new QAction(AgoraWindow);
        gameWhiteIsHumanAction->setObjectName(QString::fromUtf8("gameWhiteIsHumanAction"));
        gameWhiteIsHumanAction->setCheckable(true);
        gameWhiteIsHumanAction->setChecked(true);
        gameBlackIsHumanAction = new QAction(AgoraWindow);
        gameBlackIsHumanAction->setObjectName(QString::fromUtf8("gameBlackIsHumanAction"));
        gameBlackIsHumanAction->setCheckable(true);
        gameBlackIsHumanAction->setChecked(true);
        gameConfigureWhitePlayerAction = new QAction(AgoraWindow);
        gameConfigureWhitePlayerAction->setObjectName(QString::fromUtf8("gameConfigureWhitePlayerAction"));
        gameConfigureWhitePlayerAction->setEnabled(false);
        gameConfigureBlackPlayerAction = new QAction(AgoraWindow);
        gameConfigureBlackPlayerAction->setObjectName(QString::fromUtf8("gameConfigureBlackPlayerAction"));
        gameConfigureBlackPlayerAction->setEnabled(false);
        widget = new QWidget(AgoraWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(3, 3, 3, 3);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        agoraViewer = new AgoraViewer(widget);
        agoraViewer->setObjectName(QString::fromUtf8("agoraViewer"));

        gridLayout->addWidget(agoraViewer, 0, 0, 1, 1);

        AgoraWindow->setCentralWidget(widget);
        menubar = new QMenuBar(AgoraWindow);
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
        helpMenu->setGeometry(QRect(0, 0, 133, 84));
        editMenu = new QMenu(menubar);
        editMenu->setObjectName(QString::fromUtf8("editMenu"));
        editMenu->setGeometry(QRect(71, 123, 188, 215));
        AgoraWindow->setMenuBar(menubar);

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
        editMenu->addAction(gameWhiteIsHumanAction);
        editMenu->addAction(gameConfigureWhitePlayerAction);
        editMenu->addSeparator();
        editMenu->addAction(gameBlackIsHumanAction);
        editMenu->addAction(gameConfigureBlackPlayerAction);

        retranslateUi(AgoraWindow);

        QMetaObject::connectSlotsByName(AgoraWindow);
    } // setupUi

    void retranslateUi(QMainWindow *AgoraWindow)
    {
        AgoraWindow->setWindowTitle(QApplication::translate("AgoraWindow", "Agora", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setIconText(QApplication::translate("AgoraWindow", "Load game", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileOpenAction->setToolTip(QApplication::translate("AgoraWindow", "Load a saved game", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fileOpenAction->setShortcut(QApplication::translate("AgoraWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setIconText(QApplication::translate("AgoraWindow", "Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setShortcut(QApplication::translate("AgoraWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setIconText(QApplication::translate("AgoraWindow", "Save As", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setShortcut(QString());
        fileExitAction->setIconText(QApplication::translate("AgoraWindow", "Exit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setShortcut(QApplication::translate("AgoraWindow", "Esc", 0, QApplication::UnicodeUTF8));
        gameUndoAction->setIconText(QApplication::translate("AgoraWindow", "Undo", 0, QApplication::UnicodeUTF8));
        gameUndoAction->setShortcut(QApplication::translate("AgoraWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        gameRedoAction->setIconText(QApplication::translate("AgoraWindow", "Redo", 0, QApplication::UnicodeUTF8));
        gameRedoAction->setShortcut(QApplication::translate("AgoraWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        helpRulesAction->setIconText(QApplication::translate("AgoraWindow", "Rules of Agora", 0, QApplication::UnicodeUTF8));
        helpRulesAction->setShortcut(QString());
        helpAboutAction->setIconText(QApplication::translate("AgoraWindow", "About", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setShortcut(QString());
        gameNewGameAction->setIconText(QApplication::translate("AgoraWindow", "New game", 0, QApplication::UnicodeUTF8));
        gameNewGameAction->setShortcut(QApplication::translate("AgoraWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        toggleAnimationAction->setIconText(QApplication::translate("AgoraWindow", "Animate moves", 0, QApplication::UnicodeUTF8));
        togglePossibleMoveAction->setText(QApplication::translate("AgoraWindow", "Show possible moves", 0, QApplication::UnicodeUTF8));
        togglePossibleMoveAction->setIconText(QApplication::translate("AgoraWindow", "Show possible moves", 0, QApplication::UnicodeUTF8));
        gameWhiteIsHumanAction->setText(QApplication::translate("AgoraWindow", "White player is human", 0, QApplication::UnicodeUTF8));
        gameWhiteIsHumanAction->setIconText(QApplication::translate("AgoraWindow", "White player is human", 0, QApplication::UnicodeUTF8));
        gameBlackIsHumanAction->setIconText(QApplication::translate("AgoraWindow", "Black player is human", 0, QApplication::UnicodeUTF8));
        gameConfigureWhitePlayerAction->setText(QApplication::translate("AgoraWindow", "Configure white player", 0, QApplication::UnicodeUTF8));
        gameConfigureWhitePlayerAction->setIconText(QApplication::translate("AgoraWindow", "Configure white player", 0, QApplication::UnicodeUTF8));
        gameConfigureBlackPlayerAction->setText(QApplication::translate("AgoraWindow", "Configure black player", 0, QApplication::UnicodeUTF8));
        gameConfigureBlackPlayerAction->setIconText(QApplication::translate("AgoraWindow", "Configure black player", 0, QApplication::UnicodeUTF8));
        fileMenu->setTitle(QApplication::translate("AgoraWindow", "&File", 0, QApplication::UnicodeUTF8));
        PopupMenu->setTitle(QApplication::translate("AgoraWindow", "&Display", 0, QApplication::UnicodeUTF8));
        helpMenu->setTitle(QApplication::translate("AgoraWindow", "&Help", 0, QApplication::UnicodeUTF8));
        editMenu->setTitle(QApplication::translate("AgoraWindow", "&Game", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AgoraWindow: public Ui_AgoraWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AGORAWINDOW_H
