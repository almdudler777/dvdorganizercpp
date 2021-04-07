#ifndef ACTORWINDOW_H
#define ACTORWINDOW_H

#include <QDialog>

namespace Ui {
class actorWindow;
}

class ActorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ActorWindow(QWidget *parent = 0);
    ~ActorWindow();

private:
    Ui::actorWindow *ui;
};

#endif // ACTORWINDOW_H
