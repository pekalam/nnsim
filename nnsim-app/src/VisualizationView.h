#pragma once
#include <QGraphicsView>

class VisualizationView : public QWidget
{
	Q_OBJECT
public:
	VisualizationView(QGraphicsView* graphicsView, QWidget *parent = Q_NULLPTR);
	~VisualizationView();
	QGraphicsView* graphicsView;
	QGraphicsScene scene;
};

