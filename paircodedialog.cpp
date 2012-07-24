#include "paircodedialog.h"
#include "ui_paircodedialog.h"

PairCodeDialog::PairCodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PairCodeDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, SIGNAL(rejected()),
            this, SLOT(pairCanceled()));

}

PairCodeDialog::~PairCodeDialog()
{
    delete ui;
}

void PairCodeDialog::setPairCode(QString code)
{
    ui->lbl_Code->setText(code);
}

void PairCodeDialog::pairCanceled()
{
    emit canceled();
    this->close();
}

void PairCodeDialog::pairCanceledByDevice()
{
    this->close();
}

void PairCodeDialog::devicePaired(network_id_ptr id, QHostAddress ip, bool successful)
{
    this->close();
}
