#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QDialog>  // สำหรับหน้าต่างใหม่
#include <QLineEdit>  // สำหรับกล่องข้อความแบบแถวเดียว
#include <QLabel>
#include <QFont>
#include <QtCore>
#include <QStackedWidget>
#include <QIntValidator>
#include <QDoubleValidator>
#include <vector>  // สำหรับ vector
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QDebug>  // เพิ่ม header นี้สำหรับ debug


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setFixedSize(600, 500);  // ตั้งขนาดหน้าต่างหลัก
    this->setFixedSize(1280, 720);  // ตั้งขนาดหน้าต่างหลัก
    this->setWindowTitle("Bill Splitter");
    this->setStyleSheet("background-color: #FF69B4; color: #FFFFFF;");


    // ใช้ QStackedWidget เพื่อสร้างหลายหน้า
    stackedWidget = new QStackedWidget(this);
    page1 = new QWidget();
    page2 = new QWidget();

    stackedWidget->addWidget(page1);

    // ตรวจสอบว่า page2 มีอยู่แล้วหรือไม่ ก่อนเพิ่ม
    if (stackedWidget->indexOf(page2) == -1) {
        stackedWidget->addWidget(page2);
        qDebug() << "page2 added to stackedWidget";
    } else {
        qDebug() << "page2 already exists in stackedWidget";
    }

    setCentralWidget(stackedWidget);






    // === PAGE 1 (หน้าหลัก) ===
    // เพิ่มชื่อแอพ BillSplitter ที่หน้าต่างหลัก
    QLabel *appNameLabel = new QLabel("Bill Splitter", page1);
    QFont font = appNameLabel->font();
    font.setPointSize(40);
    appNameLabel->setFont(font);
    appNameLabel->resize(300, 80);
    appNameLabel->move(500, 40);  // ตั้งตำแหน่งของชื่อแอพ
    appNameLabel->setAlignment(Qt::AlignCenter);  // จัดข้อความตรงกลาง
    appNameLabel->setStyleSheet("background-color: #FF69B4;");


    QLabel *appNameLabel2 = new QLabel("Bill Splitter", page2);
    appNameLabel2->setFont(font);
    appNameLabel2->resize(300, 80);
    appNameLabel2->move(500, 40);
    appNameLabel2->setAlignment(Qt::AlignCenter);
    //appNameLabel2->setStyleSheet("background-color: lightgreen;");


    QFont font2 = appNameLabel2->font();
    font2.setPointSize(16);

    // สร้างปุ่ม Add Date
    QPushButton *button1 = new QPushButton("Add Date", page1);
    button1->setFont(font2);
    button1->resize(220, 50);
    button1->move(530, 150);
    //button1->setStyleSheet("background-color: lightgreen; color: black;"); // เปลี่ยนสีพื้นหลังเป็นสีน้ำเงินอ่อนและข้อความเป็นสีดำ
    button1->setStyleSheet("border: 2px solid #000000; background-color: #FFFFCC; color: #000000;");

    // สร้างปุ่ม Add Dish
    QPushButton *button2 = new QPushButton("Add Dish", page1);
    button2->setFont(font2);
    button2->resize(220, 50);
    button2->move(530, 220);
    button2->setStyleSheet("border: 2px solid #000000; background-color: #FFFFCC; color: #000000;");


    // สร้างปุ่ม Add VAT
    QPushButton *button3 = new QPushButton("Add VAT", page1);
    button3->setFont(font2);
    button3->resize(220, 50);
    button3->move(530, 290);
    vatAmount = 0.0;
    button3->setStyleSheet("border: 2px solid #000000; background-color: #FFFFCC; color: #000000;");


    // สร้างปุ่ม Add Service Charge
    QPushButton *button4 = new QPushButton("Add Service Charge", page1);
    button4->setFont(font2);
    button4->resize(220, 50);
    button4->move(530, 360);
    SerCharge = 0.0;
    button4->setStyleSheet("border: 2px solid #000000; background-color: #FFFFCC; color: #000000;");


    // สร้างปุ่ม Add Discount
    QPushButton *button5 = new QPushButton("Add Discount", page1);
    button5->setFont(font2);
    button5->resize(220, 50);
    button5->move(530, 430);
    Discount = 0.0;
    button5->setStyleSheet("border: 2px solid #000000; background-color: #FFFFCC; color: #000000;");


    QPushButton *button_N1 = new QPushButton("Next ->", page1);
    button_N1->setFont(font2);
    button_N1->resize(220, 50);
    button_N1->move(1000, 630);
    button_N1->setStyleSheet("border: 2px solid #000000; background-color: #CCCCFF; color: #000066;");

    /*QPushButton *button_B2 = new QPushButton("<- Back", page2);
    button_B2->setFont(font2);
    button_B2->resize(220, 50);
    button_B2->move(50, 630);
    button_B2->setStyleSheet("border: 2px solid #000000; background-color: #CCCCFF; color: #000066;");*/

    // ใช้ lambda function เพื่อเปิดหน้าต่าง Add Date
    connect(button1, &QPushButton::clicked, page1, [=]() {
        QDialog *nextWindow = new QDialog(this);
        nextWindow->resize(500,350);
        nextWindow->setWindowTitle("Add Date");

        QLabel *label = new QLabel("Add Date", nextWindow);
        label->setAlignment(Qt::AlignCenter);  // จัดข้อความตรงกลาง
        QFont font = label->font();
        font.setPointSize(20);
        label->setFont(font);
        label->move(190, 70);

        QLineEdit *lineEdit = new QLineEdit(nextWindow);
        lineEdit->resize(300,40);
        lineEdit->move(100, 130);
        lineEdit->setPlaceholderText("XX/XX/XXXX");
        lineEdit->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");

        QPushButton *button_Ok = new QPushButton("Ok", nextWindow);
        button_Ok->resize(100,40);
        button_Ok->move(300,200);
        button_Ok->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

        QPushButton *button_Close = new QPushButton("Close", nextWindow);
        button_Close->resize(100,40);
        button_Close->move(100,200);
        button_Close->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");

        button_Ok->setEnabled(false);  // ปิดปุ่ม "OK" ในตอนแรก จนกว่าจะมีข้อความ

        // ตรวจสอบว่ามีข้อความใน QLineEdit หรือไม่
        connect(lineEdit, &QLineEdit::textChanged, this, [=]() {
            button_Ok->setEnabled(!lineEdit->text().isEmpty());
        });

        // เมื่อกดปุ่ม OK ให้บันทึกวันที่และอัปเดต dateLabel
        connect(button_Ok, &QPushButton::clicked, this, [=]() {
            date = lineEdit->text();
            dateLabel->setText("บิลวันที่: " + date);  // อัปเดตวันที่ให้แสดงในหน้าที่ 2
            lineEdit->clear();
            nextWindow->accept();
        });
        connect(button_Close, &QPushButton::clicked, nextWindow, &QDialog::reject);

        nextWindow->exec();
    });

    // ใช้ lambda function เพื่อเปิดหน้าต่าง Add Dish
    connect(button2, &QPushButton::clicked, page1, [=]() {
        QDialog *nextWindow = new QDialog(this);
        nextWindow->resize(500,350);
        nextWindow->setWindowTitle("Add Dish");

        QLabel *label = new QLabel("Add Dish", nextWindow);
        label->setAlignment(Qt::AlignCenter);  // จัดข้อความตรงกลาง
        QFont font = label->font();
        font.setPointSize(20);
        label->setFont(font);
        label->move(190,70);

        QLineEdit *lineEdit = new QLineEdit(nextWindow);
        lineEdit->resize(300,40);
        lineEdit->move(100, 130);
        lineEdit->setPlaceholderText("Put your dish");
        lineEdit->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");

        QPushButton *button_Ok = new QPushButton("Ok", nextWindow);
        button_Ok->resize(100,40);
        button_Ok->move(300,200);
        button_Ok->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

        QPushButton *button_Close = new QPushButton("Close", nextWindow);
        button_Close->resize(100,40);
        button_Close->move(100,200);
        button_Close->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");

        button_Ok->setEnabled(false);

        connect(lineEdit, &QLineEdit::textChanged, this, [=]() {
            button_Ok->setEnabled(!lineEdit->text().isEmpty());
        });

        // เชื่อมต่อปุ่ม "OK" ให้เปิดหน้าต่างย่อยใหม่และปิดหน้าต่างแรก
        connect(button_Ok, &QPushButton::clicked, nextWindow, [=]() {
            QString dishName = lineEdit->text();
            if (!dishName.isEmpty()) {
                dish.push_back(dishName);
                lineEdit->clear();
            }

            // สร้างหน้าต่างย่อยใหม่อีกอัน (ต่างจากหน้าหลัก)
            QDialog *Dialog_P = new QDialog(this);
            Dialog_P->resize(500,350);
            Dialog_P->setWindowTitle("Add Price");

            QLabel *Label2 = new QLabel("Add Price", Dialog_P);
            Label2->setAlignment(Qt::AlignCenter);  // จัดข้อความตรงกลาง
            QFont infont2 = Label2->font();
            infont2.setPointSize(20);
            Label2->setFont(infont2);
            Label2->move(190,70);

            QLineEdit *lineEdit_P = new QLineEdit(Dialog_P);
            lineEdit_P->resize(300,40);
            lineEdit_P->move(100, 130);
            lineEdit_P->setPlaceholderText("Put price of dish");
            lineEdit_P->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");

            QDoubleValidator *validator = new QDoubleValidator(0.00, 99.99, 2, this);
            lineEdit_P->setValidator(validator);

            QPushButton *button_Ok2 = new QPushButton("Ok", Dialog_P);
            button_Ok2->resize(100,40);
            button_Ok2->move(300,200);
            button_Ok2->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

            QPushButton *button_Close2 = new QPushButton("Close", Dialog_P);
            button_Close2->resize(100,40);
            button_Close2->move(100,200);
            button_Close2->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");

            button_Ok2->setEnabled(false);

            connect(lineEdit_P, &QLineEdit::textChanged, this, [=]() {
                button_Ok2->setEnabled(!lineEdit_P->text().isEmpty());
            });

            connect(button_Ok2, &QPushButton::clicked, this, [=]() {
                bool ok;
                double enteredValue = lineEdit_P->text().toDouble(&ok);

                if (ok) {
                    Price.push_back(enteredValue);
                    lineEdit_P->clear();
                    Dialog_P->accept();

                    QDialog *Window_Nm = new QDialog(this);
                    Window_Nm->resize(500,350);
                    Window_Nm->setWindowTitle("Number of People");

                    QLabel *Label3 = new QLabel("How Many People", Window_Nm);
                    Label3->setAlignment(Qt::AlignCenter);
                    QFont infont3 = Label3->font();
                    infont3.setPointSize(20);
                    Label3->setFont(infont3);
                    Label3->move(140,70);

                    QLineEdit *lineEdit_Nm = new QLineEdit(Window_Nm);
                    lineEdit_Nm->resize(300,40);
                    lineEdit_Nm->move(100, 130);
                    lineEdit_Nm->setPlaceholderText("Put number of people");
                    lineEdit_Nm->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");

                    QIntValidator *validator2 = new QIntValidator(0, 99, this);
                    lineEdit_Nm->setValidator(validator2);

                    QPushButton *button_Ok3 = new QPushButton("Ok", Window_Nm);
                    button_Ok3->resize(100, 40);
                    button_Ok3->move(300,200);
                    button_Ok3->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

                    QPushButton *button_Close3 = new QPushButton("Close", Window_Nm);
                    button_Close3->resize(100,40);
                    button_Close3->move(100,200);
                    button_Close3->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");

                    button_Ok3->setEnabled(false);

                    connect(lineEdit_Nm, &QLineEdit::textChanged, this, [=]() {
                        button_Ok3->setEnabled(!lineEdit_Nm->text().isEmpty());
                    });

                    connect(button_Ok3, &QPushButton::clicked, this, [=]() {
                        bool ok2;
                        int enteredValue2 = lineEdit_Nm->text().toInt(&ok2);
                        N_member = enteredValue2;
                        lineEdit_Nm->clear();
                        Window_Nm->accept();

                        if (N_member > 0 && N_member < 21) {
                            QDialog *Window_x = new QDialog(this);
                            Window_x->resize(500,600);
                            Window_x->setWindowTitle("Enter Member Names");

                            /*QLabel *Label3 = new QLabel("Enter Member Names", Window_Nm);
                        Label3->setAlignment(Qt::AlignCenter);
                        QFont infont3 = Label3->font();
                        infont3.setPointSize(20);
                        Label3->setFont(infont3);*/

                            //QVBoxLayout *layout = new QVBoxLayout(Window_x);
                            //layout->addWidget(Label3);

                            std::vector<QLineEdit*> nameInputs;

                            // สร้าง QLineEdit สำหรับแต่ละสมาชิก
                            for (int i = 0; i < N_member; ++i) {
                                QLineEdit *nameInput = new QLineEdit(Window_x);
                                nameInput->resize(300,20);
                                nameInput->move(100, 70 + 22*i);
                                nameInput->setPlaceholderText(QString("Enter name of member %1").arg(i + 1));
                                nameInputs.push_back(nameInput);
                                nameInput->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");
                            }

                            QPushButton *button_OkNames = new QPushButton("Ok", Window_x);
                            button_OkNames->resize(100, 40);
                            button_OkNames->move(300,540);
                            button_OkNames->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

                            QPushButton *button_CloseNames = new QPushButton("Close", Window_x);
                            button_CloseNames->resize(100,40);
                            button_CloseNames->move(100,540);
                            button_CloseNames->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");
                            //layout->addWidget(button_OkNames);
                            //layout->addWidget(button_CloseNames);

                            button_OkNames->setEnabled(false);

                            // ตรวจสอบว่าช่องกรอกข้อมูลครบหรือไม่
                            for (auto nameInput : nameInputs) {
                                connect(nameInput, &QLineEdit::textChanged, this, [=]() {
                                    bool allFilled = true;
                                    for (auto nameInput : nameInputs) {
                                        if (nameInput->text().isEmpty()) {
                                            allFilled = false;
                                            break;
                                        }
                                    }
                                    button_OkNames->setEnabled(allFilled);
                                });
                            }

                            // บันทึกชื่อสมาชิก
                            connect(button_OkNames, &QPushButton::clicked, this, [=]() {
                                memberNames.clear();
                                for (auto nameInput : nameInputs) {
                                    memberNames.push_back(nameInput->text());
                                }
                                Window_x->accept();
                            });

                            connect(button_CloseNames, &QPushButton::clicked, Window_Nm, &QDialog::reject);

                            Window_x->exec();
                        }
                        else{
                            QMessageBox::warning(this, "Warning", "Please enter no. 1-20 only!");
                            //return;
                        }

                    });
                    connect(button_Close3, &QPushButton::clicked, Window_Nm, &QDialog::reject);
                    Window_Nm->exec();

                    delete Window_Nm;

                } else {
                    QMessageBox::warning(this, "Invalid Input", "Please enter a valid number.");
                }

            });
            connect(button_Close2, &QPushButton::clicked, this, [=]() {
                Price.push_back(0.0);
                lineEdit_P->clear();
                Dialog_P->accept();
            });

            // ปิดหน้าต่างย่อยแรก
            nextWindow->accept();  // ปิดหน้าต่างแรกก่อนเปิดหน้าต่างใหม่

            Dialog_P->exec();  // แสดงหน้าต่างย่อยที่สอง

            delete Dialog_P;

        });
        connect(button_Close, &QPushButton::clicked, nextWindow, &QDialog::reject);

        nextWindow->exec();

    });

    // ใช้ lambda function เพื่อเปิดหน้าต่าง Add VAT
    connect(button3, &QPushButton::clicked, page1, [=]() {
        QDialog *nextWindow = new QDialog(this);
        nextWindow->resize(500,350);
        nextWindow->setWindowTitle("Add VAT");

        QLabel *label = new QLabel("Add VAT", nextWindow);
        label->setAlignment(Qt::AlignCenter);  // จัดข้อความตรงกลาง
        QFont font = label->font();
        font.setPointSize(20);
        label->setFont(font);
        label->move(190, 70);

        QLineEdit *lineEdit = new QLineEdit(nextWindow);
        lineEdit->resize(300,40);
        lineEdit->move(100, 130);
        lineEdit->setPlaceholderText("Put number only");
        lineEdit->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");

        QDoubleValidator *validator = new QDoubleValidator(0.00, 99.99, 2, this);
        lineEdit->setValidator(validator);

        QPushButton *button_Ok = new QPushButton("Ok", nextWindow);
        button_Ok->resize(100,40);
        button_Ok->move(300,200);
        button_Ok->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

        QPushButton *button_Close = new QPushButton("Close", nextWindow);
        button_Close->resize(100,40);
        button_Close->move(100,200);
        button_Close->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");

        button_Ok->setEnabled(false);

        connect(lineEdit, &QLineEdit::textChanged, this, [=]() {
            button_Ok->setEnabled(!lineEdit->text().isEmpty());
        });

        connect(button_Ok, &QPushButton::clicked, this, [=]() {
            bool ok;
            double enteredValue = lineEdit->text().toDouble(&ok);  // แปลงข้อความเป็น int

            if (ok) {
                vatAmount = enteredValue;  // เก็บค่าตัวเลขในตัวแปร memberValue
                lineEdit->clear();  // เคลียร์ข้อความใน QLineEdit
                nextWindow->accept();  // ปิดหน้าต่างย่อย
            } else {
                // ถ้าผู้ใช้กรอกค่าที่ไม่ใช่ตัวเลข
                QMessageBox::warning(this, "Invalid Input", "Please enter a valid number.");
            }
        });
        connect(button_Close, &QPushButton::clicked, nextWindow, &QDialog::reject);

        nextWindow->exec();
    });

    // ใช้ lambda function เพื่อเปิดหน้าต่าง Add Service Charge
    connect(button4, &QPushButton::clicked, page1, [=]() {
        QDialog *nextWindow = new QDialog(this);
        nextWindow->resize(500,350);
        nextWindow->setWindowTitle("Add Service Charge");

        QLabel *label = new QLabel("Add Service Charge", nextWindow);
        label->setAlignment(Qt::AlignCenter);  // จัดข้อความตรงกลาง
        QFont font = label->font();
        font.setPointSize(20);
        label->setFont(font);
        label->move(140,70);

        QLineEdit *lineEdit = new QLineEdit(nextWindow);
        lineEdit->resize(300,40);
        lineEdit->move(100, 130);
        lineEdit->setPlaceholderText("Put number only");
        lineEdit->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");

        QDoubleValidator *validator = new QDoubleValidator(0.00, 99.99, 2, this);
        lineEdit->setValidator(validator);

        QPushButton *button_Ok = new QPushButton("Ok", nextWindow);
        button_Ok->resize(100,40);
        button_Ok->move(300,200);
        button_Ok->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

        QPushButton *button_Close = new QPushButton("Close", nextWindow);
        button_Close->resize(100,40);
        button_Close->move(100,200);
        button_Close->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");

        button_Ok->setEnabled(false);

        connect(lineEdit, &QLineEdit::textChanged, this, [=]() {
            button_Ok->setEnabled(!lineEdit->text().isEmpty());
        });

        connect(button_Ok, &QPushButton::clicked, this, [=]() {
            bool ok;
            double enteredValue = lineEdit->text().toDouble(&ok);

            if (ok) {
                SerCharge = enteredValue;
                lineEdit->clear();
                nextWindow->accept();
            } else {
                QMessageBox::warning(this, "Invalid Input", "Please enter a valid number.");
            }
        });
        connect(button_Close, &QPushButton::clicked, nextWindow, &QDialog::reject);

        nextWindow->exec();
    });

    connect(button5, &QPushButton::clicked, page1, [=]() {
        QDialog *nextWindow = new QDialog(this);
        nextWindow->resize(500,350);
        nextWindow->setWindowTitle("Add Discount");

        QLabel *label = new QLabel("Add Discount", nextWindow);
        label->setAlignment(Qt::AlignCenter);  // จัดข้อความตรงกลาง
        QFont font = label->font();
        font.setPointSize(20);
        label->setFont(font);
        label->move(170,70);

        QLineEdit *lineEdit = new QLineEdit(nextWindow);
        lineEdit->resize(300,40);
        lineEdit->move(100, 130);
        lineEdit->setPlaceholderText("Put number only");
        lineEdit->setStyleSheet("border: 2px solid #000000; background-color: #FFF0F5; color: #000066;");

        QDoubleValidator *validator = new QDoubleValidator(0.00, 99.99, 2, this);
        lineEdit->setValidator(validator);

        QPushButton *button_Ok = new QPushButton("Ok", nextWindow);
        button_Ok->resize(100,40);
        button_Ok->move(300,200);
        button_Ok->setStyleSheet("border: 2px solid #000000; background-color: #99FFCC; color: #000066;");

        QPushButton *button_Close = new QPushButton("Close", nextWindow);
        button_Close->resize(100,40);
        button_Close->move(100,200);
        button_Close->setStyleSheet("border: 2px solid #000000; background-color: #FF9999; color: #000066;");

        button_Ok->setEnabled(false);

        connect(lineEdit, &QLineEdit::textChanged, this, [=]() {
            button_Ok->setEnabled(!lineEdit->text().isEmpty());
        });

        connect(button_Ok, &QPushButton::clicked, this, [=]() {
            bool ok;
            double enteredValue = lineEdit->text().toDouble(&ok);

            if (ok) {
                SerCharge = enteredValue;
                lineEdit->clear();
                nextWindow->accept();
            } else {
                QMessageBox::warning(this, "Invalid Input", "Please enter a valid number.");
            }
        });
        connect(button_Close, &QPushButton::clicked, nextWindow, &QDialog::reject);

        nextWindow->exec();
    });



    /*connect(button_B2, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(0);
    });*/



    connect(button_N1, &QPushButton::clicked, this, &MainWindow::switchToSummaryPage);









    // === PAGE 2 (Summary Page) ===
    QLabel *titleLabel = new QLabel("Total expenses", page2);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #5D4037;");

    // สร้าง QLabel และ QTextEdit ทันทีใน Constructor
    dateLabel = new QLabel("บิลวันที่: ", page2);
    expenseSummary = new QTextEdit(page2);

    // ตั้งค่า UI
    dateLabel->setAlignment(Qt::AlignCenter);
    dateLabel->setStyleSheet("font-size: 22px; font-weight: bold; color: #5D4037;");
    expenseSummary->setReadOnly(true);
    expenseSummary->setStyleSheet("border: 2px solid #5D4037; font-size: 20px; background-color: #FFF5E1; padding: 10px; color: #17140a ");
    expenseSummary->setFixedHeight(200);

    promptPayInput = new QLineEdit(page2);
    promptPayInput->setPlaceholderText("PromptPay : ใส่หมายเลขบัญชีที่นี่");
    promptPayInput->setStyleSheet("border: 2px solid #5D4037; font-size: 18px; padding: 8px;");

    QIntValidator *validatorP = new QIntValidator(0, 9999999999, this);
    promptPayInput->setValidator(validatorP);

    noteTextEdit = new QTextEdit(page2);
    noteTextEdit->setPlaceholderText("โน้ต : กรอกหมายเหตุที่นี่...");
    noteTextEdit->setStyleSheet("border: 2px solid #5D4037; font-size: 18px; padding: 8px;");
    noteTextEdit->setFixedHeight(100);

    backButton = new QPushButton("⬅ BACK", page2);
    nextButton = new QPushButton("SHARE", page2);
    backButton->setStyleSheet("background-color: black; color: white; font-size: 20px; padding: 10px;");
    nextButton->setStyleSheet("background-color: black; color: white; font-size: 20px; padding: 10px;");

    QVBoxLayout *layout2 = new QVBoxLayout(page2);
    layout2->addWidget(titleLabel);
    layout2->addWidget(dateLabel);
    layout2->addWidget(expenseSummary);
    layout2->addWidget(promptPayInput);
    layout2->addWidget(noteTextEdit);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(nextButton);
    layout2->addLayout(buttonLayout);

    page2->setLayout(layout2);

    connect(backButton, &QPushButton::clicked, this, &MainWindow::switchToMainPage);
    //connect(nextButton, &QPushButton::clicked, this, &MainWindow::resetData);


    page2->setLayout(layout2);

}

