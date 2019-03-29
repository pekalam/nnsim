#pragma once

#include <QDialog>
#include "ui_Terminal.h"
#include "TerminalWidget.h"


class Terminal : public QDialog
{
	Q_OBJECT
public:
	Terminal(QWidget *parent = Q_NULLPTR);
	TerminalWidget *terminalWidget;
private:
	Ui::Terminal ui;
};
