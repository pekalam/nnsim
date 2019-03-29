#include "Layer.h"
#include "Neuron.h"
#include "Network.h"
#include "FFNetworkLayer.h"

qreal Layer::neuronGap = 40;
qreal Layer::menuWidth = 100;
qreal Layer::menuHeight = 200;

Layer::Layer(FFNetworkLayer *backingLayer, QGraphicsScene &scene, Network *parent) : GraphicComposite(scene, parent), pen(QColor("black"), 3), backingLayer(backingLayer)
{
	
}

void Layer::addComponent(Neuron* neuron)
{
	neuron->setPos(x(), components.size() * (Neuron::height + neuronGap));
	layerHeight += (Neuron::height + neuronGap);
	GraphicComposite<Neuron>::addComponent(neuron);
}

int Layer::neuronsCount() const
{
	return  backingLayer->getNeuronsCount();
}

void Layer::setPaintLayerMenu(bool paint)
{
	paintMenu = paint;
	if (paint)
	{
		for (Neuron* neuron : components)
			neuron->hide();
	}
	else
	{
		for (Neuron* neuron : components)
			neuron->show();
	}
}

void Layer::paintComposite(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	if (paintMenu) {
		painter->setPen(pen);
		painter->drawRect(0, 0, menuWidth, menuHeight);
		painter->setPen(Qt::MPenStyle);
		if(isInputLayer())
			painter->drawText(20, 20, QString("Input layer"));
		else if (isOutputLayer())
			painter->drawText(20, 20, QString("Output layer"));
		else
			painter->drawText(20, 20, QString("Layer %1").arg(index));
		
		painter->drawText(20, 40, QString("neurons: %1").arg(neuronsCount()));
	}
}

void Layer::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseDoubleClickEvent(event);
	emit layerDoubleClick(this);
}


