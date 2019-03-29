#include "VisualizationView.h"
#include <qmath.h>
//#include <QtOpenGL/QtOpenGL>

VisualizationView::VisualizationView(QGraphicsView* graphicsView, QWidget *parent) : QWidget(parent), graphicsView(graphicsView), scene(graphicsView)
{
	//scene.setSceneRect(0, 0, 10000, 10000);
	graphicsView->setScene(&scene);
	/* qreal scale = qPow(qreal(2), (250 - 250) / qreal(50));

	QMatrix matrix;
	matrix.scale(scale, scale);
	matrix.rotate(0);

	//graphicsView->setMatrix(matrix);
	graphicsView->setInteractive(true);
	graphicsView->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
	//graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	scene.update();*/
}


VisualizationView::~VisualizationView()
{
}
