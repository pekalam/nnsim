#pragma once
#include <qabstractitemmodel.h>

class matrix;

class MatrixTableModel :
	public QAbstractTableModel
{
private:
	matrix &mx;
	QString headerVertical;
	QString headerHorizontal;
	QVector<int> allowedRows;
	QVector<int> allowedColumns;
public:
	MatrixTableModel(matrix& matrix,QObject *parent);
	~MatrixTableModel();
	void setVerticalHeader(const QString vert) { headerVertical = vert; }
	void setHorizontalHeader(const QString hor) { headerHorizontal = hor; }
	void hideRow(int row) { allowedRows[row] = 0; }
	void hideColumn(int col) { allowedColumns[col] = 0; }
	int rowCount(const QModelIndex &parent = QModelIndex()) const override { int sum = 0; for (int i = 0; i < allowedRows.size(); i++) sum += allowedRows[i]; return sum; }
	int columnCount(const QModelIndex &parent = QModelIndex()) const override { int sum = 0; for (int i = 0; i < allowedColumns.size(); i++) sum += allowedColumns[i]; return sum; }
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	Qt::ItemFlags flags(const QModelIndex &index) const override{ return QAbstractItemModel::flags(index); }
	matrix& getMatrix() { return mx; }
	void updateData();
};

