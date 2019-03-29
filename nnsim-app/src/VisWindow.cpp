#include "VisWindow.h"
#include <QtOpenGL/QtOpenGL>
#include <QtOpenGL/QGLWidget>
#include <qfiledialog.h>
#include <qtextstream.h>
#include "LayerDialog.h"
#include "NeuronDialog.h"

#include "CommandType.h"
#include "FFGradientDescend.h"
#include "WindowsContainer.h"
#include "ExampleNetworkDialog.h"
#include "MainWindow.h"


VisWindow::VisWindow(AppContext &appContext, QWidget *parent)
	: QMainWindow(parent), appContext(appContext), CommandHandler(),
	netPainter(&appContext.networkContext.network)
{
	ui.setupUi(this);
	netPainter.setGraphicsView(ui.graphicsView);
	ui.graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
	connect(ui.actionOpen_file, &QAction::triggered, this, &VisWindow::scriptLoad);
	connect(ui.actionExampleNet1, &QAction::triggered, this, &VisWindow::loadExample);
	connect(&netPainter, &NetworkPainter::layerDoubleClickEvent, this, &VisWindow::layerDoubleClickEvent);
	connect(&netPainter, &NetworkPainter::neuronDoubleClickEvent, this, &VisWindow::onNeuronDoubleClick);
	connect(&netPainter, &NetworkPainter::neuronMouseOverEvent, this, &VisWindow::onNeuronMouseOver);
	connect(&netPainter, &NetworkPainter::neuronMouseLeaveEvent, this, &VisWindow::onNeuronMouseLeave);
	connect(ui.actionDisplay_layers, &QAction::triggered, this, &VisWindow::togglePaintMode);
	connect(ui.zoomSlider, &QSlider::valueChanged, this, &VisWindow::setupMatrix);

	appContext.commandEngine.addHandler(netPainter);
	netPainter.setupNetwork();
	netPainter.paintNetwork();
}

void VisWindow::closeEvent(QCloseEvent* event)
{
	appContext.commandEngine.removeHandler(*this);
	appContext.commandEngine.removeHandler(netPainter);
	QMainWindow::closeEvent(event);
}

void VisWindow::zoomIn(bool c)
{
	ui.zoomSlider->setValue(ui.zoomSlider->value() + zoomStep);
	setupMatrix();
}

void VisWindow::zoomOut(bool c)
{
	ui.zoomSlider->setValue(ui.zoomSlider->value() - zoomStep);
	setupMatrix();
}

void VisWindow::scriptLoad(bool c)
{
	QFileDialog dialog;
	QStringList files;
	dialog.setNameFilter("Program script (*.txt)");
	if (dialog.exec()) {
		files = dialog.selectedFiles();
		execScriptFromFile(files[0]);
	}
}

void VisWindow::layerDoubleClickEvent(Layer *layer)
{
	LayerDialog dialog(this, layer);
	QObject::connect(&dialog, &LayerDialog::dialogCmd, &appContext.commandEngine, &CommandExecutor::executeCommand, Qt::DirectConnection);
	dialog.exec();
}

void VisWindow::onNeuronDoubleClick(Neuron* neuron)
{
	NeuronDialog dialog(neuron, this);
	dialog.exec();
}

void VisWindow::onNeuronMouseOver(Neuron* neuron)
{
}

void VisWindow::onNeuronMouseLeave(Neuron* neuron)
{
}

bool VisWindow::execScriptFromFile(QString path)
{
	appContext.commandEngine.executeCommand("net.Clear()");
	QFile f(path);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
		return false;
	QTextStream txt(&f);
	while (!txt.atEnd())
		appContext.commandEngine.executeCommand(txt.readLine());
	f.close();
	return true;
}

void VisWindow::setupMatrix()
{
	QMatrix matrix;
	qreal scale = qPow(qreal(2), (ui.zoomSlider->value() - 250) / qreal(50));
	matrix.scale(scale, scale);
	ui.graphicsView->setMatrix(matrix);
}

void VisWindow::loadExample()
{
	ExampleNetworkDialog exampleNetworkDialog;
	if(exampleNetworkDialog.exec() == QDialog::Accepted)
	{
		execScriptFromFile(exampleNetworkDialog.selectedPath);
	}
}

void VisWindow::togglePaintMode()
{
	netPainter.togglePaintMode();
}

void VisWindow::handleCommand(CommandType cmd)
{
	switch (cmd) {
	case NETWORK_VALUE:
	{
		netPainter.updateNeuronsOutput();
		break;
	}
	case TRAINER_ITERATION:
	{
		ui.stat1Label->setText(QString("Iteration error: %1").arg(appContext.networkContext.trainer.get()->lastIterationTrainingError()));
		ui.stat2Label->setText(QString("Network error: %1").arg(appContext.networkContext.trainer.get()->lastEpochTrainingError()));
		ui.stat3Label->setText(QString("Epoch: %1").arg(appContext.networkContext.trainer.get()->epochsCount()));

		break;
	}
	case TRAINER_TRAIN_EPOCH:
	{
		ui.stat1Label->setText(QString(""));
		ui.stat2Label->setText(QString("Network error: %1").arg(appContext.networkContext.trainer.get()->lastEpochTrainingError()));
		ui.stat3Label->setText(QString("Epoch: %1").arg(appContext.networkContext.trainer.get()->epochsCount()));
		break;
	}
	case TRAINER_TRAIN_LAST_EPOCH:
	case TRAINER_EPOCH:
	{
		ui.stat1Label->setText(QString(""));
		ui.stat2Label->setText(QString("Network error: %1").arg(appContext.networkContext.trainer.get()->lastEpochTrainingError()));
		ui.stat3Label->setText(QString("Epoch: %1").arg(appContext.networkContext.trainer.get()->epochsCount()));
		break;
	}
	}
}
