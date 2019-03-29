#pragma once

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QStringList>

class TerminalWidget : public QTextEdit
{
	Q_OBJECT

public:
	TerminalWidget(QWidget *parent = Q_NULLPTR);
	void addAutoCompletionCommand(QString cmd);
	QStringList history;
	int isInAutoCompletionList(QString cmd);
signals:
	void commandExec(QString command);
public slots:
	void printLine(QString line);
protected:
	void keyPressEvent(QKeyEvent * e);
private:
	int historyPositon = 0;
	int currentCommandPos = 2;
	QString currentCommand;
	QStringList autoCompletionCommands;
	bool cursorInEditionZone();



};




