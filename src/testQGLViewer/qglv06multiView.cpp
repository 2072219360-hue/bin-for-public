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

#include "qglv06multiView.h"

using namespace qglviewer;
using namespace std;

qglv06multiViewer::qglv06multiViewer(const qglv06Scene* const s, int type, QWidget* parent, const QGLWidget* shareWidget)
#if QT_VERSION < 0x040000
  : QGLViewer(parent, "viewer", shareWidget), scene_(s)
#else
  : QGLViewer(parent, shareWidget), scene_(s)
#endif
{
  setAxisIsDrawn();
  setGridIsDrawn();

  if (type < 3)
    {
      // Move camera according to viewer type (on X, Y or Z axis)
      camera()->setPosition(Vec((type==0)? 1.0 : 0.0, (type==1)? 1.0 : 0.0, (type==2)? 1.0 : 0.0));
      camera()->lookAt(sceneCenter());

      camera()->setType(Camera::ORTHOGRAPHIC);
      camera()->showEntireScene();

      // Forbid rotation
      WorldConstraint* constraint = new WorldConstraint();
      constraint->setRotationConstraintType(AxisPlaneConstraint::FORBIDDEN);
      camera()->frame()->setConstraint(constraint);
    }

  restoreStateFromFile();
}

void qglv06multiViewer::draw()
{
  scene_->draw();
}

// Draws a spiral
void qglv06Scene::draw() const
{
  const float nbSteps = 200.0;
  glBegin(GL_QUAD_STRIP);
  for (float i=0; i<nbSteps; ++i)
    {
      float ratio = i/nbSteps;
      float angle = 21.0*ratio;
      float c = cos(angle);
      float s = sin(angle);
      float r1 = 1.0 - 0.8f*ratio;
      float r2 = 0.8f - 0.8f*ratio;
      float alt = ratio - 0.5f;
      const float nor = 0.5f;
      const float up = sqrt(1.0-nor*nor);
      glColor3f(1.0-ratio, 0.2f , ratio);
      glNormal3f(nor*c, up, nor*s);
      glVertex3f(r1*c, alt, r1*s);
      glVertex3f(r2*c, alt+0.05f, r2*s);
    }
  glEnd();
}

qglv06multiViewerDialog::qglv06multiViewerDialog()
{
	// Create the scene
	qglv06Scene* s = new qglv06Scene();

	//创建主分割窗口 设置方向为水平分割窗口   
	QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, 0);  
	//创建一个QTextEdit控件 设置其父控件为mainSplitter   
	//创建右分割窗口 设置其父控件为mainSplitter 设置方向为垂直分割窗口 最后其分割成上下两个窗口
	QSplitter *leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
	// Instantiate the viewers.
	qglv06multiViewer *side=new qglv06multiViewer(s,0,leftSplitter);

	qglv06multiViewer *top=new qglv06multiViewer(s,1,leftSplitter, side);


	//创建右分割窗口 设置其父控件为mainSplitter 设置方向为垂直分割窗口 最后其分割成上下两个窗口
	QSplitter *rightSplitter = new QSplitter(Qt::Vertical, mainSplitter);
	//设定分割窗口的分割条在拖动时是否为实时更新 true为实时更新 false则在拖动时只显示一条灰色的线条
	//在拖动到位并弹起鼠标后再显示分隔条
	rightSplitter->setOpaqueResize(false);
	//开始将其分割成上下两个窗口

	qglv06multiViewer *front = new qglv06multiViewer (s,2,rightSplitter, side);
	qglv06multiViewer *persp = new qglv06multiViewer (s,3,rightSplitter, side);

	mainSplitter->setStretchFactor(1, 1);  
	mainSplitter->setWindowTitle(QObject::tr("分割窗口"));  
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	mainLayout->addWidget(mainSplitter);

}

qglv06multiViewerWidget::qglv06multiViewerWidget(QWidget *parent):QWidget(parent)
{
	// Create the scene
	qglv06Scene* s = new qglv06Scene();

	//创建主分割窗口 设置方向为水平分割窗口   
	QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, 0);  
	//创建一个QTextEdit控件 设置其父控件为mainSplitter   
	//创建右分割窗口 设置其父控件为mainSplitter 设置方向为垂直分割窗口 最后其分割成上下两个窗口
	QSplitter *leftSplitter = new QSplitter(Qt::Vertical, mainSplitter);
	// Instantiate the viewers.
	qglv06multiViewer *side=new qglv06multiViewer(s,0,leftSplitter);

	qglv06multiViewer *top=new qglv06multiViewer(s,1,leftSplitter, side);


	//创建右分割窗口 设置其父控件为mainSplitter 设置方向为垂直分割窗口 最后其分割成上下两个窗口
	QSplitter *rightSplitter = new QSplitter(Qt::Vertical, mainSplitter);
	//设定分割窗口的分割条在拖动时是否为实时更新 true为实时更新 false则在拖动时只显示一条灰色的线条
	//在拖动到位并弹起鼠标后再显示分隔条
	rightSplitter->setOpaqueResize(false);
	//开始将其分割成上下两个窗口

	qglv06multiViewer *front = new qglv06multiViewer (s,2,rightSplitter, side);
	qglv06multiViewer *persp = new qglv06multiViewer (s,3,rightSplitter, side);

	mainSplitter->setStretchFactor(1, 1);  
	mainSplitter->setWindowTitle(QObject::tr("分割窗口"));  
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	mainLayout->addWidget(mainSplitter);
}