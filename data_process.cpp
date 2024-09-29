#include "data_process.h"
#include "ui_data_process.h"

QStringList strList_one;
QStringList strList_two;
double mean;
double Standard_deviation;

Data_process::Data_process(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Data_process)
{
    ui->setupUi(this);
}

Data_process::~Data_process()
{
    delete ui;
}


void Normal_distribution(QStringList tmpList)
{
    int n = tmpList.count();
    double sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += tmpList[i].toDouble();
    }
    mean = sum / n;
    sum = 0;
    for (int i = 0; i < n; i++)
    {
        double k = tmpList[i].toDouble() - mean;
        sum += k*k;
    }
    Standard_deviation = sqrt(sum / n);
    qDebug() << mean << "," << Standard_deviation;
}

void Data_process::on_csv_choose_clicked()
{
    QString fileLoadName;
    strList_one = QStringList();
    std::string str;
    fileLoadName = QFileDialog::getOpenFileName(this, "选择文件", "./", "CSV文件(*.csv)");
    if (fileLoadName.isEmpty())
    {
        QMessageBox Box;
        Box.setText("没有选中文件!");
        Box.exec();
        return;
    }
    ui->csv_path->setPlainText(fileLoadName);
    std::ifstream fin;
    fin.open(fileLoadName.toLocal8Bit().toStdString(), std::ios::in);
    if (!fin.is_open())
    {
        QMessageBox msgBox;
        msgBox.setText("ERROR!");
        msgBox.exec();
        fin.close();
        return;
    }
    int a = 1;
    while(getline(fin,str))
    {
        qDebug() << "读入第" << a << "行数据";
        strList_one << QString::fromStdString(str);
        a++;
    }
    fin.close();
    strList_two = strList_one;
    //show_csvData(strList_one);

}

void Data_process::show_csvData(QStringList List)
{
    int num = List.count();
    if (num == 0)
    {
        return;
    }
    QStringList tmpList;
    for (int i = 0; i < num; i++)
    {
        qDebug() << "“显示”处理到第" << i+1 << "行内容";
        tmpList = List[i].split(',');
        if (tmpList.count() == 0)
        {
            return;
        }
        for (int s = 0; s < tmpList.count(); s++)
        {
            ui->Output->insertPlainText(tmpList[s] + "  ");
        }
        ui->Output->insertPlainText("\n");
    }
}


void Data_process::on_Replace_outliers_clicked()
{
    int numb = strList_two.count();
    int r = ui->Checkcol->toPlainText().toInt();
    int kl = ui->Checkl->toPlainText().toInt();
    int jm = ui->jmpRow->toPlainText().toInt();
    int xm = ui->Sign_edit->toPlainText().toInt();
    QStringList numList;
    for (int p = 0; p < numb; p++)
    {
        if (p+1 == jm || p+1 == xm)
        {
            continue;
        }
        numList << strList_two[p].split(',')[r-1];
    }
    Normal_distribution(numList);
    QStringList tmpList;
    QString tmpStr;
    for (int i = 0; i < numb; i++)
    {
        tmpList.clear();
        if (i+1 == jm || i+1 == xm)
        {
            continue;
        }
        tmpList = strList_two[i].split(',');
        qDebug() << tmpList;
        if (tmpList[r-1].toDouble() < mean - Standard_deviation * kl || tmpList[r-1].toDouble() > mean + Standard_deviation * kl)
        {
            ui->Output->insertPlainText("检测到第" + QString::number(i+1) + "行数据异常，执行替换..." + "\n");
            tmpList[r-1] = QString::number(mean);
            tmpStr.clear();
            for (int hs = 0; hs < tmpList.count(); hs++)
            {
                tmpStr += tmpList[hs];
                if (hs != tmpList.count() - 1)
                {
                    tmpStr += ",";
                }
            }
            qDebug() << tmpStr;
            strList_two[i] = tmpStr;
        }
    }
}


void Data_process::on_ShowFin_clicked()
{
    show_csvData(strList_two);
}


void Data_process::on_Check_Fix_Miss_clicked()
{
    QString te_1 = ui->Missing->toPlainText();
    int te_2 = ui->Check_miss_col->toPlainText().toInt();
    int numb = strList_two.count();
    int jm = ui->jmpRow->toPlainText().toInt();
    int xm = ui->Sign_edit->toPlainText().toInt();
    QStringList mTRow;
    QStringList numList;
    QString tmpStr;
    for (int p = 0; p < numb; p++)
    {
        if (p+1 == jm || p+1 == xm)
        {
            continue;
        }
        if (strList_two[p].split(',')[te_2-1] == te_1)
        {
            mTRow << QString::number(p);
            continue;
        }
        numList << strList_two[p].split(',')[te_2-1];
    }
    Normal_distribution(numList);
    int nummt = mTRow.count();
    QStringList mTList;
    for (int i = 0; i < nummt; i++)
    {
        int tmi = mTRow[i].toInt();
        mTList = strList_two[tmi].split(',');
        mTList[te_2-1] = QString::number(mean);
        tmpStr.clear();
        for (int hs = 0; hs < mTList.count(); hs++)
        {
            tmpStr += mTList[hs];
            if (hs != mTList.count() - 1)
            {
                tmpStr += ",";
            }
        }
        qDebug() << tmpStr;
        strList_two[tmi] = tmpStr;
    }
}


