#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <cmath>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    
    //,currentInput("0")
{
    ui->setupUi(this);
    QList <QPushButton*> digitButtons = {
        ui->pushButton_0, ui->pushButton_1, ui->pushButton_2, ui->pushButton_3,
        ui->pushButton_4, ui->pushButton_5, ui->pushButton_6, ui->pushButton_7,
        ui->pushButton_8, ui->pushButton_9
    };
    for (auto button : digitButtons) {
        connect(button, &QPushButton::clicked, this, &MainWindow::onDigitClicked);
  
    }
    QString name = "Calculator";
    setWindowTitle(name);
    ui ->lineEdit -> setText("0");
    connect(ui->pushButton_add, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    connect(ui->pushButton_mult, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    connect(ui->pushButton_div, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    connect(ui->pushButton_minus, &QPushButton::clicked, this, &MainWindow::onOperatorClicked);
    connect(ui->pushButton_eq, &QPushButton::clicked, this, &MainWindow::onEqualClicked);
    connect(ui->pushButton_clear, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(ui->pushButton_sqrt, &QPushButton::clicked, this, &MainWindow::onSqrtClicked);
    connect(ui ->pushButton_sqr, &QPushButton::clicked, this, &MainWindow::onSqrClicked);
    connect(ui -> pushButton_madd, &QPushButton::clicked, this, &MainWindow::onMaddClicked);
    connect(ui-> pushButton_mminus, &QPushButton::clicked, this, &MainWindow::onMMinusClicked);
    connect(ui ->pushButton_mc, &QPushButton::clicked, this, &MainWindow::onMCClicked );
    connect(ui -> pushButton_ms, &QPushButton::clicked, this, &MainWindow::onMSClicked );
    connect(ui ->pushButton_mr, &QPushButton::clicked, this, &MainWindow::onMRClicked );
    connect(ui -> pushButton_inverse, &QPushButton::clicked, this, &MainWindow::onInverseClicked);
    connect(ui ->pushButton_dot, &QPushButton::clicked, this, &MainWindow::onDotClicked);
    connect(ui ->pushButton_bs, &QPushButton::clicked, this, &MainWindow::onBackClicked);
    connect(ui -> pushButton_ce, &QPushButton::clicked, this, &MainWindow::onHandleCE);
    connect(ui ->pushButton_pct, &QPushButton::clicked, this, &MainWindow::onHandlepct);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSqrtClicked()
{   
    bool ok;
   double currentNumber = ui ->lineEdit -> text().toDouble(&ok);
   if(currentNumber <0||!ok)
   {
    errorHandle();
    return;
   }   
   if (ok)
   {
     double result = std::sqrt(currentNumber);  

     QString newLine =  QString::number(result);              
     ui->lineEdit->setText(newLine);  
     waitingForOperand = true;
     afterUnary = true; 
    
   } 
      
}



void MainWindow::onSqrClicked()
{    
    bool ok;
    
    double currentNumber = ui ->lineEdit -> text().toDouble(&ok);
    if (ok)
    {
        double result = currentNumber* currentNumber;
        QString currentInput = QString::number(result);
        ui -> lineEdit -> setText(currentInput);   
        waitingForOperand = true;
       afterUnary = true;  
     
    }
    
   
}



void MainWindow::onMaddClicked() {
    bool ok;
    double currentNumber = ui->lineEdit->text().toDouble(&ok);
    
    if (ok) 
    {
        memoryValue += currentNumber;
       
        ui->pushButton_mr->setEnabled(true);
        ui->pushButton_mc -> setEnabled(true);
        waitingForOperand = true;
    }
}

void MainWindow::onMMinusClicked()
{
    bool ok;
    double currentNumber = ui ->lineEdit -> text().toDouble(&ok);
    if (ok)
    {
        memoryValue -= currentNumber;
        ui ->pushButton_mr -> setEnabled(true);
        ui ->pushButton_mc -> setEnabled(true);
      
        waitingForOperand = true;
    }
    
}

void MainWindow::onMCClicked()
{
    memoryValue = 0;
  
    ui ->pushButton_mr -> setEnabled(false);
    ui -> pushButton_mc -> setEnabled(false);
    waitingForOperand = true;

}

void MainWindow::onMSClicked() {
    bool ok;
    double currentNumber = ui->lineEdit->text().toDouble(&ok);
    if (ok) 
    {  
        memoryValue = currentNumber;
      
        ui->pushButton_mr->setEnabled(true); 
        ui->pushButton_mc->setEnabled(true);  
        waitingForOperand = true;
    }
}

void MainWindow::onMRClicked()
{
 
    ui -> lineEdit -> setText(QString::number(memoryValue));
    waitingForOperand = true;
    storedValue = memoryValue;
}

void MainWindow::onInverseClicked()
{    
    bool ok;
    double currentNumber = ui->lineEdit->text().toDouble(&ok);
    if (currentNumber == 0)
    {
        errorHandle();
        return;
    }
    double ret = 1 / currentNumber;
   
    waitingForOperand = true;
    ui ->lineEdit ->setText(QString::number(ret));
    afterUnary = true;   
  
}

void MainWindow::onDotClicked()
{
   
   QString text = ui->lineEdit->text();
    if (text == "Error" || text.contains('.')) 
        return;

    if (waitingForOperand || text.isEmpty()) 
    {
        ui->lineEdit->setText("0.");
        waitingForOperand = false;
        
    }
    else 
    {
        ui->lineEdit->setText(text + '.');
    }
    storedValue = ui ->lineEdit ->text().toDouble();
}

void MainWindow::onBackClicked()
{
    QString displayText = ui->lineEdit->text();
    if (waitingForOperand)
        return;
    if( displayText == "Error"  ||  displayText.isEmpty())
    {
        ui ->lineEdit -> setText("0");
        clearError();
        return;
    } 

    if (displayText.length() == 1)
    {
        ui->lineEdit->setText("0");
        return;
    }
    else
    {
        displayText.chop(1);
        ui->lineEdit ->setText(displayText);
    }
    storedValue = ui->lineEdit ->text().toDouble();
}


void MainWindow::onDigitClicked() 
{
    
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    
    if (!button) 
        return;
    if ( waitingForOperand|| ui ->lineEdit ->text() == "0" || ui->lineEdit->text()== "Error") 
    {
        if ( ui->lineEdit->text()== "Error")
            clearError();
        if (equalClicked)
        {
            storedValue = 0;
            pendingOperator.clear();
            lastOperand = 0;
            equalClicked = false;
           
        }
        ui->lineEdit->setText(button->text());
        
    }
    else 
    {
        QString curr = ui ->lineEdit ->text();
        ui->lineEdit->setText(curr + button -> text());
    }
    waitingForOperand = false;
    afterUnary = false;

}



void MainWindow::onOperatorClicked()
{
    if (ui->lineEdit->text() == "Error")
        return;

    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button)
        return;

    bool ok;
    double currentNumber = ui->lineEdit->text().toDouble(&ok);
    if (!ok)
    {
        errorHandle();
        return;
    }


    if (afterUnary)
    {
        storedValue = currentNumber;
        pendingOperator = button->text();
        afterUnary = false;
        waitingForOperand = true;
        clearedEntry = false;
        equalClicked = false;
        return;
    }

    if (!pendingOperator.isEmpty() && !waitingForOperand && !clearedEntry)
    {
        double result = 0;
        if (pendingOperator == "+")
            result = storedValue + currentNumber;
        else if (pendingOperator == "-")
            result = storedValue - currentNumber;
        else if (pendingOperator == "*")
            result = storedValue * currentNumber;
        else if (pendingOperator == "/")
        {
            if (currentNumber == 0)
            {
                errorHandle();
                return;
            }
            result = storedValue / currentNumber;
        }

        storedValue = result;
        ui->lineEdit->setText(QString::number(result));
    }
    else if (waitingForOperand || clearedEntry)
    {
       
        storedValue = storedValue;
    }
    else
    {
        storedValue = currentNumber;
    }

    pendingOperator = button->text();
    waitingForOperand = true;
    clearedEntry = false;
    equalClicked = false;
    
}


void MainWindow::onEqualClicked() 
{
    if(ui ->lineEdit ->text() == "Error")
    {
        ui->lineEdit->setText("0");
        clearError();
    }


    if(pendingOperator.isEmpty()||ui -> lineEdit -> text().isEmpty())
    {
        return;
    }
    bool ok;
    double rightOperand = 0;
    rightOperand = ui ->lineEdit -> text().toDouble(&ok);
   
    
    if (!ok)
    {
        errorHandle();
        return;
    }
    if (equalClicked)
    {
       rightOperand = lastOperand;
    } else
    {
        lastOperand = rightOperand;
    }
    equalClicked = true;
    double result = 0;
    if (pendingOperator == "+") 
    {
        result = storedValue + rightOperand;
    }else if (pendingOperator == "-") 
    {
        result = storedValue - rightOperand;
    } else if (pendingOperator == "*") 
    {
        result = storedValue * rightOperand;
    } else if (pendingOperator == "/") 
    {
        if (rightOperand != 0) 
        {
            result = storedValue / rightOperand;
        }
        else 
        {
            errorHandle();
            return;
        }

    }
    ui->lineEdit->setText(QString::number(result));
   
    storedValue = result;
    waitingForOperand = true;
    afterUnary = false;
}

void MainWindow::onClearClicked() 
{
    if (ui ->lineEdit ->text() == "Error")
        clearError();
    storedValue = 0;
    pendingOperator.clear();
    ui->lineEdit->setText("0");
    equalClicked = false;
    lastOperand = 0;
    clearedEntry = false;
    afterUnary = false;
}

void MainWindow::errorHandle()
{
    ui ->lineEdit -> setText("Error");
    storedValue = 0;
    pendingOperator.clear();
    waitingForOperand = true;
    ui ->pushButton_div -> setEnabled(false);
    ui -> pushButton_dot ->setEnabled(false);
    ui ->pushButton_inverse -> setEnabled(false);
    ui -> pushButton_add ->setEnabled(false);
    ui -> pushButton_madd ->setEnabled(false);
    ui ->pushButton_minus ->setEnabled(false);
    ui ->pushButton_mminus ->setEnabled(false);
    ui ->pushButton_ms ->setEnabled(false);
    ui->pushButton_mult ->setEnabled(false);
    ui ->pushButton_sqr ->setEnabled(false);
    ui->pushButton_sqrt->setEnabled(false);
    ui ->pushButton_pct -> setEnabled(false);
    lastOperand = 0;
    equalClicked = false;


}

void MainWindow::onHandleCE()
{
  
    if(equalClicked||afterUnary)
    {
        onClearClicked();
        return;
    }
    waitingForOperand = true;
    if (ui->lineEdit->text() == "Error")
        clearError();
    ui->lineEdit ->setText("0");
    clearedEntry = true;
    afterUnary = false;
}

void MainWindow::onHandlepct()
{
    
    waitingForOperand = true;
    QString current = ui ->lineEdit->text();
    bool ok;
    double currentNumber = current.toDouble(&ok);
    if (ok)
    {
        currentNumber *= 0.01;
        ui ->lineEdit -> setText(QString::number(currentNumber));
        storedValue = currentNumber;
    }
    else {
        return;
    }
}


void MainWindow::clearError()
{
    ui ->pushButton_div -> setEnabled(true);
    ui -> pushButton_dot -> setEnabled(true);
    ui ->pushButton_inverse ->  setEnabled(true);
    ui -> pushButton_add -> setEnabled(true);
    ui -> pushButton_madd -> setEnabled(true);
    ui ->pushButton_minus -> setEnabled(true);
    ui ->pushButton_mminus -> setEnabled(true);
    ui ->pushButton_ms -> setEnabled(true);
    ui->pushButton_mult -> setEnabled(true);
    ui ->pushButton_sqr -> setEnabled(true);
    ui->pushButton_sqrt-> setEnabled(true);
    ui ->pushButton_pct -> setEnabled(true);
}