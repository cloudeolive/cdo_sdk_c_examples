#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <cloudeo_sdk.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setVersion(std::string version);

    void draw();
    void startRendering(CDOH platformHandle, std::string sinkId);
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