void MainWindow::calculateExpenses() {
    if (totalAmount <= 0.0) {
        qDebug() << "Error: totalAmount is zero or negative!";
        QMessageBox::warning(this, "Error", "Total amount cannot be zero or negative.");
        return;
    }

    /*if (memberNames.empty()) {
        qDebug() << "Error: memberNames is empty!";
        QMessageBox::warning(this, "Error", "No members found.");
        return;
    }*/

    if (memberPayments.empty()) {
        qDebug() << "Error: memberPayments is empty!";
        QMessageBox::warning(this, "คำเตือน", "เช็กรายการอาหารให้แน่ใจก่อนกด ok ");
        return;
    }

    if (memberNames.size() != memberPayments.size()) {
        qDebug() << "❌ Error: memberNames and memberPayments size mismatch!";
        QMessageBox::warning(this, "Error", "Mismatch in member names and payments.");
        return;
    }

    // ✅ ตรวจสอบว่า expenseSummary ถูกสร้างแล้ว
    if (!expenseSummary) {
        qDebug() << "❌ Error: expenseSummary is nullptr!";
        QMessageBox::warning(this, "Error", "Expense summary widget is missing.");
        return;
    }

    // กำหนดค่า totalFinal ให้แน่ใจว่าไม่เป็นค่าที่ไม่ได้ประกาศ
    double totalAfterDiscount = totalAmount - Discount;
    double taxAmount = totalAfterDiscount * (vatAmount / 100.0);
    double totalFinal = totalAfterDiscount + taxAmount + SerCharge;

    /*QVector<double> adjustedPayments(memberNames.size(), 0.0);
    for (int i = 0; i < memberNames.size(); ++i) {
        if (i >= memberPayments.size()) {
            qDebug() << "Error: Index out of range for memberPayments!";
            return;
        }
        adjustedPayments[i] = (memberPayments[i] / totalAmount) * totalFinal;
    }*/

     // ✅ แสดงข้อมูลใน summaryTable (ถ้ามี)
    QVector<double> adjustedPayments(memberNames.size(), 0.0);
    for (int i = 0; i < memberNames.size(); ++i) {
        adjustedPayments[i] = (memberPayments[i] / totalAmount) * totalFinal;
    }

    if (summaryTable) {
        summaryTable->setRowCount(memberNames.size());
        summaryTable->setColumnCount(2);
        summaryTable->setHorizontalHeaderLabels({"Member", "Amount"});

        for (int i = 0; i < memberNames.size(); ++i) {
            summaryTable->setItem(i, 0, new QTableWidgetItem(memberNames[i]));
            summaryTable->setItem(i, 1, new QTableWidgetItem(QString::number(adjustedPayments[i], 'f', 2) + " บาท"));
        }
    }

    /*
    QString summaryText;
    for (int i = 0; i < memberNames.size(); ++i) {
        summaryText += "Member " + QString::number(i + 1) + " : " +
                       QString::number(adjustedPayments[i], 'f', 2) + " บาท\n";
    }*/

    QString summaryText;
    for (int i = 0; i < memberNames.size(); ++i) {
        summaryText += "Member " + QString::number(i + 1) + " : " +
                       QString::number(adjustedPayments[i], 'f', 2) + " บาท\n";
    }


    expenseSummary->setText(summaryText);
    dateLabel->setText("บิลวันที่ " + date);

    qDebug() << "✅ Expense summary updated successfully!";
}






