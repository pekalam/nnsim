#pragma once
#include "ui_VisWindow.h"

#include "CommandExecutor.h"
#include "NetworkPainter.h"



#include "SineGeneratorDialog.h"
#include "NetworkContext.h"
#include "Trainer.h"
#include "CommandHandler.h"
#include "AppContext.h"

enum CommandType;
class WindowsContainer;

class VisWindow : public QMainWindow, public CommandHandler
{
	Q_OBJECT
		friend class VisualizationWindowProxy;
public:
	VisWindow(AppContext &appContext, QWidget *parent = Q_NULLPTR);
	NetworkPainter netPainter;
	void handleCommand(CommandType cmd) override;
private:
	Ui::VisualizationWindow ui;
	AppContext &appContext;
	qreal zoomStep = 5;
	bool execScriptFromFile(QString path);
	void closeEvent(QCloseEvent* event) override;
private slots:
	void zoomIn(bool c);
	void zoomOut(bool c);
	void scriptLoad(bool c);
	void layerDoubleClickEvent(Layer *layer);
	void onNeuronDoubleClick(Neuron *neuron);
	void onNeuronMouseOver(Neuron *neuron);
	void onNeuronMouseLeave(Neuron *neuron);
	void setupMatrix();
	void loadExample();
public slots:
	void togglePaintMode();
	
};

