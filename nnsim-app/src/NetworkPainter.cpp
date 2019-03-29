#include "NetworkPainter.h"
#include <QGraphicsView>
#include <qthread.h>
#include "FFNeuralNetwork.h"
#include "Network.h"
#include "Layer.h"
#include "Neuron.h"
#include "FFNetworkLayer.h"

NetworkPainter::NetworkPainter(FFNeuralNetwork *network, QObject* parent) : QObject(parent), net(network), paintMode(FULL), networkOnScene(false), scene(new QGraphicsScene(this))
{
	this->network = new Network(*scene, Q_NULLPTR);
	this->network->setPos(0, 0);
}

NetworkPainter::~NetworkPainter()
{
	delete scene;
}

void NetworkPainter::updateNeuronsOutput(int layerNum)
{
	if (layerNum == -1)
		for(Layer* layer : network->components)
			for(Neuron* neuron : layer->components)
			{
				neuron->neuronOutput = net->getLayer(layer->getIndex()).getOutput().getAt(neuron->getIndex(), 0);
				neuron->showOutput();
			}
	else
		for(Neuron* neuron : network->getChild(layerNum)->components)
		{
			neuron->neuronOutput = net->getLayer(layerNum).getOutput().getAt(neuron->getIndex(), 0);
			neuron->showOutput();
		}
	scene->update();
}

void NetworkPainter::paintNetwork()
{
	if (network->components.size() == 0)
		return;
	if (paintMode == FULL) {
		network->setPaintLayersMenu(false);
	}
	if(paintMode == LAYERS)
		network->setPaintLayersMenu(true);

	networkOnScene = true;
	scene->update();
	if (paintMode == FULL)
		view->ensureVisible(network);
	else
		view->ensureVisible(network->components[0]);
}

void NetworkPainter::clear()
{
	network->eraseAllComponents();
	networkOnScene = false;
}

void NetworkPainter::update()
{
	network->update();
}



void NetworkPainter::setupNetwork()
{
	if (networkOnScene)
		clear();
	const int lays = net->getTotalLayersCount();
	if (lays <= 0)
		return;

	if (paintMode == FULL) {
		for (int i = 0; i < lays; i++)
		{
			Layer* layer = new Layer(&net->getLayer(i), *scene, network);
			connect(layer, &Layer::layerDoubleClick, this, &NetworkPainter::layerDoubleClick);
			network->addComponent(layer);
			for (int j = 0; j < net->getLayer(i).getNeuronsCount(); j++) {
				Neuron* neuron = new Neuron(*scene, layer);
				connect(neuron, &Neuron::neuronDoubleClick, this, &NetworkPainter::neuronDoubleClick);
				connect(neuron, &Neuron::neuronMouseOver, this, &NetworkPainter::neuronMouseOverEvent);
				connect(neuron, &Neuron::neuronMouseLeave, this, &NetworkPainter::neuronMouseLeaveEvent);
				layer->addComponent(neuron);
			}

		}
		network->updateInOutNeuronsPositions();
		for (int i = 1; i < network->layersCount(); i++) {

			Layer* layer = network->getChild(i);
			for (int j = 0; j < layer->neuronsCount(); j++) {

				Layer* prevlayer = network->getChild(i - 1);
				Neuron* neuron = layer->getChild(j);
				for (int b = 0; b < prevlayer->neuronsCount(); b++) {
					Neuron* prevneuron = prevlayer->getChild(b);
					Synapse* syn = new Synapse(*scene, prevneuron, neuron);
					syn->setZValue(-1);
					neuron->addComponent(syn);
				}
			}
		}

		
	}else if(paintMode == LAYERS)
	{
		for (int i = 0; i < lays; i++)
		{
			Layer* layer = new Layer(&net->getLayer(i), *scene, network);
			connect(layer, &Layer::layerDoubleClick, this, &NetworkPainter::layerDoubleClick);
			network->addComponent(layer);
		}
	}
	QRectF scRect = view->sceneRect();
	scRect.setWidth(network->getNetworkWidth());
	scRect.setHeight(network->getNetworkHeight());
	view->setSceneRect(scRect);
}

void NetworkPainter::setupNetwork(FFNeuralNetwork * net)
{
	this->net = net;
	setupNetwork();
}

void NetworkPainter::setPaintMode(NetworkPaintMode mode)
{
	clear();
	paintMode = mode;
	setupNetwork();
	paintNetwork();
}

void NetworkPainter::setGraphicsView(QGraphicsView* view)
{
	this->view = view;
	this->view->setScene(scene);
}

void NetworkPainter::handleCommand(CommandType cmd)
{
	switch (cmd) {
	case NETWORK_CLEAR:
	{
		clear();
		break;
	}
	case NETWORK_INPUT:
	{
		updateNeuronsOutput(0);
		break;
	}
	case NETWORK_STRUCTURE:
	{
		setupNetwork();
		paintNetwork();
		break;
	}
	case TRAINER_ITERATION:
	{
		updateNeuronsOutput(-1);
		break;
	}
	case TRAINER_TRAIN_EPOCH:
	{
		//netPainter.updateNeuronsOutput(-1);
		break;
	}
	case TRAINER_TRAIN_LAST_EPOCH:
	case TRAINER_EPOCH:
	{
		updateNeuronsOutput(-1);
		break;
	}
	}
}

void NetworkPainter::layerDoubleClick(Layer* layer)
{
	emit layerDoubleClickEvent(layer);
}

void NetworkPainter::neuronDoubleClick(Neuron* neuron)
{
	emit neuronDoubleClickEvent(neuron);
}
