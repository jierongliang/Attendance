#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include    <QLabel>
#include    <QString>
#include<QMap>
#include    <Qtsql>
#include    <QDataWidgetMapper>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSqlDatabase  DB;//数据库连接
    QSqlTableModel  *tabModel;  //数据模型
    QItemSelectionModel *theSelection; //选择模型
    QDataWidgetMapper   *dataMapper; //数据映射

//    QSqlTableModel  *tabModel_2;  //数据模型
//    QItemSelectionModel *theSelection_2; //选择模型
//    QDataWidgetMapper   *dataMapper_2; //数据映射
    void calculate();

    void    openTable();//打开数据表
    void    getFieldNames_1();//获取字段名称,填充“排序字段”的comboBox
    void    getFieldNames_2();//获取字段名称,填充“排序字段”的comboBox
//    void    openTable_2();//打开数据表
//    void    getFieldNames_2();//获取字段名称,填充“排序字段”的comboBox

    void showresult();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    //QTableView的SelectionModel发生了变化，进行处理
        void on_currentChanged(const QModelIndex &current, const QModelIndex &previous);

    // QTableView的SelectionModel的行发生了变化，进行处理
        void on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous);



    void on_btn1_clicked();



    void on_btn2_clicked();

    void on_btn6_clicked();

    void on_c2_currentIndexChanged(int index);

    void on_r1_clicked();

    void on_r2_clicked();

    void on_btn7_clicked();

    void on_btn3_clicked();

    void on_btn4_clicked();

    void on_btn5_clicked();

    void on_btn8_clicked();

private:
    Ui::MainWindow *ui;
    bool flag ;
    QMap<QString,int>  map;
};

#endif // MAINWINDOW_H
