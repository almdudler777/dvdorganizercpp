#ifndef SELECTIONWINDOW_H
#define SELECTIONWINDOW_H

#include <QDialog>
#include "roles.h"

namespace Ui {
class selectionWindow;
}

class selectionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit selectionWindow( int const modus, QWidget *parent = 0);
    ~selectionWindow();
    QList<int> getSelectedIds(int UserRole);

private:
    Ui::selectionWindow *ui;
};

#endif // SELECTIONWINDOW_H
