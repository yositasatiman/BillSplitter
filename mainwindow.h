#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>  // สำหรับ vector
#include <QString> // สำหรับ QString
#include <QStackedWidget>
#include <QTableWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>


using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // ฟังก์ชันสำหรับคำนวณ VAT
    double calculateVAT(double totalAmount) {
        return totalAmount * vatAmount / 100;
    }

    // ฟังก์ชันสำหรับคำนวณ Service Charge
    double calculateServiceCharge(double totalAmount) {
        return totalAmount * SerCharge / 100;
    }

    // ฟังก์ชันสำหรับคำนวณ Discount
    double calculateDiscount(double totalAmount) {
        return totalAmount * Discount / 100;
    }

    void calculateExpenses();  // ฟังก์ชันคำนวณค่าใช้จ่าย
    void resetData();  // ฟังก์ชันรีเซ็ตข้อมูล
    void switchToMainPage();  // ฟังก์ชันสำหรับเปลี่ยนกลับไปหน้าหลัก
    void switchToSummaryPage();   // เปลี่ยนไปหน้าสรุป (หน้า 2)


private slots:

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;

    QWidget *page1;
    QWidget *page2;
    QTableWidget *summaryTable;

    QString date;
    std::vector<QString> dish;           // รายการจานอาหาร
    std::vector<double> Price;           // ราคาของจานอาหาร
    int N_member;                   // จำนวนสมาชิก
    std::vector<QString> members;        // รายชื่อสมาชิกทั้งหมด
    std::vector<QString> memberNames;    // รายชื่อสมาชิกที่กรอก
    double vatAmount;               // จำนวน VAT
    double SerCharge;               // จำนวน Service Charge
    double Discount;                // จำนวน Discount

    QLabel *totalExpenseLabel;  // หัวข้อ Total expenses
    QLabel *dateLabel;  // แสดงวันที่ที่ผู้ใช้กรอกจากหน้า 1
    QTextEdit *expenseSummary;  // แสดงรายการค่าใช้จ่ายของแต่ละคน
    QLineEdit *promptPayInput;  // ช่องกรอกหมายเลข PromptPay
    QTextEdit *noteTextEdit;  // ช่องกรอกหมายเหตุ
    QPushButton *backButton;
    QPushButton *nextButton;


    QVector<double> memberPayments;  // ค่าที่แต่ละคนต้องจ่าย
    double totalAmount;  // ค่าใช้จ่ายรวมทั้งหมด


};

#endif // MAINWINDOW_H
