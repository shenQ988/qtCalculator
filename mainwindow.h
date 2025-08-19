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

private:
    Ui::MainWindow *ui;
    double storedValue = 0;
    QString pendingOperator;
    bool waitingForOperand = false;
    double memoryValue = 0;
    double lastOperand = 0;
    bool equalClicked = false;
    bool clearedEntry = false;
    bool afterUnary = false;
    
    

private slots:

    void onDigitClicked();
    void onOperatorClicked();
    void onEqualClicked();
    void onClearClicked();
    void onSqrtClicked();
    void onSqrClicked();
    void onMaddClicked();
    void onMMinusClicked();
    void onMCClicked();
    void onMSClicked();
    void onMRClicked();
    void onInverseClicked();
    void onDotClicked();
    void onBackClicked();
    void errorHandle();
    void onHandleCE();
    void onHandlepct();
    void clearError();
   

};
#endif // MAINWINDOW_H