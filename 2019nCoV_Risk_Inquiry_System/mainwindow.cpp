#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QFile"
#include "QMessageBox"
#include "QTextStream"
#include "QFileDialog"

#include "QList"


QList<QStringList> transportation_info;

QList<QStringList> Employee_info;

int check_item=1;

//排列判断
int compare(const QStringList &infoA,QStringList &infoB)
{
    return infoA.at(check_item) < infoB.at(check_item);
}

QString Source_File = "目前统计疫情相关乘车方式.csv";
QString Check_File  = "批量查询数据源.csv";
QString Result_File = "查询结果.csv";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    on_Show_Source_BTN_clicked();
    this->update();

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::read_source_csvfile(QString File_path,QList<QStringList> *dst_List) //读取csv
{
    QFile csvFile(File_path);//"/home/gengyuchao/esp/Qt_project/a.csv"
    QStringList csvList;
    csvList.clear();
    if (csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
    {
        QTextStream stream(&csvFile);
        while (!stream.atEnd())
        {
            csvList.push_back(stream.readLine()); //保存到List当中
        }
        csvFile.close();
    }
    else
    {
        QMessageBox::about(NULL, "csv文件", "未打开该文件！");
    }

    if(csvList.isEmpty())
    {
        QMessageBox::about(NULL, "csv文件", "文件为空!");
        return;
    }

    for(int i=0;i<csvList.size();i++)
    {
        QStringList valsplit = csvList.at(i).split(","); //分隔字符串
        {
            dst_List->append(valsplit);
        }
    }
    ui->comboBox->addItems(dst_List->at(0));
}

void MainWindow::check_same_transpotation(QString check_info)
{
    int error_code = 0;
    QStringList valsplit = check_info.split(",");
//    for(int i=0;i<valsplit.size()/2;i++)
//    {
//        ui->textEdit->append(QString::number(i)+":"+ valsplit.at(2*i)+" "+ valsplit.at(2*i+1));
//    }

    for(int i=0;i<transportation_info.size();i++)
    {
        bool match_flag = true;
        for(int k=0;k<valsplit.size()/2;k++)
        {
            //ui->textEdit->append(QString::number(k)+":"+QString::number(transportation_info.at(0).indexOf(valsplit.at(k)))+" ");
            if(transportation_info.at(0).indexOf(valsplit.at(2*k))!=-1 &&
               valsplit.at(2*k+1).isNull()==false &&valsplit.at(2*k+1).isEmpty()==false)
            {
                if(transportation_info.at(i).at(transportation_info.at(0).indexOf(valsplit.at(2*k))).toUpper().indexOf(valsplit.at(2*k+1).toUpper())==-1)
                {
                    match_flag = false;
                    break;
                }
            }
            else
            {
                error_code |= ((valsplit.at(2*k+1).isNull()<<0)+(valsplit.at(2*k+1).isEmpty()<<1));
//                goto err_handle;
            }

        }
        if(match_flag)
        {
            //QMessageBox::about(NULL, "Got", transportation_info.at(i).join(","));
            ui->textEdit->append("\n查找到:,"+check_info.replace(","," "));
            ui->textEdit->append(transportation_info.at(i).join(","));
        }

    }

err_handle:
    {
        if((error_code & 0x01) == 0x01)
        {
            QMessageBox::warning(NULL, "Check Information", "Formate Error!");
        }
        if((error_code & 0x02) == 0x02)
        {
            QMessageBox::warning(NULL, "Check Information", "存在缺省数据!");
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString check_info=ui->lineEdit->text();
    ui->textEdit->clear();
    check_same_transpotation(check_info);
}

void MainWindow::on_Filter_Add_BTN_clicked()
{
    QString Add_Filter_str = ui->comboBox->currentText()+","+ui->lineEdit_Filter->text();
    if(ui->lineEdit->text().isEmpty()==false)
    {
        ui->lineEdit->setText(ui->lineEdit->text()+","+ Add_Filter_str);
    }
    else
    {
        ui->lineEdit->setText(Add_Filter_str);
    }


}

void MainWindow::on_Batch_query_BTN_clicked()
{
    Check_File = ui->lineEdit_Check_File->text();
    QFile csvFile(Check_File);
    QStringList csvList;
    csvList.clear();
    if (csvFile.open(QIODevice::ReadWrite)) //对csv文件进行读写操作
    {
        QTextStream stream(&csvFile);
        while (!stream.atEnd())
        {
            csvList.push_back(stream.readLine()); //保存到List当中
        }
        csvFile.close();
    }
    else
    {
        QMessageBox::about(NULL, "csv文件", "未打开该文件！");
    }

    if(csvList.isEmpty())
    {
        QMessageBox::about(NULL, "csv文件", "该文件空！");
        return;
    }

//    Q_FOREACH(QString str, csvList)   //遍历List
//    {
//        ui->textEdit->append(str);
//    }

    QStringList Head_valsplit = csvList.first().split(","); //分隔字符串
    Q_FOREACH(QString str, csvList)   //遍历List
    {
        if(str!=csvList.first())
        {
            QStringList valsplit = str.split(","); //分隔字符串
            QString check_info;
            for(int i=0;i<valsplit.size();i++)
            {
                check_info += Head_valsplit.at(i)+","+valsplit.at(i)+",";
            }
            check_same_transpotation(check_info);
        }

    }

    on_Save_BTN_clicked();
    on_Show_Result_BTN_clicked();
}

void MainWindow::on_Save_BTN_clicked()
{
    QFile csvFile(Result_File);
    if(transportation_info.isEmpty())
    {
        QMessageBox::about(NULL, "csv文件", "无可存储数据！");
        return;
    }
    if (csvFile.open(QIODevice::ReadWrite))
    {
        csvFile.resize(0);
        QString Save_Text = (transportation_info.at(0).join(",") + "\n") ;
        csvFile.write(Save_Text.toUtf8(),Save_Text.toUtf8().size());
        Save_Text.clear();
        Save_Text = ui->textEdit->toPlainText();

        csvFile.write(Save_Text.toUtf8(),Save_Text.toUtf8().size());

        csvFile.close();
    }
}

void MainWindow::on_Show_Result_BTN_clicked()
{
    ui->tableWidget->clear();
    Employee_info.clear();
    read_source_csvfile(Result_File,&Employee_info);
    ui->tableWidget->setColumnCount(Employee_info.at(0).size());
    ui->tableWidget->setRowCount(Employee_info.size());

    for(int i=0;i<Employee_info.size();i++)
    {
        for(int j=0;j<Employee_info.at(i).size();j++)
        {
            QTableWidgetItem *new_item = new QTableWidgetItem;
            new_item->setText(Employee_info.at(i).at(j));
            ui->tableWidget->setItem(i,j,new_item);
        }
    }
}

void MainWindow::on_Show_Source_BTN_clicked()
{
    ui->tableWidget->clear();
    transportation_info.clear();
    read_source_csvfile(Source_File,&transportation_info);
    ui->tableWidget->setColumnCount(transportation_info.at(0).size());
    ui->tableWidget->setRowCount(transportation_info.size());
    qSort(transportation_info.begin()+1,transportation_info.end(),compare);

    for(int i=0;i<transportation_info.size();i++)
    {
        for(int j=0;j<transportation_info.at(i).size();j++)
        {
            QTableWidgetItem *new_item = new QTableWidgetItem;
            new_item->setText(transportation_info.at(i).at(j));
            ui->tableWidget->setItem(i,j,new_item);
        }
    }
}

void MainWindow::on_Source_Path_Set_clicked()
{
    QString Path;
    if(ui->lineEdit_source_Path->text().isEmpty())
    {
        Path = QFileDialog::getOpenFileName(this,tr("Open Source File"),".",tr("csv (*.csv)"));
    }
    else
    {
        Path = QFileDialog::getOpenFileName(this,tr("Open Source File"),ui->lineEdit_source_Path->text(),tr("csv (*.csv)"));
    }
    if(Path.isEmpty()==false)
        Source_File = Path;
    ui->lineEdit_source_Path->setText(Source_File);
    on_Show_Source_BTN_clicked();
}

void MainWindow::on_Check_File_Path_BTN_clicked()
{
    Check_File = QFileDialog::getOpenFileName(this);
    ui->lineEdit_Check_File->setText(Check_File);
}

void MainWindow::on_Source_Path_Set_BTN_clicked()
{
    Source_File = ui->lineEdit_source_Path->text();
    on_Show_Source_BTN_clicked();
}
