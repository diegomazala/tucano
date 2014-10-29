#ifndef __MAINWINDOW__
#define __MAINWINDOW__

#include <QMainWindow>
#include "glwidget.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initialize( void );

private:
    Ui::MainWindow *ui;

protected:
    void keyPressEvent(QKeyEvent *ke);


};

#endif // MAINWINDOW
