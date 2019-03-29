#pragma once

#include <QObject>
#include <QQuickView>
#include <QQmlEngine>
#include "matrix.h"
#include "NeuralNetworkProxy.h"
#include "AppContext.h"

class AppHandwrittenDigits : public QObject, public CommandHandler
{
	Q_OBJECT
	Q_PROPERTY(int networkResponse MEMBER response NOTIFY networkResponseChanged)
private:
	matrix mat;
	NeuralNetworkProxy &netProxy;
	AppContext &context;
	int response = 0;
private slots:
	void setNetworkResponse(int resp) { response = resp; }
	int getNetworkResponse() { return response; }
public:
	QQuickView view;
	AppHandwrittenDigits(AppContext &context, NeuralNetworkProxy &proxy,QObject *parent);
	~AppHandwrittenDigits();
	void handleCommand(CommandType cmd) override;
public slots:
	void show();
	void setAt(int i, int j, int val);
	void clear();
	void predict();
signals:
	void networkResponseChanged();
};
