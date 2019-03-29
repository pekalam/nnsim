#pragma once
#include "Synapse.h"

#include "GraphicComposite.h"

class Layer;

class Neuron : public GraphicComposite<Synapse>
{
	Q_OBJECT
public:
	Neuron(QGraphicsScene &scene, Layer *parent);
	~Neuron() = default;
	QRectF boundingRect() const override;
	void paintComposite(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
	static int width;
	static int height;
	static int textAreaHeight;
	void showOutput();
	void hideOutput();
	void setNeuronOutput(double& neuronOutput) { this->neuronOutput = neuronOutput; }
	double neuronOutput;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event) override;
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event) override;
	Layer* getLayer() { return layer; }
private:
	Layer *layer;
	bool m_showOutput = false;
	QColor nColor;
	QPen nPen;
	QBrush nBrush;
	QBrush activBrush;
signals:
	void neuronDoubleClick(Neuron* neuron);
	void neuronMouseOver(Neuron* neuron);
	void neuronMouseLeave(Neuron* neuron);
};

