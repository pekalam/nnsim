#pragma once

#include <QObject>

class VisWindow;
enum CommandType;

class VisualizationWindowProxy : public QObject
{
	Q_OBJECT
private:

	VisWindow* visualizationWindow;
public:
	VisualizationWindowProxy(QObject *parent, VisWindow* visualizationWindow);
	~VisualizationWindowProxy();
};