void Data_process::on_Delete_Miss_clicked()
{
    QString te_1 = ui->Missing->toPlainText();
    int te_2 = ui->Check_miss_col->toPlainText().toInt();
    int jm = ui->jmpRow->toPlainText().toInt();
    int xm = ui->Sign_edit->toPlainText().toInt();
    for (int i = 0; i < strList_two.count(); i++)
    {
        qDebug() << "正在检测目前第" << i+1 << "行内容";
        if (i+1 == jm || i+1 == xm)
        {
            continue;
        }
        if (strList_two[i].split(',')[te_2-1] == te_1)
        {
            qDebug() << "发现目前第" << i+1 << "行内容存在缺失值，执行删除";
            strList_two.removeAt(i);
            i -= 1;
        }
    }
}


void Data_process::on_csv_out_clicked()
{
    bool ifOK = false;
    QString savedFileName = QInputDialog::getText(NULL, "保存", "请输入文件名(不要带有空格!):", QLineEdit::Normal, "", &ifOK);
    if (!ifOK)
    {
        QMessageBox msgBox;
        msgBox.setText("ERROR!");
        msgBox.exec();
        return;
    }
    savedFileName += ".csv";
    std::ofstream fout;
    fout.open(savedFileName.toLocal8Bit().toStdString(), std::ios::out);
    if (!fout.is_open())
    {
        QMessageBox msgBox;
        msgBox.setText("ERROR!");
        msgBox.exec();
        fout.close();
        return;
    }
    int num = strList_two.count();
    if (num == 0)
    {
        return;
    }
    QStringList tmpList;
    for (int i = 0; i < num; i++)
    {
        qDebug() << "处理到第" << i+1 << "行内容";
        tmpList = strList_two[i].split(',');
        if (tmpList.count() == 0)
        {
            return;
        }
        for (int s = 0; s < tmpList.count(); s++)
        {
            fout << tmpList[s].toStdString();
            if (s != tmpList.count() - 1)
            {
                fout << ',';
            }
        }
        fout << '\n';
    }
    fout.close();
}


void Data_process::on_Regression_clicked()
{
    int sg = ui->Sign_edit->toPlainText().toInt();
    QStringList iList;
    iList << "a" << "b" << "c" << "d" << "e" << "f" << "g" << "h" << "i" << "j" << "k" << "l" << "m" << "n";
    QStringList aList = ui->Independent_variable->toPlainText().split(',');
    int aco = aList.count();
    QString* sStr = new QString[aco];
    QString yStr;
    int ssum = strList_two.count();
    for (int i = 0; i < ssum; i++)
    {
        if (i+1 == sg)
        {
            continue;
        }
        for (int s = 0; s < aco; s++)
        {
            sStr[s] += strList_two[i].split(',')[aList[s].toInt() - 1];
            if (i != ssum - 1)
            {
                sStr[s] += ",";
            }
        }
        yStr += strList_two[i].split(',')[ui->Dependent_variable->toPlainText().toInt() - 1];
        if (i != ssum - 1)
        {
            yStr += ",";
        }
    }
    std::ofstream fout;
    fout.open("test.R", std::ios::out);
    if (!fout.is_open())
    {
        QMessageBox msgBox;
        msgBox.setText("ERROR!");
        msgBox.exec();
        fout.close();
        return;
    }
    for (int i = 0; i < aco; i++)
    {
        fout << iList[i].toStdString() << "=c(" << sStr[i].toStdString() << ")" << "\n";
    }
    fout << "y=c(" << yStr.toStdString() << ")" << "\n" << "coef(lm(y~";
    for (int i = 0; i < aco; i++)
    {
        fout << iList[i].toStdString();
        if (i != aco - 1)
        {
            fout << "+";
        }
    }
    fout << "))";
    fout.close();
    delete [] sStr;
    QProcess rProcess;
    QStringList rpList;
    rpList << "./test.R";
    rProcess.start("C:/Program Files/R/R-4.2.2/bin/Rscript.exe",rpList);
    rProcess.waitForStarted();
    rProcess.waitForReadyRead();
    ui->Output->insertPlainText(rProcess.readAll());
    rProcess.kill();
    rProcess.waitForFinished();
}

