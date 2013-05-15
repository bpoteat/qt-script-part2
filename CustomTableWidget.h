#ifndef CUSTOMTABLEWIDGET_H
#define CUSTOMTABLEWIDGET_H

#include <QTableWidget>

class CustomTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    CustomTableWidget(QWidget *parent = 0) : QTableWidget(parent) { }
    
    Q_INVOKABLE void setItemData(int row, int column, QVariant data, int userRole = Qt::DisplayRole)
    {
        QTableWidgetItem* item = this->item(row, column);
        if (NULL == item)
        {
            item = new QTableWidgetItem(data.toString());
            this->setItem(row, column, item);
        }
        item->setData(userRole, data);
    }
};

#endif // CUSTOMTABLEWIDGET_H
