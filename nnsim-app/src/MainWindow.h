#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"

#include "AppContext.h"
#include "Trainer.h"
#include "Terminal.h"
#include "VisWindow.h"
#include "ClassificationDataGenerator.h"
#include "AppHandwrittenDigits.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
	void showErrorMessage(QString msg);
	static void addChild(QWidget &wid, float width, float height, int initX = 0, int initY = 0);
protected:
	void showEvent(QShowEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;
private:
	struct SubWindowSz
	{
		QPointer<QMdiSubWindow> window;
		float percWidth;
		float percHeight;
		int initX;
		int initY;
	};
	Ui::MainWindow ui;
	static MainWindow *instance;
	void createVisWindow();
	void createTerminal();
	void networkChangeRegression();
	void networkChangeClassification();
	QMessageBox messageBox;
	AppContext appContext;
	SineGeneratorDialog dialogSineGen;
	ClassificationDataGenerator classDataGen;
	QPointer<VisWindow> visWind = nullptr;
	QPointer<Terminal> term = nullptr;
	QPointer<AppHandwrittenDigits> appDigits = nullptr;
	QVector<SubWindowSz> mdiChildren;
	Trainer trainer;
	NeuralNetworkProxy netProxy;
	NetworkTrainerProxy netTrainerProxy;
	bool wasVisible = false;
private slots:
	void onSubwindowDestroyed();

};
