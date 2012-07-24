#ifndef ASKTOPAIRDIALOG_H
#define ASKTOPAIRDIALOG_H

#include <QDialog>

namespace Ui {
class AskToPairDialog;
}

class AskToPairDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AskToPairDialog(QWidget *parent = 0);
    ~AskToPairDialog();

signals:
    void allowedToPair(bool yes);

private:
    Ui::AskToPairDialog *ui;

private slots:
    void yesClicked();
    void noClicked();
};

#endif // ASKTOPAIRDIALOG_H
