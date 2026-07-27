#ifndef QGLV12_INTERFACE_DIALOG_H
#define QGLV12_INTERFACE_DIALOG_H

#include <qapplication.h>
#include "ui_viewerInterfacedialog.Qt4.h"
#include "ui_viewerInterfacewidget.Qt4.h"

class qglv12interfaceViewerDialog : public QDialog, public Ui::ViewerInterfaceDialog
{
  public:
    qglv12interfaceViewerDialog();
};

class qglv12interfaceViewerWidget : public QWidget, public Ui::ViewerInterfaceWidget
{
public:
	qglv12interfaceViewerWidget(QWidget *parent=0);
};

#endif


