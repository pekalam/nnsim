#pragma once

#include <QGraphicsScene>
#include <QGraphicsView>
#include <qstatemachine.h>
#include <qtimer.h>
#include "CommandHandler.h"


class Network;
class Neuron;
class FFNeuralNetwork;
class Layer;




class NetworkPainter : public QObject, public CommandHandler
{
	Q_OBJECT
public:
	enum NetworkPaintMode { FULL, LAYERS };
	NetworkPainter(FFNeuralNetwork *network, QObject *parent = Q_NULLPTR);
	~NetworkPainter();
	void updateNeuronsOutput(int layerNum = -1);
	void paintNetwork();
	void clear();
	void update();
	void setupNetwork(); 
	void setupNetwork(FFNeuralNetwork * net);
	void setPaintMode(NetworkPaintMode mode);
	void togglePaintMode() { if (paintMode == FULL) setPaintMode(LAYERS); else setPaintMode(FULL); }
	void setGraphicsView(QGraphicsView* view);
	void handleCommand(CommandType cmd) override;
	NetworkPaintMode paintMode;
private:
	Network *network;
	FFNeuralNetwork *net;
	QGraphicsScene *scene;
	QGraphicsView *view;
	bool networkOnScene;
	
private slots:
	void layerDoubleClick(Layer *layer);
	void neuronDoubleClick(Neuron *neuron);
signals:
	void layerDoubleClickEvent(Layer *layer);
	void neuronDoubleClickEvent(Neuron *neuron);
	void neuronMouseOverEvent(Neuron *neuron);
	void neuronMouseLeaveEvent(Neuron *neuron);
	void changeAnimationState();
};

