#ifndef NEWMOVIE_H
#define NEWMOVIE_H

#include <QDialog>
#include "roles.h"
#include "movie.h"

namespace Ui {
class movieWindow;
}

class movieWindow : public QDialog
{
    Q_OBJECT

public:
    explicit movieWindow(QWidget *parent = 0, int movieID = 0);
    ~movieWindow();

private slots:
    void on_schauspielerTree_itemSelectionChanged();
    void on_kategorieTree_itemSelectionChanged();
    void on_kategorieRemove_clicked();
    void on_SchauspielerRemove_clicked();
    void on_schauspielerAdd_clicked();
    void on_kategorieAdd_clicked();
    void on_movieWindow_accepted();
    void done(int r);

private:
    Ui::movieWindow *ui;
    int movieId;
};

#endif // NEWMOVIE_H
