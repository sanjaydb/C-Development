#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDir>
#include <QWidget>
#include <QListWidget>



QT_BEGIN_NAMESPACE
class QComboBox;
class QLabel;
class QPushButton;
class QTableWidget;
class QTableWidgetItem;
QT_END_NAMESPACE


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void browse();
    void find();
    void animateFindClick();
    void openFileOfItem(int row, int column);
    void contextMenu(const QPoint &pos);
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_clicked();
    void on_pushButton_9_clicked();
    void onListMailItemClicked(QListWidgetItem* item);
    void onListMailItemClicked4(QListWidgetItem* item);
    //void slotx(QVariant &var){ qDebug() << var.toStringList();}

    void on_pushButton_10_clicked();

    void on_pushButton_5_clicked();

signals :
    //void signalx(QVariant &var);


private:
    Ui::MainWindow *ui;

    void GetLocalIP();
    void GetInternetIP();
    void showTime();

    QStringList findFiles(const QStringList &files, const QString &text);
    void showFiles(const QStringList &paths);
    QComboBox *createComboBox(const QString &text = QString());
    void createFilesTable();

    QTimer *ctime;

    int ss_DirIndex = 1 ;
    QDir currentDir;

    QComboBox *fileComboBox;
    QComboBox *textComboBox;
    QComboBox *directoryComboBox;
    QLabel *filesFoundLabel;
    QPushButton *findButton;
    QTableWidget *filesTable;

    //void on_listView_clicked(const QModelIndex &index);
    QString a1,b1 ;


};
#endif // MAINWINDOW_H
