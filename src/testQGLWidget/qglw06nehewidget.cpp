#include "gl/glew.h"

#include "qglw06nehewidget.h"
#include <qimage.h>
#include <QKeyEvent>
NeHe06Widget::NeHe06Widget( QWidget* parent, bool fs )
    : QGLWidget( parent )
{
  this->setFocusPolicy(Qt:: StrongFocus); 
  xRot = yRot = zRot = 0.0;
  fullscreen = fs;
  setGeometry(100, 100, 640, 480 );
  setWindowTitle( "NeHe's Texture Mapping Tutorial" );

  if ( fullscreen )
    showFullScreen();
}

NeHe06Widget::~NeHe06Widget()
{
}

void NeHe06Widget::paintGL()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );//清除界面and清除深度缓存
  glLoadIdentity();//重新定位到0.0点；X坐标轴从左至右，Y坐标轴从下至上，Z坐标轴从里至外。
  glTranslatef(  0.0,  0.0, -5.0 );//移动到深度为5的位置

  glRotatef( xRot,  1.0,  0.0,  0.0 );
  glRotatef( yRot,  0.0,  1.0,  0.0 );
  glRotatef( zRot,  0.0,  0.0,  1.0 );

  glBindTexture( GL_TEXTURE_2D, texture[0] );

  glBegin( GL_QUADS );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );

    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );

    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );

    glTexCoord2f( 1.0, 0.0 ); glVertex3f(  1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f(  1.0,  1.0, -1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f(  1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 0.0 ); glVertex3f(  1.0, -1.0,  1.0 );

    glTexCoord2f( 0.0, 0.0 ); glVertex3f( -1.0, -1.0, -1.0 );
    glTexCoord2f( 1.0, 0.0 ); glVertex3f( -1.0, -1.0,  1.0 );
    glTexCoord2f( 1.0, 1.0 ); glVertex3f( -1.0,  1.0,  1.0 );
    glTexCoord2f( 0.0, 1.0 ); glVertex3f( -1.0,  1.0, -1.0 );
  glEnd();

  xRot += 0.3;
  yRot += 0.1;
  zRot += 0.4;
}

void NeHe06Widget::initializeGL()
{
  loadGLTextures();

  glEnable( GL_TEXTURE_2D );
  glShadeModel( GL_SMOOTH );
  glClearColor( 0.0, 0.0, 0.0, 0.5 );
  glClearDepth( 1.0 );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );//所作深度测试的类型 接着告诉OpenGL我们希望进行最好的透视修正。这会十分轻微的影响性能。但使得透视图看起来好一点。
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );// 真正精细的透视修正
}

void NeHe06Widget::resizeGL( int width, int height )
{
  if ( height == 0 )
  {
    height = 1;
  }
  glViewport( 0, 0, (GLint)width, (GLint)height );
  glMatrixMode( GL_PROJECTION );//选择投影矩阵
  glLoadIdentity();// 重置投影矩阵
  gluPerspective( 45.0, (GLfloat)width/(GLfloat)height, 0.1, 100.0 );// 计算窗口的外观比例
  /*
   GLdouble fovy, //角度
　　GLdouble aspect,//视景体的宽高比
　　GLdouble zNear,//沿z轴方向的两裁面之间的距离的近处
　　GLdouble zFar //沿z轴方向的两裁面之间的距离的远处
    */
  glMatrixMode( GL_MODELVIEW );// 选择模型观察矩阵
  glLoadIdentity();// 重置模型观察矩阵
}

void NeHe06Widget::loadGLTextures()
{
  QImage tex, buf;
  if ( !buf.load( "data/NeHe.bmp" ) )
  {
    qWarning( "Could not read image file, using single-color instead." );
    QImage dummy( 128, 128, QImage::Format_RGB32);
   // dummy.fill(8 );
    buf = dummy;
  }
  tex = QGLWidget::convertToGLFormat( buf );

  glGenTextures( 1, &texture[0] );//glGenTextures函数根据纹理参数返回n个纹理索引。纹理名称集合不必是一个连续的整数集合。
  /*
    glGenTextures(GLsizei n, GLuint *textures)函数说明
    n:用来生成纹理的数量
    textures:存储纹理索引的
    （glGenTextures就是用来产生你要操作的纹理对象的索引的，比如你告诉OpenGL，我需要5个纹理对象，它会从没有用到的整数里返回5个给你）

    */

  glBindTexture( GL_TEXTURE_2D, texture[0] );

  /*
    glBindTexture实际上是改变了OpenGL的这个状态，它告诉OpenGL下面对纹理的任何操作都是对它所绑定的纹理对象的，
    比如 glBindTexture(GL_TEXTURE_2D,1)告诉OpenGL下面代码中对2D纹理的任何设置都是针对索引为1的纹理的。
    */
  glTexImage2D( GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0,
      GL_RGBA, GL_UNSIGNED_BYTE, tex.bits() );
  /*
    glTexImage2D(GL_TEXTURE_2D, //此纹理是一个2D纹理
    0,                                         //代表图像的详细程度, 默认为0即可
    3,                                         //颜色成分R(红色分量)、G(绿色分量)、B(蓝色分量)三部分，若为4则是R(红

                                         //色分量)、G(绿色分量)、B(蓝色分量)、Alpha
    TextureImage[0]->sizeX,          //纹理的宽度
    TextureImage[0]->sizeY,          //纹理的高度
    0,                                         //边框的值
    GL_RGB,                               //告诉OpenGL图像数据由红、绿、蓝三色数据组成
    GL_UNSIGNED_BYTE,                //组成图像的数据是无符号字节类型
    TextureImage[0]->data);          //告诉OpenGL纹理数据的来源,此例中指向存放在TextureImage[0]记录中的数据
    */

  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  /*
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        GL_TEXTURE_MIN_FILTER: 缩小过滤
        GL_LINEAR_MIPMAP_NEAREST: 使用GL_NEAREST对最接近当前多边形的解析度的两个层级贴图进行采样,然后用这两个值进行线性插值.
    */
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  /*
    lTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    这是纹理过滤
    GL_TEXTURE_MAG_FILTER: 放大过滤
    GL_LINEAR: 线性过滤, 使用距离当前渲染像素中心最近的4个纹素加权平均值.

    */

}

void NeHe06Widget::keyPressEvent( QKeyEvent *e )
{
  switch ( e->key() )
  {

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
      setGeometry( 0, 0, 640, 480 );
    }
    update();
    break;
  case Qt::Key_Escape:
    close();
  }
}



NeHe06Dialog::NeHe06Dialog()
{
	nh=new NeHe06Widget();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
	mainLayout->addWidget(nh);
	this->resize(400,400);
	this->setLayout(mainLayout);
}