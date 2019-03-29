#include <QtWidgets/QApplication>
#include "WindowsContainer.h"
#include "VisWindow.h"
#include "Terminal.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow wmain;
	wmain.showMaximized();
	return a.exec();
}
