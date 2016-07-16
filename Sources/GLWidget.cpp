#include "GLWidget.h"
//#include "graphics/grp.h"
//#include "android.h"

//using namespace grp;

//Camera* _camModel;
//Camera* _camUI;
//Drawable* _android;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{

}

GLWidget::~GLWidget()
{
    QOpenGLWidget::makeCurrent();

//    delete _android;
//    delete _camModel;
//    delete _camUI;

    QOpenGLWidget::doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    //initShaders();
   /*
    _camModel = new Camera();
    _camUI = new Camera();


    _android = new Drawable(1, g_num_vertex_android);
    _android->setVertices((Vector3*) g_vetices_android);
    _android->setCoordArray((Vector2*) g_tex_coords_android);
    _android->setPosition(0, -70, -350);
    _android->setColor(0.4f, 0.8f, 0.0f, 1.0f);
    _android->setDispMode(DISP_LINE);


    //_android->setTexture();
*/
    _timer.start(12, this);
}

void GLWidget::resizeGL(int w, int h)
{
    w++;
    h++;
/*
    _camUI->setViewport(0, 0, w, h);
    _camUI->setOrtho(0, w, 0, h, -1, 100);

    _camModel->setViewport(0, 0, w, h);
    _camModel->setPerspective(45, w/h, 0.1f, 500);
*/
//    tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h - 20.0f ), 0 );
//    tls->xmg_text->SetPosition ( XMGVector3F ( 5.0f, wnd.m_h / 5.0f - 20.0f ), 1 );
//    tls->xmg_text->SetLineLength ( wnd.m_w - 5.0f );
}

void GLWidget::paintGL()
{
    /*
    Canvas::getInstance()->clear();

    //_camModel->apply();
    //_android->draw();

    //_camUI->apply();
    //_title->render();

    Canvas::getInstance()->flush();
    */
}

void GLWidget::initShaders()
{
    /*
    // Compile vertex shader
    if (!_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!_program.link())
        close();

    // Bind shader pipeline for use
    if (!_program.bind())
        close();
        */
}

void GLWidget::mousePressEvent(QMouseEvent *)
{

}

void GLWidget::mouseReleaseEvent(QMouseEvent *)
{

}

void GLWidget::timerEvent(QTimerEvent *)
{
    QOpenGLWidget::update();

    /*
    XMGMatrix4F	 mat;

        mat.Rotate ( tls->app_rot, 0.0f, 1.0f, 0.0f );

        tls->xmg_android->SetMatrix ( mat );

        if ( kdGetTimeUST() - tls->app_time > 2 * 1000000000LL )
        {
            tls->app_time  = kdGetTimeUST();
            tls->app_index = tls->app_index == 3 ? 0 : tls->app_index + 1;

            switch ( tls->app_index )
            {
                case 0 :

                    tls->xmg_android->SetDispMode ( XMG_DISP_TEXTURE );
                    tls->xmg_text->SetText ( "DRAWING TEXTURE", 1 );

                    break;

                case 1 :

                    tls->xmg_android->SetDispMode ( XMG_DISP_POINT );
                    tls->xmg_text->SetText ( "DRAWING POINTS", 1 );

                    break;

                case 2 :

                    tls->xmg_android->SetDispMode ( XMG_DISP_LINE );
                    tls->xmg_text->SetText ( "DRAWING LINES", 1 );

                    break;

                case 3 :

                    tls->xmg_android->SetDispMode ( XMG_DISP_FILL );
                    tls->xmg_text->SetText ( "DRAWING FILL", 1 );

                    break;
            }
        }

        tls->app_rot += 2.0f;
        */

}
