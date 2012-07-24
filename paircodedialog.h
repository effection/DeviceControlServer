#ifndef PAIRCODEDIALOG_H
#define PAIRCODEDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include "networkid.h"

namespace Ui {
class PairCodeDialog;
}

class PairCodeDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PairCodeDialog(QWidget *parent = 0);
    ~PairCodeDialog();

    /**
      * Set the pair code label.
      */
    void setPairCode(QString code);

signals:
    /**
      * Cancel button was pressed.
      */
    void canceled();

public slots:
    /**
      * Pair canceled by device that is pairing.
      */
    void pairCanceledByDevice();

    /**
      * Device pairing finished.
      */
    void devicePaired(network_id_ptr id, QHostAddress ip, bool successful);
    
private:
    Ui::PairCodeDialog *ui;

private slots:
    /**
      * Cancel button pressed.
      */
    void pairCanceled();

};

#endif // PAIRCODEDIALOG_H
