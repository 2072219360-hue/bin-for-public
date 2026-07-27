/********************************************************************************
** Form generated from reading UI file 'dvonnwindow.Qt4.ui'
**
** Created: Fri Aug 31 15:23:05 2012
**      by: Qt User Interface Compiler version 4.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DVONNWINDOW_H
#define UI_DVONNWINDOW_H

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
#include "dvonnviewer.h"

QT_BEGIN_NAMESPACE

class Ui_DvonnWindow
{
public:
    QAction *fileOpenAction;
    QAction *fileSaveAction;
    QAction *fileSaveAsAction;
    QAction *filePrintAction;
    QAction *fileExitAction;
    QAction *gameUndoAction;
    QAction *gameRedoAction;
    QAction *helpHelpAction;
    QAction *helpAboutAction;
    QAction *gameNewGameAction;
    QAction *displayTogglePossDestAction;
    QAction *displayToggleTexturesAction;
    QAction *displayToggleLightsAction;
    QAction *gameRandomlyFinishAction;
    QAction *displayToggleDragToPlayAction;
    QAction *displayToggleShowLabelsAction;
    QAction *gameRandomlyMoveStackAction;
    QAction *displayToggleShowAnimationAction;
    QAction *helpMouseBindingsAction;
    QAction *optionsAnimate_playsAction;
    QActionGroup *levelActionGroup;
    QAction *easyAction;
    QAction *averageAction;
    QAction *difficultAction;
    QWidget *widget;
    QGridLayout *gridLayout;
    DvonnViewer *dvonnViewer;
    QMenuBar *menubar;
    QMenu *File;
    QMenu *Game;
    QMenu *Options;
    QMenu *Help;

    void setupUi(QMainWindow *DvonnWindow)
    {
        if (DvonnWindow->objectName().isEmpty())
            DvonnWindow->setObjectName(QString::fromUtf8("DvonnWindow"));
        DvonnWindow->resize(600, 500);
        fileOpenAction = new QAction(DvonnWindow);
        fileOpenAction->setObjectName(QString::fromUtf8("fileOpenAction"));
        QIcon icon;
        icon.addFile(QString::fromUtf8("image2"), QSize(), QIcon::Normal, QIcon::Off);
        fileOpenAction->setIcon(icon);
        fileSaveAction = new QAction(DvonnWindow);
        fileSaveAction->setObjectName(QString::fromUtf8("fileSaveAction"));
        fileSaveAction->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("image3"), QSize(), QIcon::Normal, QIcon::Off);
        fileSaveAction->setIcon(icon1);
        fileSaveAsAction = new QAction(DvonnWindow);
        fileSaveAsAction->setObjectName(QString::fromUtf8("fileSaveAsAction"));
        filePrintAction = new QAction(DvonnWindow);
        filePrintAction->setObjectName(QString::fromUtf8("filePrintAction"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8("image4"), QSize(), QIcon::Normal, QIcon::Off);
        filePrintAction->setIcon(icon2);
        fileExitAction = new QAction(DvonnWindow);
        fileExitAction->setObjectName(QString::fromUtf8("fileExitAction"));
        gameUndoAction = new QAction(DvonnWindow);
        gameUndoAction->setObjectName(QString::fromUtf8("gameUndoAction"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8("image5"), QSize(), QIcon::Normal, QIcon::Off);
        gameUndoAction->setIcon(icon3);
        gameRedoAction = new QAction(DvonnWindow);
        gameRedoAction->setObjectName(QString::fromUtf8("gameRedoAction"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8("image6"), QSize(), QIcon::Normal, QIcon::Off);
        gameRedoAction->setIcon(icon4);
        helpHelpAction = new QAction(DvonnWindow);
        helpHelpAction->setObjectName(QString::fromUtf8("helpHelpAction"));
        helpAboutAction = new QAction(DvonnWindow);
        helpAboutAction->setObjectName(QString::fromUtf8("helpAboutAction"));
        gameNewGameAction = new QAction(DvonnWindow);
        gameNewGameAction->setObjectName(QString::fromUtf8("gameNewGameAction"));
        displayTogglePossDestAction = new QAction(DvonnWindow);
        displayTogglePossDestAction->setObjectName(QString::fromUtf8("displayTogglePossDestAction"));
        displayTogglePossDestAction->setCheckable(true);
        displayTogglePossDestAction->setChecked(true);
        displayToggleTexturesAction = new QAction(DvonnWindow);
        displayToggleTexturesAction->setObjectName(QString::fromUtf8("displayToggleTexturesAction"));
        displayToggleTexturesAction->setCheckable(true);
        displayToggleTexturesAction->setChecked(true);
        displayToggleLightsAction = new QAction(DvonnWindow);
        displayToggleLightsAction->setObjectName(QString::fromUtf8("displayToggleLightsAction"));
        displayToggleLightsAction->setCheckable(true);
        displayToggleLightsAction->setChecked(true);
        gameRandomlyFinishAction = new QAction(DvonnWindow);
        gameRandomlyFinishAction->setObjectName(QString::fromUtf8("gameRandomlyFinishAction"));
        displayToggleDragToPlayAction = new QAction(DvonnWindow);
        displayToggleDragToPlayAction->setObjectName(QString::fromUtf8("displayToggleDragToPlayAction"));
        displayToggleDragToPlayAction->setCheckable(true);
        displayToggleShowLabelsAction = new QAction(DvonnWindow);
        displayToggleShowLabelsAction->setObjectName(QString::fromUtf8("displayToggleShowLabelsAction"));
        displayToggleShowLabelsAction->setCheckable(true);
        displayToggleShowLabelsAction->setChecked(false);
        gameRandomlyMoveStackAction = new QAction(DvonnWindow);
        gameRandomlyMoveStackAction->setObjectName(QString::fromUtf8("gameRandomlyMoveStackAction"));
        displayToggleShowAnimationAction = new QAction(DvonnWindow);
        displayToggleShowAnimationAction->setObjectName(QString::fromUtf8("displayToggleShowAnimationAction"));
        displayToggleShowAnimationAction->setCheckable(true);
        displayToggleShowAnimationAction->setChecked(true);
        helpMouseBindingsAction = new QAction(DvonnWindow);
        helpMouseBindingsAction->setObjectName(QString::fromUtf8("helpMouseBindingsAction"));
        optionsAnimate_playsAction = new QAction(DvonnWindow);
        optionsAnimate_playsAction->setObjectName(QString::fromUtf8("optionsAnimate_playsAction"));
        optionsAnimate_playsAction->setCheckable(true);
        optionsAnimate_playsAction->setChecked(true);
        levelActionGroup = new QActionGroup(DvonnWindow);
        levelActionGroup->setObjectName(QString::fromUtf8("levelActionGroup"));
        easyAction = new QAction(levelActionGroup);
        easyAction->setObjectName(QString::fromUtf8("easyAction"));
        easyAction->setCheckable(true);
        easyAction->setChecked(true);
        easyAction->setEnabled(false);
        averageAction = new QAction(levelActionGroup);
        averageAction->setObjectName(QString::fromUtf8("averageAction"));
        averageAction->setCheckable(true);
        averageAction->setEnabled(false);
        difficultAction = new QAction(levelActionGroup);
        difficultAction->setObjectName(QString::fromUtf8("difficultAction"));
        difficultAction->setCheckable(true);
        difficultAction->setEnabled(false);
        widget = new QWidget(DvonnWindow);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(10, 10, 10, 10);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dvonnViewer = new DvonnViewer(widget);
        dvonnViewer->setObjectName(QString::fromUtf8("dvonnViewer"));

        gridLayout->addWidget(dvonnViewer, 0, 0, 1, 1);

        DvonnWindow->setCentralWidget(widget);
        menubar = new QMenuBar(DvonnWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 600, 22));
        File = new QMenu(menubar);
        File->setObjectName(QString::fromUtf8("File"));
        File->setGeometry(QRect(0, 0, 157, 165));
        Game = new QMenu(menubar);
        Game->setObjectName(QString::fromUtf8("Game"));
        Game->setGeometry(QRect(0, 0, 243, 234));
        Options = new QMenu(menubar);
        Options->setObjectName(QString::fromUtf8("Options"));
        Options->setGeometry(QRect(0, 0, 169, 122));
        Help = new QMenu(menubar);
        Help->setObjectName(QString::fromUtf8("Help"));
        Help->setGeometry(QRect(0, 0, 141, 115));
        DvonnWindow->setMenuBar(menubar);

        menubar->addAction(File->menuAction());
        menubar->addAction(Game->menuAction());
        menubar->addAction(Options->menuAction());
        menubar->addAction(Help->menuAction());
        File->addAction(fileOpenAction);
        File->addAction(fileSaveAction);
        File->addAction(fileSaveAsAction);
        File->addSeparator();
        File->addAction(filePrintAction);
        File->addSeparator();
        File->addAction(fileExitAction);
        Game->addAction(gameUndoAction);
        Game->addAction(gameRedoAction);
        Game->addSeparator();
        Game->addAction(gameNewGameAction);
        Game->addAction(gameRandomlyFinishAction);
        Game->addAction(gameRandomlyMoveStackAction);
        Game->addSeparator();
        Game->addAction(easyAction);
        Game->addAction(averageAction);
        Game->addAction(difficultAction);
        Game->addSeparator();
        Options->addAction(displayTogglePossDestAction);
        Options->addAction(displayToggleShowLabelsAction);
        Options->addAction(displayToggleDragToPlayAction);
        Options->addAction(optionsAnimate_playsAction);
        Help->addAction(helpHelpAction);
        Help->addSeparator();
        Help->addAction(helpMouseBindingsAction);
        Help->addAction(helpAboutAction);

        retranslateUi(DvonnWindow);

        QMetaObject::connectSlotsByName(DvonnWindow);
    } // setupUi

    void retranslateUi(QMainWindow *DvonnWindow)
    {
        DvonnWindow->setWindowTitle(QApplication::translate("DvonnWindow", "Dvonn", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setText(QApplication::translate("DvonnWindow", "&Load game", 0, QApplication::UnicodeUTF8));
        fileOpenAction->setIconText(QApplication::translate("DvonnWindow", "Load game", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        fileOpenAction->setToolTip(QApplication::translate("DvonnWindow", "Load a saved game", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        fileOpenAction->setShortcut(QApplication::translate("DvonnWindow", "Ctrl+O", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setText(QApplication::translate("DvonnWindow", "&Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setIconText(QApplication::translate("DvonnWindow", "Save", 0, QApplication::UnicodeUTF8));
        fileSaveAction->setShortcut(QApplication::translate("DvonnWindow", "Ctrl+S", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setText(QApplication::translate("DvonnWindow", "Save &As...", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setIconText(QApplication::translate("DvonnWindow", "Save As", 0, QApplication::UnicodeUTF8));
        fileSaveAsAction->setShortcut(QString());
        filePrintAction->setText(QApplication::translate("DvonnWindow", "&Print...", 0, QApplication::UnicodeUTF8));
        filePrintAction->setIconText(QApplication::translate("DvonnWindow", "Print", 0, QApplication::UnicodeUTF8));
        filePrintAction->setShortcut(QString());
        fileExitAction->setText(QApplication::translate("DvonnWindow", "E&xit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setIconText(QApplication::translate("DvonnWindow", "Exit", 0, QApplication::UnicodeUTF8));
        fileExitAction->setShortcut(QApplication::translate("DvonnWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        gameUndoAction->setText(QApplication::translate("DvonnWindow", "&Undo", 0, QApplication::UnicodeUTF8));
        gameUndoAction->setIconText(QApplication::translate("DvonnWindow", "Undo", 0, QApplication::UnicodeUTF8));
        gameUndoAction->setShortcut(QApplication::translate("DvonnWindow", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        gameRedoAction->setText(QApplication::translate("DvonnWindow", "&Redo", 0, QApplication::UnicodeUTF8));
        gameRedoAction->setIconText(QApplication::translate("DvonnWindow", "Redo", 0, QApplication::UnicodeUTF8));
        gameRedoAction->setShortcut(QApplication::translate("DvonnWindow", "Ctrl+Y", 0, QApplication::UnicodeUTF8));
        helpHelpAction->setText(QApplication::translate("DvonnWindow", "Rules of Dvonn", 0, QApplication::UnicodeUTF8));
        helpHelpAction->setIconText(QApplication::translate("DvonnWindow", "Rules of Dvonn", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setText(QApplication::translate("DvonnWindow", "&About...", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setIconText(QApplication::translate("DvonnWindow", "About", 0, QApplication::UnicodeUTF8));
        helpAboutAction->setShortcut(QString());
        gameNewGameAction->setText(QApplication::translate("DvonnWindow", "&New Game", 0, QApplication::UnicodeUTF8));
        gameNewGameAction->setIconText(QApplication::translate("DvonnWindow", "New Game", 0, QApplication::UnicodeUTF8));
        gameNewGameAction->setShortcut(QApplication::translate("DvonnWindow", "Ctrl+N", 0, QApplication::UnicodeUTF8));
        displayTogglePossDestAction->setText(QApplication::translate("DvonnWindow", "Show &possible plays", 0, QApplication::UnicodeUTF8));
        displayTogglePossDestAction->setIconText(QApplication::translate("DvonnWindow", "Show &possible plays", 0, QApplication::UnicodeUTF8));
        displayToggleTexturesAction->setText(QApplication::translate("DvonnWindow", "Use &textures", 0, QApplication::UnicodeUTF8));
        displayToggleTexturesAction->setIconText(QApplication::translate("DvonnWindow", "Use textures", 0, QApplication::UnicodeUTF8));
        displayToggleLightsAction->setText(QApplication::translate("DvonnWindow", "&Spot light", 0, QApplication::UnicodeUTF8));
        displayToggleLightsAction->setIconText(QApplication::translate("DvonnWindow", "Spot light", 0, QApplication::UnicodeUTF8));
        gameRandomlyFinishAction->setText(QApplication::translate("DvonnWindow", "Randomly &finish placement", 0, QApplication::UnicodeUTF8));
        gameRandomlyFinishAction->setIconText(QApplication::translate("DvonnWindow", "Randomly &finish placement", 0, QApplication::UnicodeUTF8));
        gameRandomlyFinishAction->setShortcut(QApplication::translate("DvonnWindow", "R", 0, QApplication::UnicodeUTF8));
        displayToggleDragToPlayAction->setText(QApplication::translate("DvonnWindow", "&Drag to play", 0, QApplication::UnicodeUTF8));
        displayToggleDragToPlayAction->setIconText(QApplication::translate("DvonnWindow", "Drag to play", 0, QApplication::UnicodeUTF8));
        displayToggleShowLabelsAction->setText(QApplication::translate("DvonnWindow", "Show &Labels", 0, QApplication::UnicodeUTF8));
        displayToggleShowLabelsAction->setIconText(QApplication::translate("DvonnWindow", "Show Labels", 0, QApplication::UnicodeUTF8));
        gameRandomlyMoveStackAction->setText(QApplication::translate("DvonnWindow", "Rand&omly move Stack", 0, QApplication::UnicodeUTF8));
        gameRandomlyMoveStackAction->setIconText(QApplication::translate("DvonnWindow", "Randomly move Stack", 0, QApplication::UnicodeUTF8));
        gameRandomlyMoveStackAction->setShortcut(QApplication::translate("DvonnWindow", "Shift+R", 0, QApplication::UnicodeUTF8));
        displayToggleShowAnimationAction->setText(QApplication::translate("DvonnWindow", "Show an&imation", 0, QApplication::UnicodeUTF8));
        displayToggleShowAnimationAction->setIconText(QApplication::translate("DvonnWindow", "Show animation", 0, QApplication::UnicodeUTF8));
        helpMouseBindingsAction->setText(QApplication::translate("DvonnWindow", "&Mouse bindings", 0, QApplication::UnicodeUTF8));
        helpMouseBindingsAction->setIconText(QApplication::translate("DvonnWindow", "&Mouse bindings", 0, QApplication::UnicodeUTF8));
        optionsAnimate_playsAction->setText(QApplication::translate("DvonnWindow", "Animate plays", 0, QApplication::UnicodeUTF8));
        optionsAnimate_playsAction->setIconText(QApplication::translate("DvonnWindow", "Animate plays", 0, QApplication::UnicodeUTF8));
        easyAction->setText(QApplication::translate("DvonnWindow", "&Easy", 0, QApplication::UnicodeUTF8));
        easyAction->setIconText(QApplication::translate("DvonnWindow", "Easy", 0, QApplication::UnicodeUTF8));
        averageAction->setText(QApplication::translate("DvonnWindow", "&Average", 0, QApplication::UnicodeUTF8));
        averageAction->setIconText(QApplication::translate("DvonnWindow", "&Average", 0, QApplication::UnicodeUTF8));
        difficultAction->setText(QApplication::translate("DvonnWindow", "&Difficult", 0, QApplication::UnicodeUTF8));
        difficultAction->setIconText(QApplication::translate("DvonnWindow", "Difficult", 0, QApplication::UnicodeUTF8));
        File->setTitle(QApplication::translate("DvonnWindow", "&File", 0, QApplication::UnicodeUTF8));
        Game->setTitle(QApplication::translate("DvonnWindow", "&Game", 0, QApplication::UnicodeUTF8));
        Options->setTitle(QApplication::translate("DvonnWindow", "Options", 0, QApplication::UnicodeUTF8));
        Help->setTitle(QApplication::translate("DvonnWindow", "&Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DvonnWindow: public Ui_DvonnWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DVONNWINDOW_H
