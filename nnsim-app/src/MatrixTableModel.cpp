#include "MatrixTableModel.h"
#include "matrix.h"


MatrixTableModel::MatrixTableModel(matrix& matrix, QObject *parent) : QAbstractTableModel(parent), mx(matrix)
{
	for(int i = 0; i < matrix.rows(); i++)
	{
		allowedRows.push_back(1);
	}
		for(int j = 0; j < matrix.cols(); j++)
		{
			allowedColumns.push_back(1);
		}
}


MatrixTableModel::~MatrixTableModel()
{

}

QVariant MatrixTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role != Qt::DisplayRole)
		return QVariant();

	if (orientation == Qt::Vertical && !headerVertical.isEmpty()) {
		return QString("%1 %2").arg(headerVertical).arg(section);
	}
	if(orientation == Qt::Horizontal && !headerHorizontal.isEmpty())
	{
		return QString("%1 %2").arg(headerHorizontal).arg(section);
	}
	return QVariant();
}

QVariant MatrixTableModel::data(const QModelIndex& index, int role) const
{
	if (role == Qt::DisplayRole) {
		int row = index.row();
		int col = index.column();
		for (int i = 0; i < allowedRows.size(); i++) {
			if (allowedRows[i] == 1)
				row--;
			if (row < 0) {
				row = i;
				break;
			}
		}
		for (int i = 0; i < allowedColumns.size(); i++) {
			if (allowedColumns[i] == 1)
				col--;
			if (col < 0) {
				col = i;
				break;
			}
		}
		return static_cast<double>(MX(row, col, mx));
	}
	return QVariant();
}

bool MatrixTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
	if(role == Qt::EditRole)
	{
		const int row = index.row();
		const int col = index.column();
		bool ok;
		double v = value.toDouble(&ok);
		if (ok)
			MX(row, col, mx) = v;
		return ok;
	}
	return false;
}

void MatrixTableModel::updateData()
{
	emit dataChanged(index(0, 0), index(mx.rows() - 1, mx.cols() - 1));
}
