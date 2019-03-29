#pragma once

#include <QObject>
#include "lib/qcustomplot.h"


class AutoRescalingPlot : public QCustomPlot
{
public:
	AutoRescalingPlot(QWidget * parent);
	void addData(double x, double y);
};
