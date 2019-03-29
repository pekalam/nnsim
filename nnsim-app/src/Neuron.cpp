#include "Neuron.h"
#include "Layer.h"
#include "FFNetworkLayer.h"

int Neuron::width = 60;
int Neuron::height = 60;
int Neuron::textAreaHeight = 25;

Neuron::Neuron(QGraphicsScene &scene, Layer *parent) :  GraphicComposite(scene, parent), neuronOutput(0),
	nColor("green"), layer(parent), activBrush(Qt::red)
{
	nPen = QPen(nColor, 0);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges);
	setAcceptHoverEvents(true);
}

QRectF Neuron::boundingRect() const
{
	return QRectF(0, 0, this->width, this->height + textAreaHeight);
}



void Neuron::paintComposite(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	painter->setPen(nPen);
	painter->setBrush(Qt::green);
	painter->drawEllipse(0,textAreaHeight, this->width, this->height);
	painter->setPen(QPen(Qt::black, 1.5));
	painter->drawText(width/2, this->height + textAreaHeight - 25, QString::fromStdString(layer->getBackingLayer()->getActivationFunctionName()));
	painter->setPen(Qt::MPenStyle);
	if(m_showOutput)
		painter->drawText(0, textAreaHeight, QString("%1").arg(neuronOutput));
}

void Neuron::showOutput()
{
	m_showOutput = true;
}

void Neuron::hideOutput()
{
	m_showOutput = false;
}

void Neuron::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	QGraphicsItem::mouseDoubleClickEvent(event);
	emit neuronDoubleClick(this);
}

void Neuron::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	QGraphicsItem::hoverEnterEvent(event);
	emit neuronMouseOver(this);
}

void Neuron::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	QGraphicsItem::hoverLeaveEvent(event);
	emit neuronMouseLeave(this);
}




