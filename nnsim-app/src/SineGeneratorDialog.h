#pragma once

#include <QDialog>
#include "ui_SineGeneratorDialog.h"
#include <QVector>

class matrix;

class SineGeneratorDialog : public QDialog
{
	Q_OBJECT

public:
	SineGeneratorDialog(std::vector<matrix>* input, std::vector<matrix>* ideal, QWidget *parent = Q_NULLPTR);
	~SineGeneratorDialog();
	void setSetSize(int setSize) { this->setSize = setSize; }
private:
	Ui::SineGeneratorDialog ui;
	std::vector<matrix>* input;
	std::vector<matrix>* ideal;
	QVector<double> *sineX;
	QVector<double> *sineY;
	int setSize;
private slots:
	void onOkClicked(bool b);
	void onGenerateClicked(bool b);
};
