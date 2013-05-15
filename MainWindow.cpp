#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QStringBuilder>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QCloseEvent>
#include <QScriptValue>
#include "CustomUiLoader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionExecute, SIGNAL(triggered()), this, SLOT(ExecuteScript()));

    this->SetupScriptEngine();

    ui->textOutput->setPlainText("current directory: " % QDir::currentPath());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetupScriptEngine(void)
{
    // expose this widget to the script
    QScriptValue appContext = fScriptEngine.newQObject(this);
    fScriptEngine.globalObject().setProperty("app", appContext);
}

void MainWindow::feedback(QString message)
{
    ui->textOutput->setPlainText(message);
}


void MainWindow::ExecuteScript(void)
{
    QString text = ui->textScript->toPlainText();
    QScriptValue result = fScriptEngine.evaluate(text);
    if (fScriptEngine.hasUncaughtException())
    {
        int line = fScriptEngine.uncaughtExceptionLineNumber();
        QString err = result.toString();
        QString msg = QString("Error at line %1: %2")
            .arg(line).arg(err);

        ui->textOutput->setPlainText(msg);
    }
}


QScriptValue MainWindow::loadUi(QString uiPath)
{
    if (uiPath.isEmpty())
    {
        uiPath = QFileDialog::getOpenFileName(this, "Select file", ".", "UI Files (*.ui);;All files (*.*);;");
    }
    if (uiPath.isEmpty())
    {
        return QScriptValue(&fScriptEngine, 0);
    }

    CustomUiLoader loader;

    QFile file(uiPath);
    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::warning(this, "Invalid UI file", QString("The UI file specified, [%1], could not be loaded as a designer file").arg(uiPath));
        return QScriptValue(&fScriptEngine, 0);
    }

    QWidget* widget = loader.load(&file);
    file.close();

    return fScriptEngine.newQObject(widget);
}
