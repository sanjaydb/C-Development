#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
      void valueChanged(int newValue);


private slots:
    void on_Left_pushButton_clicked();
    void on_Con_Clear_pushButton_2_clicked();
    void setValue(int value);

    void on_dial_valueChanged(int value);

private:
    Ui::MainWindow *ui;

    int m_value = 0;
    int m_bit = 0;
    void BitRun(int m_value);


};
#endif // MAINWINDOW_H
