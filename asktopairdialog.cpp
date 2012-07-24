#include "asktopairdialog.h"
#include "ui_asktopairdialog.h"

AskToPairDialog::AskToPairDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskToPairDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(accepted()),
            this, SLOT(yesClicked()));
    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(noClicked()));
}

AskToPairDialog::~AskToPairDialog()
{
    delete ui;
}

void AskToPairDialog::yesClicked()
{
    emit allowedToPair(true);
    this->close();
}

void AskToPairDialog::noClicked()
{
    emit allowedToPair(false);
    this->close();
}
