#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent, Settings* globalSettings)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->globalSettings = globalSettings;

    QFont font(globalSettings->fontSettings.fontFamily, globalSettings->fontSettings.fontSize);
    ui->textEditor->setFont(font);
//    ui->textEditor->setFontFamily(globalSettings->fontSettings.fontFamily);
//    ui->textEditor->setFontPointSize(globalSettings->fontSettings.fontSize);

    //Add Options button on Menu toolbar
    QAction* action = new QAction("Options");
    connect(action, &QAction::triggered, this, &MainWindow::on_actionMenuOptions_triggered);
    ui->menubar->addAction(action);

    //Add Highlighter to textEditor
    highlighter = new Highlighter(ui->textEditor->document(), this->globalSettings);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionMenuExit_triggered()
{
    engine->Close();
    QApplication::exit();
}

void MainWindow::on_actionMenuOptions_triggered()
{
    OptionsDialog dialog(this);
    dialog.settings = globalSettings;
    dialog.show();
    if (dialog.exec() == QDialog::Accepted)
    {
        QFont font(globalSettings->fontSettings.fontFamily, globalSettings->fontSettings.fontSize);
        ui->textEditor->setFont(font);
//        ui->textEditor->setFontFamily(globalSettings->fontSettings.fontFamily);
//        ui->textEditor->setFontPointSize(globalSettings->fontSettings.fontSize);
        ui->textEditor->setPlainText(ui->textEditor->toPlainText());
    }
}

void MainWindow::on_actionMenuRun_triggered()
{
    if (!this->engine)
        this->engine = new Engine();
    if(this->ui->textEditor->toPlainText().size() > 0) {
        if (this->engine->New()) {
            QString codeString = this->ui->textEditor->toPlainText();
//            QByteArray code8bit = codeString.toLocal8Bit();
//            char *code = code8bit.data();
//            const char *result = this->engine->Run(code);
//            this->ui->textResults->append(result);
            this->ui->textResults->append(this->engine->Run(codeString));
            return;
        }
    }
    this->ui->textResults->append("Error\n");
}
void MainWindow::on_actionMenuClear_triggered()
{
    ui->textResults->setPlainText("");
}
void MainWindow::on_actionMenuNew_triggered()
{
    ui->textEditor->setPlainText("");
}
