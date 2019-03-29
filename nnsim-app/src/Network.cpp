#include "Network.h"


void Network::addComponent(Layer* layer)
{
	layer->setPos(x() + (components.size() * (layerGap + Neuron::width)), y());
	GraphicComposite<Layer>::addComponent(layer);
}

void Network::updateInOutNeuronsPositions()
{
	Layer* maxNLayer = nullptr;
	for (Layer* layer : components)
		if (maxNLayer == nullptr || layer->neuronsCount() > maxNLayer->neuronsCount())
			maxNLayer = layer;
	qreal maxLayerH = maxNLayer->neuronsCount()*Neuron::height + (maxNLayer->neuronsCount()-1)*Layer::neuronGap;

	
	for (Layer* layer : components)
		if (layer != maxNLayer) {
			int n = 0;
			qreal layerH = layer->neuronsCount()*Neuron::height + (layer->neuronsCount() - 1)*Layer::neuronGap;
			for (Neuron* neuron : layer->components)
			{
				neuron->setPos(neuron->pos().x(), neuron->pos().y() + (maxLayerH-layerH)/2);
				n++;
			}
		}
}

void Network::setPaintLayersMenu(bool paint)
{
	isPaintingLayerMenu = paint;
	for (Layer* layer : components)
	{
		layer->setPaintLayerMenu(paint);
	}
}

qreal Network::getNetworkHeight() const
{
	int m = 0;
	for (Layer* layer : components)
	{
		if (layer->neuronsCount() > m)
			m = layer->neuronsCount();
	}
	return m * (Layer::neuronGap + Neuron::height);
}
