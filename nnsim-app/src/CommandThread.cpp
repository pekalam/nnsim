#include "CommandThread.h"
#include  <exception>
#include "CommandType.h"


CommandThread::CommandThread(std::function<CommandType(CommandThread*)> func, QObject *parent)
	: QObject(parent), func(func)
{
	moveTo(&thread);
}

CommandThread::CommandThread(QObject* parent) : QObject(parent)
{
	moveTo(&thread);
}

void CommandThread::process()
{
		try
		{
			CommandType t = func(this);
			emit completed(t);
			emit finished();
		}
		catch (const std::exception& e)
		{
			emit error();
		}
}
