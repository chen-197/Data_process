#ifndef DATA_PROCESS_H
#define DATA_PROCESS_H

#include <QMainWindow>
#include <QMessageBox>
#include <iostream>
#include <QFileDialog>
#include <QInputDialog>
#include <fstream>
#include <cmath>
#include <QProcess>

QT_BEGIN_NAMESPACE
namespace Ui { class Data_process; }
QT_END_NAMESPACE

class Data_process : public QMainWindow
{
    Q_OBJECT

public:
    Data_process(QWidget *parent = nullptr);
    ~Data_process();

private slots:
    void on_csv_choose_clicked();

    void show_csvData(QStringList);

    void on_Replace_outliers_clicked();

    void on_ShowFin_clicked();

    void on_Check_Fix_Miss_clicked();

    void on_Delete_Miss_clicked();

    void on_csv_out_clicked();

    void on_Regression_clicked();

private:
    Ui::Data_process *ui;
};
#endif // DATA_PROCESS_H
