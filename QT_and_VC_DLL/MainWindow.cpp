#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "D:/Sanjay/Development/qrcdll/san.h"

#include <QString>
#include <QDebug>





MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Get Intiger from QTC++ and send to Vc++ , Return the data back to QTC++
    int result = AddNumbers(15, 5);

    QString resultText = "Total Number " + QString::number(result);
    ui->textEdit->append(resultText);

    //Call VC++ Dll function and return string to QTc++
    QString hardDriveInfo;
    const char* hardDriveInfoC = GetHardDriveInfo();
    hardDriveInfo = QString::fromLatin1(hardDriveInfoC); // Convert the C-style string to QString
    ui->textEdit->append(hardDriveInfo);


    // Take a Qstring as parameter , send to Vc++ DLL and the Send String to QTC++
    QString inputString = "Hello, World!";
    const char* inputC = inputString.toUtf8().constData(); // Convert QString to const char*

    const char* resultC = ProcessString(inputC); // Call the DLL function

    // Convert the result const char* back to QString
    QString resultString = QString::fromUtf8(resultC);
    ui->textEdit->append(resultString) ;


    //Wrapper
    char volumeName[1024] = {0};
    DWORD serialNumber = 0;
    DWORD maxComponentLen = 0;
    DWORD fileSystemFlags = 0;

    const char* driveLetter = "C:\\"; // Replace with the desired drive letter

    if (GetVolumeInfo(driveLetter, volumeName, sizeof(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags))
    {

        ui->textEdit->append( QString("Volume Name: ") + QString(volumeName));
        ui->textEdit->append( QString("Serial Number: ") + QString::number(serialNumber));
        ui->textEdit->append( QString("Max Component Length: ") + QString::number(maxComponentLen));
        ui->textEdit->append( QString("File System Flags: ") + QString::number(fileSystemFlags));
    } else {
        qDebug() << "Error getting volume information.";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Preseeing SCAN button
void MainWindow::on_pushButton_clicked()
{
    QString rpc_ip = ui->txt_IP_Remote->text();
    QString rpc_user = ui->txtIP_user->text();
    QString rpc_pass = ui->txtIP_password->text();

    std::wstring rpc_ip_w = rpc_ip.toStdWString();
    std::wstring rpc_user_w = rpc_user.toStdWString();
    std::wstring rpc_pass_w = rpc_pass.toStdWString();

    LPCWSTR con_rpc_ip = rpc_ip_w.c_str();
    LPCWSTR con_rpc_user = rpc_user_w.c_str();
    LPCWSTR con_rpc_pass = rpc_pass_w.c_str();

    UserInformation userInfo = RPCShareEnum(con_rpc_ip, con_rpc_user, con_rpc_pass);

    ui->textEdit->append("----------------");
    ui->textEdit->append("Password : " + QString::fromWCharArray(userInfo.password));
    ui->textEdit->append("User Name : " + QString::fromWCharArray(userInfo.userName));
    ui->textEdit->append("URL : " +QString::fromWCharArray(userInfo.url));
    ui->textEdit->append("Password Age (Seconds): " + QString::number(userInfo.passwd_age));
    ui->textEdit->append("Password Level : " + QString::number(userInfo.privilege_level));
  //  ui->textEdit->append("Home Dir : " + QString::fromWCharArray(userInfo.home_directory));


    ui->textEdit->append("----------------");


//    QString logonHoursHex;
//    for (int i = 0; i < 21; i++) {
//        logonHoursHex += QString("%1 ").arg((int)userInfo.logonHours[i], 2, 16, QChar('0'));
//    }

    // Append the logonHoursHex to the QTextEdit
    //ui->textEdit->append("Logon hours : " + logonHoursHex);




}

