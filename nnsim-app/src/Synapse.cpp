#include "Synapse.h"
#include "QPoint"
#include "Neuron.h"

Synapse::Synapse(QGraphicsScene &scene, Neuron* startNeuron, Neuron* parent) : GraphicComponent(scene, parent), n1(startNeuron), n2(parent)
{
	pen = QPen(QColor("black"),1);
	setPos(0, 0);
	setFlag(ItemIsMovable, false);
}

QRectF Synapse::boundingRect() const
{
	return QRectF(n1->x(), fmin(n1->y(), n2->y()), abs(n1->x() - n2->x()), abs(n1->y() - n2->y()));
}

void Synapse::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	painter->setPen(pen);
	painter->drawLine(QPoint(n1->x() + n1->width / 2, n1->y() + n1->height / 2 + n1->textAreaHeight),
		QPoint(n2->x() + n2->width / 2, n2->y() + n2->height / 2 + n2->textAreaHeight));
}

void Synapse::setColor(QColor color)
{
	this->pen.setColor(color);
	update();
}

QColor Synapse::getColor()
{
	return this->pen.color();
}

qreal Synapse::getWidth()
{
	return this->pen.widthF();
}

void Synapse::setWidth(qreal width)
{
	this->pen.setWidthF(width);
	update();
}
