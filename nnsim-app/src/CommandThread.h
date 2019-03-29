#pragma once

#include <QObject>
#include <qthread.h>
#include <functional>

enum CommandType;

class CommandThread : public QObject
{
	Q_OBJECT

	std::function<CommandType(CommandThread*)> func;
	void moveTo(QThread* thread)
	{
		bool ok;
		this->moveToThread(thread);
		ok = connect(thread, SIGNAL(started()), this, SLOT(process()), Qt::DirectConnection);
		Q_ASSERT(ok);
		ok = connect(this, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
		Q_ASSERT(ok);
	}
public:
	CommandThread(std::function<CommandType(CommandThread*)> func, QObject *parent);
	CommandThread(QObject *parent);
	QThread thread;
	void setFunction(std::function<CommandType(CommandThread*)> func) { this->func = func; }
	void start()
	{
		thread.start();
	}
	void wait()
	{
		thread.wait();
	}
	void stop()
	{
		emit finished();
	}
public slots:
	void process();
	
signals:
	void completed(CommandType cmd);
	void finished();
	void error();
};
