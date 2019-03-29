#include "Terminal.h"
#include "TerminalWidget.h"

Terminal::Terminal(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	terminalWidget = ui.widget;
}
