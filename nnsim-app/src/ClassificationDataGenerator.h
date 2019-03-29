#pragma once

#include <QDialog>
#include "ui_ClassificationDataGenerator.h"


class AppContext;

class ClassificationDataGenerator : public QDialog
{
	Q_OBJECT

public:
	ClassificationDataGenerator(AppContext &constext, QWidget *parent = Q_NULLPTR);
	~ClassificationDataGenerator();

private:
	Ui::ClassificationDataGenerator ui;
	AppContext& context;
	void genData();
};
