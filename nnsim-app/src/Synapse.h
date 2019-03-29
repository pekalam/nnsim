#pragma once
#include <QColor>
#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include "GraphicComposite.h"

class Neuron;


class Synapse : public GraphicComponent
{
	Q_OBJECT
	Q_PROPERTY(QColor color READ getColor WRITE setColor)
	Q_PROPERTY(qreal width READ getWidth WRITE setWidth)
public:
	Synapse(QGraphicsScene &scene, Neuron* startNeuron, Neuron* parent);
	~Synapse() = default;
	QRectF boundingRect() const override;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	
	void setColor(QColor color);
	QColor getColor();
	qreal getWidth();
	void setWidth(qreal width);
	QPen pen;
private:
	Neuron *n1;
	Neuron *n2;
};

