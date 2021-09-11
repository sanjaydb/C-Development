#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::BitRun(int m_value)
{
    QString  m_chars = ui->Con_textEdit_3->toPlainText();
    QString m_data ;
    int m_leftShift ;

    for (int i =0 ;i<m_chars.size(); i++)
    {
        /*
        int StringValue = m_chars.at(i).digitValue();
               qDebug () << "firstDigit" << StringValue ;
             ui->Con_textEdit_2->append(QString::number(StringValue));
        */

             QChar c = m_chars.at(i);
             int v_latin = c.toLatin1();
              m_leftShift = v_latin << m_bit ;

             if (v_latin == 32){
                  m_data.truncate(  m_data.lastIndexOf(QChar('-')));
                 m_data = m_data + "    " ;
             }else{
                 m_data = m_data + QString::number(v_latin)+"-" ;
             }

             ui->Con_textEdit->append(QString::number(m_leftShift));

    }
     m_data.truncate(  m_data.lastIndexOf(QChar('-')));
    ui->Con_textEdit_2->append(m_data );

}

void MainWindow::on_Left_pushButton_clicked()
{
    BitRun(m_bit);
}


void MainWindow::on_Con_Clear_pushButton_2_clicked()
{
    ui->Con_textEdit->clear();
    ui->Con_textEdit_2->clear();
    ui->Con_textEdit_3->clear();
    ui->Con_textEdit_3->setFocus();
}

void MainWindow::setValue(int value)
{
    if (value != m_value) {
            m_value = value;
            emit valueChanged(value);
        }
}


void MainWindow::on_dial_valueChanged(int value)
{
    ui->lcdNumber->display(value);
    m_bit = value ;
     BitRun(m_bit);


//    ui->lcdNumber_2->display(value);
}

