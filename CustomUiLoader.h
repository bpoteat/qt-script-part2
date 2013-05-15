#ifndef CUSTOMUILOADER_H
#define CUSTOMUILOADER_H

#include <QtUiTools/QUiLoader>
#include "CustomTableWidget.h"

class CustomUiLoader : public QUiLoader
{
    Q_OBJECT
public:
    CustomUiLoader(QObject *parent = 0) : QUiLoader(parent) { }
    
    virtual QWidget* createWidget(const QString &className, QWidget *parent = 0, const QString &name = QString())
    {
        QWidget* widget = NULL;
        if (className == "QTableWidget")
        {
            // replace any QTableWidget instances with instance of our own CustomTableWidget
            widget = new CustomTableWidget(parent);
        }

        if (NULL != widget)
        {
            widget->setObjectName(name);
        }
        else
        {
            // let base class handle any widgets for which we don't have a custom class
            widget = QUiLoader::createWidget(className, parent, name);
        }
        return widget;
    }

};

#endif // CUSTOMUILOADER_H
