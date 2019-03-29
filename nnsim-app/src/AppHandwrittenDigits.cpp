#include "AppHandwrittenDigits.h"
#include <QQuickView>
#include <QUrl>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlComponent>


AppHandwrittenDigits::AppHandwrittenDigits(AppContext &context, NeuralNetworkProxy &proxy, QObject *parent)
	: QObject(parent), mat(16,16,0), netProxy(proxy), context(context)
{
	view.setSource(QUrl("qrc:/qml_views/qml_files/handwritten_digits.qml"));
	view.rootContext()->setContextProperty("backend", this);
}

AppHandwrittenDigits::~AppHandwrittenDigits()
{
}

void AppHandwrittenDigits::handleCommand(CommandType cmd)
{
	if(cmd == NETWORK_VALUE)
	{
		auto output = context.networkContext.network.getOutput();
		matrix_dtype max = MX(0, 0, output);
		int maxind = 0;
		for(int i = 1; i < output.rows(); i++)
			if(MX(i, 0, output) > max)
			{
				max = MX(i, 0, output);
				maxind = i;
			}
		response = maxind;
		emit networkResponseChanged();
	}
}

void AppHandwrittenDigits::show()
{
	view.show();
}

void AppHandwrittenDigits::setAt(int i, int j, int val)
{
	if(i >= 0 && j >= 0 && i < mat.rows() && j < mat.cols())
		MX(i, j, mat) = val;
}

void AppHandwrittenDigits::clear()
{
	for(int i = 0; i < mat.rows(); i++)
	{
		for(int j = 0; j < mat.cols(); j++)
		{
			MX(i, j, mat) = 0;
		}
	}
}

void AppHandwrittenDigits::predict()
{
	matrix input(mat.rows()*mat.cols(), 1);
	int r = 0;
	for (int i = 0; i < mat.rows(); i++)
		for (int j = 0; j < mat.cols(); j++) {
			MX(r, 0, input) = MX(i, j, mat);
			r++;
		}
	context.commandEngine.handleCommand(netProxy.app_Evaluate(input));
}
