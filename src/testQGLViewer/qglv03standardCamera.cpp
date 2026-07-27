/****************************************************************************

 Copyright (C) 2002-2011 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.3.17.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "qglv03standardCamera.h"

#include "testQGLViewer/qglv03cameraViewer.h"
#include "testQGLViewer/qglv03viewer.h"

#if QT_VERSION >= 0x040000
# include <QWheelEvent>
#endif

using namespace qglviewer;

qglv03StandardCamera::qglv03StandardCamera()
{
  standard = true;
  orthoSize = 1.0;
}

float qglv03StandardCamera::zNear() const
{ 
  if (standard) 
    return 0.001f; 
  else 
    return Camera::zNear(); 
}

float qglv03StandardCamera::zFar() const
{  
  if (standard) 
    return 1000.0; 
  else 
    return Camera::zFar();
}

void qglv03StandardCamera::changeOrthoFrustumSize(int delta)
{
  if (delta > 0)
    orthoSize *= 1.1f;
  else
    orthoSize /= 1.1f;
}

void qglv03StandardCamera::getOrthoWidthHeight(GLdouble &halfWidth, GLdouble &halfHeight) const
{
  if (standard)
  {
    halfHeight = orthoSize;
    halfWidth = aspectRatio() * orthoSize;
  }
  else
    Camera::getOrthoWidthHeight(halfWidth, halfHeight);
}

qglv03StandardCameraDialog::qglv03StandardCameraDialog()
{
	// Instantiate the two viewers.
	qglv03StandardCamera* sc = new qglv03StandardCamera();

	qglv03Viewer *viewer=new qglv03Viewer(sc,this);
	qglv03CameraViewer *cviewer=new qglv03CameraViewer(sc,this);

	// Make sure every v camera movement updates the camera viewer
	QObject::connect(viewer->camera()->frame(), SIGNAL(manipulated()), cviewer, SLOT(updateGL()));
	QObject::connect(viewer->camera()->frame(), SIGNAL(spun()), cviewer, SLOT(updateGL()));
	// Also update on camera change (type or mode)
	QObject::connect(viewer, SIGNAL(cameraChanged()), cviewer, SLOT(updateGL()));

#if QT_VERSION < 0x040000
	application.setMainWidget(&viewer);
#else
	viewer->setWindowTitle("standardCamera");
	cviewer->setWindowTitle("Camera viewer");
#endif

	cviewer->show();
	viewer->show();

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	mainLayout->addWidget(viewer);
	mainLayout->addWidget(cviewer);

	setLayout(mainLayout);
	

}

qglv03StandardCameraWidget::qglv03StandardCameraWidget(QWidget *parent):QWidget(parent)
{
	// Instantiate the two viewers.
	qglv03StandardCamera* sc = new qglv03StandardCamera();

	qglv03Viewer *viewer=new qglv03Viewer(sc,this);
	qglv03CameraViewer *cviewer=new qglv03CameraViewer(sc,this);

	// Make sure every v camera movement updates the camera viewer
	QObject::connect(viewer->camera()->frame(), SIGNAL(manipulated()), cviewer, SLOT(updateGL()));
	QObject::connect(viewer->camera()->frame(), SIGNAL(spun()), cviewer, SLOT(updateGL()));
	// Also update on camera change (type or mode)
	QObject::connect(viewer, SIGNAL(cameraChanged()), cviewer, SLOT(updateGL()));

#if QT_VERSION < 0x040000
	application.setMainWidget(&viewer);
#else
	viewer->setWindowTitle("standardCamera");
	cviewer->setWindowTitle("Camera viewer");
#endif

	//cviewer.show();
	//viewer.show();

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	mainLayout->addWidget(viewer);
	mainLayout->addWidget(cviewer);

	setLayout(mainLayout);
}