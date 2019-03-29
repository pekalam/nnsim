#pragma once

#include <QDialog>
#include "ui_OutputPlot.h"

class matrix;

class OutputPlot : public QDialog
{
	Q_OBJECT

public:
	OutputPlot(QWidget *parent = Q_NULLPTR);
	~OutputPlot() = default;
	void update()
	{
		ui.plot->rescaleAxes();
		ui.plot->replot(QCustomPlot::RefreshPriority::rpImmediateRefresh);
	}
	void updateLinePlot() { ui.plot->replotLinePlot(); }
	void updateScatteredPlot() {  ui.plot->replotScatteredPlot(); }
	bool autoUpdate() { return ui.autoUpdate->isChecked(); }
	void setScatteredData(std::vector<matrix>& x, std::vector<matrix>& y);
	void setMaxLineDataSize(int max) { maxLineDataSize = max; }

	void addLineData(std::vector<matrix>& x, std::vector<matrix>& y);

	void resetLineData()
	{
		ui.plot->linePlotReset();
		maxLineDataSize = 0;
	}
	void resetScatteredData()
	{
		ui.plot->scatterPlotReset();
	}
private:
	int maxLineDataSize;
	Ui::OutputPlot ui;
};
