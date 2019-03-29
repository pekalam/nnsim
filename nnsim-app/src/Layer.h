#pragma once

#include "GraphicComposite.h"
#include "Neuron.h"


class Network;
class FFNetworkLayer;

class Layer : public GraphicComposite<Neuron>
{
	Q_OBJECT
public:
	Layer(FFNetworkLayer *backingLayer, QGraphicsScene &scene, Network *parent);
	~Layer() = default;
	QRectF boundingRect() const override{ return QRect(0, 0, getLayerWidth(), getLayerHeight()); }
	void paintComposite(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	void addComponent(Neuron* neuron);
	QPen pen;
	FFNetworkLayer* getBackingLayer() { return backingLayer; }
	static qreal neuronGap;
	static qreal menuWidth;
	static qreal menuHeight;
	int neuronsCount() const;
	bool isInputLayer() const { return index == 0; }
	bool isOutputLayer() const { return index == parent->getComposite()->components.size() - 1; }
	void setPaintLayerMenu(bool paint);
	int getLayerHeight() const { if (paintMenu) return menuHeight; else return 0; }
	int getLayerWidth() const { if (paintMenu) return menuWidth; else return 0; }
private:
	FFNetworkLayer* backingLayer;
	qreal layerWidth = 0;
	qreal layerHeight = 0;
	bool paintMenu = false;
signals:
	void layerDoubleClick(Layer *layer);

};

