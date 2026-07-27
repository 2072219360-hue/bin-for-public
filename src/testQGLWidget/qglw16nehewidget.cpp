#include <qimage.h>

#include "QtGui"
#include "qglw16nehewidget.h"
#include "gl/GLU.h"

GLfloat lightAmbient[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat lightDiffuse[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat lightPosition[4] = { 0.0, 0.0, 2.0, 1.0 };

GLuint fogMode[3] = { GL_EXP, GL_EXP2, GL_LINEAR };
GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 };

NeHe16Widget::NeHe16Widget( QWidget* parent, bool fs )
    : QGLWidget( parent)
{
  this->setFocusPolicy(Qt:: StrongFocus); 
  xRot = yRot = zRot = 0.0;
  zoom = -5.0;
  xSpeed = ySpeed = 0.0;

  filter = 0;

  light = false;

  fogFilter = 0;

  fullscreen = fs;
  setGeometry( 100, 100, 640, 480 );
  setWindowTitle( "Chris Aliotta & NeHe's Fog Tutorial" );

  if ( fullscreen )
    showFullScreen();
}

NeHe16Widget::~NeHe16Widget()
{
}

void NeHe16Widget::initializeGL()
{
  loadGLTextures();

  glEnable( GL_TEXTURE_2D );
  glShadeModel( GL_SMOOTH );
  glClearColor( 0.5, 0.5, 0.5, 1.0 );
  glClearDepth( 1.0 );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

  glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient );
  glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse );
  glLightfv( GL_LIGHT1, GL_POSITION, lightPosition );

  glEnable( GL_LIGHT1 );

  glFogi( GL_FOG_MODE, fogMode[fogFilter] );
  glFogfv( GL_FOG_COLOR, fogColor );
  glFogf( GL_FOG_DENSITY, 0.35 );
  glHint( GL_FOG_HINT, GL_DONT_CARE );
  glFogf( GL_FOG_START, 1.0 );
  glFogf( GL_FOG_END, 5.0 );
  glEnable( GL_FOG );
}

void NeHe16Widget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity();
  glTranslatef(  0.0,  0.0, zoom );

  glRotatef( xRot,  1.0,  0.0,  0.0 );
  glRotatef( yRot,  0.0,  1.0,  0.0 );

  glBindTexture( GL_TEXTURE_2D, texture[filter] );

  glBegin( GL_QUADS );
    glNormal3f(  0.0,  0.0, 1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );

    glNormal3f(  0.0,  0.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

    glNormal3f(  0.0,  1.0,  0.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

    glNormal3f(  0.0, -1.0,  0.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

    glNormal3f(  1.0,  0.0,  0.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

    glNormal3f( -1.0,  0.0,  0.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
  glEnd();

  xRot += xSpeed;
  yRot += ySpeed;
}

void NeHe16Widget::resizeGL( int width, int height )
{
  if ( height == 0 )
  {
    height = 1;
  }
  glViewport( 0, 0, (GLint)width, (GLint)height );
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
}

void NeHe16Widget::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {
  case Qt::Key_L:
    light = !light;
    if ( !light )
    {
      glDisable( GL_LIGHTING );
    }
    else
    {
      glEnable( GL_LIGHTING );
    }
    updateGL();
    break;
  case Qt::Key_F:
    filter += 100;;
    if ( filter > 101 )
    {
      filter = 0;
    }
    updateGL();
    break;
  case Qt::Key_G:
    fogFilter += 1;;
    if ( fogFilter > 2 )
    {
      fogFilter = 0;
    }
    glFogi( GL_FOG_MODE, fogMode[fogFilter] );
    updateGL();
    break;
  /*case Qt::Key_Prior:
    zoom -= 0.2;
    updateGL();
    break;
  case Qt::Key_Next:
    zoom += 0.2;
    updateGL();
    break;*/
  case Qt::Key_Up:
    xSpeed -= 0.01;
    updateGL();
    break;
  case Qt::Key_Down:
    xSpeed += 0.01;
    updateGL();
    break;
  case Qt::Key_Right:
    ySpeed += 0.01;
    updateGL();
    break;
  case Qt::Key_Left:
    ySpeed -= 0.01;
    updateGL();
    break;
  case Qt::Key_F2:
    fullscreen = !fullscreen;
    if ( fullscreen )
    {
      showFullScreen();
    }
    else
    {
      showNormal();
      setGeometry( 100, 100, 640, 480 );
    }
    update();
    break;
  case Qt::Key_Escape:
    close();
  }
}

void NeHe16Widget::loadGLTextures()
{
  QImage tex, buf;
  if ( !buf.load( "data/Crate.bmp" ) )
  {
    qWarning( "Could not read image file, using single-color instead." );
    QImage dummy( 128, 128,QImage::Format_RGB32);
    dummy.fill(8);//Qt::green.rgb()
    buf = dummy;
  }
  tex = QGLWidget::convertToGLFormat( buf );

  glGenTextures( 3, &texture[0] );

  glBindTexture( GL_TEXTURE_2D, texture[0] );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );

  glBindTexture( GL_TEXTURE_2D, texture[1] );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );

  glBindTexture( GL_TEXTURE_2D, texture[2] );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGB, tex.width(), tex.height(),
      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );

}

NeHe16Dialog::NeHe16Dialog()
{
	nh=new NeHe16Widget();
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	mainLayout->addWidget(nh);
	this->resize(400,400);
	this->setLayout(mainLayout);
}