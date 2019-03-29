#pragma once

#include <QDialog>
#include "ui_ClassificationOutputPlot.h"
#include "FFNeuralNetwork.h"

class matrix;

class ClassificationOutputPlot : public QDialog
{
	Q_OBJECT

public:
	ClassificationOutputPlot(QWidget *parent = Q_NULLPTR);
	~ClassificationOutputPlot();
	void setScatteredData(std::vector<matrix>& trainingSetX, std::vector<int>& trainingSetClasses);
	void setDataRange(double xlower, double xupper, double ylower, double yupper);
	void resetScatteredData();
	void resetColorMapData();
	void setResolution(int res);
	void setClassCount(int count);
	void fillMap(FFNeuralNetwork& net);
	void update();
private:
	Ui::ClassificationOutputPlot ui;
};
