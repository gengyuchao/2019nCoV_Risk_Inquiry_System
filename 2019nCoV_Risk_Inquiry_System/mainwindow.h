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
    void read_source_csvfile(QString File_path,QList<QStringList> *dst_List);
    void check_same_transpotation(QString info);

private slots:
    void on_pushButton_clicked();

    void on_Filter_Add_BTN_clicked();

    void on_Batch_query_BTN_clicked();

    void on_Save_BTN_clicked();

    void on_Show_Result_BTN_clicked();

    void on_Show_Source_BTN_clicked();

    void on_Source_Path_Set_clicked();

    void on_Check_File_Path_BTN_clicked();

    void on_Source_Path_Set_BTN_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
