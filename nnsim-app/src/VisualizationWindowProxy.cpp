#include "VisualizationWindowProxy.h"
#include "VisWindow.h"
#include "CommandType.h"


VisualizationWindowProxy::VisualizationWindowProxy(QObject* parent, VisWindow* visualizationWindow) : QObject(parent), visualizationWindow(visualizationWindow)
{
}

VisualizationWindowProxy::~VisualizationWindowProxy()
{
}
