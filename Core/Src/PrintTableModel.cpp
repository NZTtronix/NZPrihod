#include "PrintTableModel.h"
PrintTableModel::PrintTableModel(QObject *parent)
    : QAbstractTableModel(parent), m_timer(new QTimer(this))
{
   
}

int PrintTableModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 2;
}

int PrintTableModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 6;
}



QVariant PrintTableModel::data(const QModelIndex &index, int role) const
{
    
    int row = index.row();
    int col = index.column();
   
    // generate a log message when this method gets called
    
    if (role == Qt::DisplayRole && row == 0 && col == )
        return QTime::currentTime().toString();

    return QVariant();
}
QVariant PrintTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal) {
           switch (section) {
        case 0:
            return QString("X, mm");
        case 1:
            return QString("Y, mm");
        case 2:
            return QString("Width");
        case 3:
            return QString("Height");
        case 4:
            return QString("Type");
        case 5:
            return QString("Content");
        }
        } else if (orientation == Qt::Vertical) {
            return QString("Row %1").arg(section + 1);
        }
    }
    return QVariant();
   
}

void PrintTableModel::timerHit()
{
    // we identify the top left cell
    QModelIndex topLeft = createIndex(0,0);
    // emit a signal to make the view reread identified data
    emit dataChanged(topLeft, topLeft, {Qt::DisplayRole});

}



