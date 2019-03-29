#pragma once
#include "GraphicComposite.h"
#include "Layer.h"


class Network :
	public GraphicComposite<Layer>
{
	Q_OBJECT
public:
	Network(QGraphicsScene &scene, GraphicComponent *parent) : GraphicComposite(scene,parent){};
	~Network(){}
	void addComponent(Layer* layer);
	void updateInOutNeuronsPositions();
	void setPaintLayersMenu(bool paint);
	GraphicComposite<>* getComposite() const override { return (GraphicComposite<>*)this; };
	void paintComposite(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override {};
	QRectF boundingRect() const override { return QRect(0,0, getNetworkWidth(), getNetworkHeight()); };
	int layersCount() const { return components.size(); }
	qreal getNetworkWidth() const { 
		return (layerGap + Neuron::width)*components.size(); 
	}
	qreal getNetworkHeight() const;
private:
	qreal layerGap = 200;
	bool isPaintingLayerMenu = false;
};

