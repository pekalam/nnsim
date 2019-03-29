#pragma once
#include <QString>
#include <QObject>
#include "CommandType.h"
#include <QVariant>

class matrix;

class CommandProxy : public QObject
{
	Q_OBJECT
public:
	virtual bool acceptedObject(QString objectName) { return false; }
	virtual void stopAsyncCommands(){}
	QString commandError;
signals:
	void asyncCommandCompleted(CommandType cmd);
protected:
	CommandProxy(QObject *parent);
	
};