void MainWindow::resetData() {
    date.clear();
    members.clear();
    memberPayments.clear();
    totalAmount = 0.0;
    promptPayInput->clear();
    noteTextEdit->clear();
    expenseSummary->clear();
    stackedWidget->setCurrentWidget(page1);
}


void MainWindow::switchToSummaryPage() {
    qDebug() << "Switching to Summary Page...";
    qDebug() << "Total Amount: " << totalAmount;
    qDebug() << "Member Payments: " << memberPayments;

    /*if (memberPayments.empty()) {
        qDebug() << "Error: No member payment data!";
        QMessageBox::warning(this, "Error", "กรุณากรอกข้อมูลค่าอาหารก่อนกด Next.");
        return;
    }*/

    if (totalAmount <= 0.0) {
        qDebug() << "Error: totalAmount is zero!";
        QMessageBox::warning(this, "Error", "ค่าใช้จ่ายรวมต้องมากกว่า 0");
        return;
    }

    calculateExpenses();  // คำนวณค่าใช้จ่าย

    if (stackedWidget->indexOf(page2) == -1) {
        stackedWidget->addWidget(page2);
        qDebug() << "page2 was missing! Adding it to stackedWidget.";
    }

    stackedWidget->setCurrentWidget(page2);
    qDebug() << "Switched to Summary Page";
}






void MainWindow::switchToMainPage() {
    stackedWidget->setCurrentWidget(page1);  // เปลี่ยนกลับไปหน้าหลัก
}




MainWindow::~MainWindow()
{
    delete ui;
}
