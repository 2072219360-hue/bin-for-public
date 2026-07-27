
#include "QtGui"

#include "qwt05plot.h"
#include "qwt05friedberg2007.h"
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_legend.h>
#include <qwt_interval_symbol.h>
#include <qwt_symbol.h>
#include <qwt_series_data.h>
#include <qwt_text.h>
#include <qwt_scale_draw.h>
#include <qwt_plot_renderer.h>
#include <qdatetime.h>
#include <qfiledialog.h>
#include <qimagewriter.h>
#include <qprintdialog.h>
#include <qfileinfo.h>

#include "qwt05plotDialogDemo.h"


PlotDialogDemo::PlotDialogDemo(QWidget *parent):QWidget(parent)
{
	d_plot = new QWT05Plot( this );
	QVBoxLayout *layout = new QVBoxLayout(this);

	layout->addWidget( d_plot );

	QToolBar *toolBar = new QToolBar( this );

	QComboBox *typeBox = new QComboBox( toolBar );
	typeBox->addItem( "Bars" );
	typeBox->addItem( "Tube" );
	typeBox->setCurrentIndex( 1 );
	typeBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );

	QToolButton *btnExport = new QToolButton( toolBar );
	btnExport->setText( "Export" );
	btnExport->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
	connect( btnExport, SIGNAL( clicked() ), d_plot, SLOT( exportPlot() ) );

	toolBar->addWidget( typeBox );
	toolBar->addWidget( btnExport );
	layout->addWidget( toolBar );

	d_plot->setMode( typeBox->currentIndex() );
	connect( typeBox, SIGNAL( currentIndexChanged( int ) ),
		d_plot, SLOT( setMode( int ) ) );

}