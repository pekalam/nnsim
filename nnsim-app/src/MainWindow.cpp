#include "MainWindow.h"
#include <QtWidgets>
#include "FFGradientDescend.h"
#include "QuadraticCost.h"

MainWindow* MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
	trainer(appContext, this),
	netProxy(appContext.networkContext, this),
	netTrainerProxy(appContext.networkContext, this),
	dialogSineGen(&appContext.networkContext.inputDataTraining, &appContext.networkContext.idealDataTraining, Q_NULLPTR),
	classDataGen(appContext, this)
{
	Q_ASSERT(instance == nullptr);
	if (instance == nullptr)
		instance = this;
	ui.setupUi(this);

	appContext.commandEngine.addProxy(netProxy);
	appContext.commandEngine.addProxy(netTrainerProxy);
	appContext.commandEngine.addHandler(trainer);
	connect(ui.actionIris, &QAction::triggered, [&]
	{
		classDataGen.exec();
	});
	connect(ui.actionTrainer_2, &QAction::triggered, &trainer, &Trainer::showTrainingDialog);
	connect(ui.actionNetwork_error, &QAction::triggered, &trainer, &Trainer::showNetworkErrorPlot);
	connect(ui.actionNetwork_output, &QAction::triggered, &trainer, &Trainer::showNetworkOutputPlot);
	connect(ui.actionRegression_network, &QAction::triggered, this, &MainWindow::networkChangeRegression);
	connect(ui.actionClassification_network, &QAction::triggered, this, &MainWindow::networkChangeClassification);
	connect(ui.actionVisualization, &QAction::triggered, [=]
	{
		if (visWind == nullptr)
			createVisWindow();
	});
	connect(ui.actionHandwritten_digits, &QAction::triggered, [=]
	{
		if (appContext.networkContext.network.getInputLayer().getNeuronsCount() != 256 ||
			appContext.networkContext.network.getOutputLayer().getNeuronsCount() != 10)
		{
			return;
		}
		const QString path = QString(":/example_data/classification_data/handwritten.txt");
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly)) {
			return;
		}
		appContext.networkContext.inputDataTraining.clear();
		appContext.networkContext.idealDataTraining.clear();

		while (!file.atEnd()) {
			QString line = file.readLine();
			QStringList l = line.split(' ');
			matrix x(256, 1);
			int yi = 0;
			matrix y(10, 1, 0);
			for(int i = 0; i < 256; i++)
				MX(i, 0, x) = l[i].toDouble();
			for(int i = 256; i < 256+10; i++, yi++)
				if(l[i].toInt() == 1)
					MX(yi, 0, y) = 1;
			appContext.networkContext.inputDataTraining.push_back(x);
			appContext.networkContext.idealDataTraining.push_back(y);
		}
		file.close();
	});
	connect(ui.actionConsole, &QAction::triggered, [&]
	{
		if (term == nullptr)
			createTerminal();
	});
	connect(ui.actionSine_function, &QAction::triggered, [&]
	{
		if (appContext.networkContext.network.getTotalLayersCount() < 2)
		{
			showErrorMessage("Network must have more than 2 layers.");
			return;
		}
		const int nInputSize = appContext.networkContext.network.getInputLayer().getNeuronsCount();
		const int nOutPutSize = appContext.networkContext.network.getOutputLayer().getNeuronsCount();
		if (nInputSize != nOutPutSize) {
			showErrorMessage("Network input and output layers must have equal size.");
			return;
		}
		dialogSineGen.setSetSize(nInputSize);
		dialogSineGen.exec();
	});
	connect(ui.actionAppHandwritten_digits, &QAction::triggered, [&]
	{
		appDigits->show();
	});
	appDigits = new AppHandwrittenDigits(appContext, netProxy, this);
	appContext.commandEngine.addHandler(*appDigits);
	createVisWindow();
	createTerminal();
}

MainWindow::~MainWindow()
{
	netTrainerProxy.terminate();
}

void MainWindow::showErrorMessage(QString msg)
{
	messageBox.critical(0, "Error", msg);
}

void MainWindow::addChild(QWidget& wid, float width, float height, int initX, int initY)
{
	Q_ASSERT(instance != nullptr);
	SubWindowSz subWindSz;
	QMdiSubWindow* subWindow = instance->ui.mdiArea->addSubWindow(&wid);
	subWindSz.window = subWindow;
	subWindSz.percWidth = width;
	subWindSz.percHeight = height;
	subWindSz.initX = initX;
	subWindSz.initY = initY;
	const QRect& winGeo = instance->ui.mdiArea->geometry();
	subWindSz.window->setGeometry(winGeo.width()*subWindSz.initX / 100,
		winGeo.height()*subWindSz.initY / 100,
		winGeo.width()*subWindSz.percWidth / 100,
		winGeo.height()*subWindSz.percHeight / 100);
	instance->mdiChildren.push_back(subWindSz);
	connect(subWindow, &QMdiSubWindow::destroyed, instance, &MainWindow::onSubwindowDestroyed);
}

void MainWindow::showEvent(QShowEvent* event)
{
	QMainWindow::showEvent(event);
	
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	QMainWindow::resizeEvent(event);
	qDebug() << "resize" << isVisible();
	if (!wasVisible && isVisible())
	{
		const QRect& winGeo = ui.mdiArea->geometry();
		for(auto subWinSz : mdiChildren)
		{
			subWinSz.window->setGeometry(winGeo.width()*subWinSz.initX / 100,
				winGeo.height()*subWinSz.initY / 100,
				winGeo.width()*subWinSz.percWidth/100,
				winGeo.height()*subWinSz.percHeight/100);
		}
		wasVisible = true;
	}
}

void MainWindow::createVisWindow()
{
	visWind = new VisWindow(appContext, this);
	appContext.commandEngine.addHandler(*visWind);
	MainWindow::addChild(*visWind, 70, 60, 0, 0);
	visWind->show();
}

void MainWindow::createTerminal()
{
	term = new Terminal(this);
	connect(term->terminalWidget, &TerminalWidget::commandExec, &appContext.commandEngine, &CommandExecutor::executeCommand);
	connect(&appContext.commandEngine, &CommandExecutor::terminalOutput, term->terminalWidget, &TerminalWidget::printLine);
	MainWindow::addChild(*term, 30, 30, 70, 0);
	
	term->show();
}

void MainWindow::networkChangeRegression()
{
	appContext.networkContext.setNetworkTaskType(NetworkContext::Regression);
}

void MainWindow::networkChangeClassification()
{
	appContext.networkContext.setNetworkTaskType(NetworkContext::Classification);
}

void MainWindow::onSubwindowDestroyed()
{
	QMdiSubWindow* sub = (QMdiSubWindow*)sender();
	for(int i = 0; i < mdiChildren.size(); i++)
	{
		if(mdiChildren.at(i).window.data() == sub)
		{
			mdiChildren.remove(i);
			break;
		}
	}
}
