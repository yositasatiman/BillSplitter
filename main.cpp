#include <iostream> // ใช้รับค่าและแสดงผล
#include <map> // ใช้เก็บคู่ของ key-value e.g. เมนูอาหารและราคา
#include <vector> // ใช้เก็บรายการข้อมูล e.g. รายการเมนู
#include <iomanip> // ใช้แสดงผลทศนิยม 2 ตำแหน่ง
#include <ctime> // จัดการวันที่และเวลา
#include <chrono> // จัดการวันที่แลพเวลา
#include <sstream> // แปลงค่าข้อมูลเป็น string
#include <fstream> // อ่านและเขียนไฟล์
#include <cstdlib> // ฟังก์ชันทั่วไป e.g. การจัดการหน่วยความจำ
#include "json.hpp" // ไลบรารี JSON ของ nlohmann ใช้จัดการข้อมูล JSON

using namespace std; // ไม่ต้องเขียน std:: ทุกครั้ง
using namespace chrono; // ใช้งาน chrono สะดวกขึ้น
using json = nlohmann::json; // กำหนดชื่อน่อ json ให้ใช้งานง่ายขึ้น

struct splitHistory { // กำหนดโครงสร้าง splitHistory
    string date; // วันที่ทำรายการ
    int taxInvoiceNo; // เลขที่ใบเสร็จ
    string user; // ชื่อเจ้าของใบเสร็จ
    int numberPeople; // จำนวนคนที่แชร์ค่าอาหาร
    vector<string> menu; // รายการอาหาร
    map<string, double> menuPrice; // แผนที่เก็บเมนูและราคา
    map<string, double> personPayment; // แผนที่เก็บจำนวนเงินที่แต่ละคนต้องจ่าย
    double totalBath; // ราคารวมค่าอาหารทั้งหมด
    double discount; // ส่วนลด
    int numberPromptPay; // เบอร์โทรศัพท์ Promptpay
    double taxRate; // อัตราภาษีมูลค่าเพิ่ม
    double VATable; // ราคาที่รวมภาษีแล้ว

    json to_json() const { // ฟังก์ชันแปลง splitHistory เป็น JSON
        return { // คืนค่า 
            {"date", date},
            {"taxInvoiceNo", taxInvoiceNo},
            {"user", user},
            {"numberPeople", numberPeople},
            {"menu", menu},
            {"menuPrice", menuPrice},
            {"personPayment", personPayment},
            {"totalBath", totalBath},
            {"discount", discount},
            {"numberPromptPay", numberPromptPay},
            {"taxRate", taxRate},
            {"VATable", VATable}
        };
    }


    static splitHistory from_json(const json& j) { // ฟังก์ชันแปลง JSON กลับเป็น splitHistory
        splitHistory data; // ประกาศตัวแปร
        //---> ใช้ j.at("key").get_to(data.date); เพื่อดึงค่าจาก JSON ไปเก็บใน data
        j.at("date").get_to(data.date); // ดึงวันที่เก็บไว้ใน data.date
        j.at("taxInvoiceNo").get_to(data.taxInvoiceNo); // ดึงเลขใบเสร็จดเก็บไว้ใน data.taxInvoiceNo
        j.at("user").get_to(data.user); // ดึงค่าชื่อผู้ใช้เก็บไว้ใน data.user
        j.at("numberPeople").get_to(data.numberPeople); // ดึงค่าจำนวนคนเก็บไว้ใน data.numberPeople
        j.at("menu").get_to(data.menu); // ดึงค่ารายการอาหารเก็บไว้ใน data.menu
        //---> ใช้ .value("key", default_value) ป้องกันค่าข้อผิดพลาดถ้าคีย์ไม่อยู่ใน JSON
        //---> ถ้าคีย์ "menuPrice" หรือ "personPayment" ไม่มีใน JSON จะไม่เกิดข้อผิดพลาด แต่จะคืนค่าเป็น {} <-ว่างเปล่า
        data.menuPrice = j.value("menuPrice", map<string, double>{}); 
        data.personPayment = j.value("personPayment", map<string, double>{});
        j.at("totalBath").get_to(data.totalBath); // ดึงค่าราคาทั้งหมดเก็บไว้ใน data.totalBath
        j.at("discount").get_to(data.discount); // ดึงค่าส่วนลดเก็บไว้ใน data.discount
        j.at("numberPromptPay").get_to(data.numberPromptPay); // ดึงค่าเบอร์โทรศัพท์ promptpay เก็บไว้ใน data.numberPromptpay
        j.at("taxRate").get_to(data.taxRate); // ดึงค่าอัตราภาษีมูลค่าเพิ่มเก็บไว้ใน data.taxRate
        j.at("VATable").get_to(data.VATable); // ดึงค่าราคารวมภาษีแล้วเก็บไว้ใน data.VATable
        return data; // หลังจากดึงค่าจาก JSON มาเก็บใน splitHistory เสร็จแล้ว ฟังก์ชันจะคืนค่า data ออกไป
    }
};

string getCurrent() { // ฟังก์ชันดึงเวลาปัจจุบัน จะคืนค่าเป็น string
    auto now = system_clock::now(); // ดึงเวลาปัจจุบันของระบบ chrono::system_clock::now(); คืนค่าแบบ time_point
    time_t currentTime = system_clock::to_time_t(now); // แปลงค่า time_point เป็น time_t -> เป็นค่าที่ใช้เก็บเวลาในหน่วยวินาทีตั้งแต่ปี 1970
    stringstream ss; // ใช้ stingstream สร้าง string ของวันที่และเวลา
    ss << put_time(localtime(&currentTime), "%d/%m/%Y %H:%M"); // format วัน/เดือน/ปี ชั่วโมง/นาที เก็บไว้ใน ss
    return ss.str(); คืนค่าเป็น string
}

vector<splitHistory> loadFromFile() { // ฟังก์ชันโหลดข้อมูลจากไฟล์ JSON
    ifstream file("data.json"); // เปิดไฟล์ data.json เพื่ออ่านข้อมูล
    if (!file) { // เงื่อนไขตรวจสอบว่าเปิดสำเร็จหรือไม่ ถ้าไม่สำเร็จจะคืนค่าว่าง {}
        return {};
    }
    json jsonData; 
    file >> jsonData; // โหลด JSON ทั้งหมดจากไฟล์เข้าสู่ตัวแปร jsonData
    file.close(); // ปิดไฟล์

    vector<splitHistory> historyList; // ใช้เก็บข้อมูลที่โหลดจาก JSON
    for (const auto& item : jsonData) { // ลูปอ่านแต่ละ item
        historyList.push_back(splitHistory::from_json(item)); // เพิ่มข้อมูล vector -> historyList
    }
    return historyList; // คืนค่าส่วนนี้ไปให้ main
}

void saveToFile(const vector<splitHistory>& historyList) { // ฟังก์ชันบันทึกข้อมูลลงไฟล์ เป็น const เพื่อไม่ให้ถูกเปลี่ยนแปลง
    json jsonData = json::array(); // สร้างตัวแปร jsonData เป็น array
    for (const auto& data : historyList) {
        jsonData.push_back(data.to_json()); // ลูปแปลง splitHistory เป็น JSON แล้วเพิ่มลง jsonData
    }

    ofstream file("data.json"); // เปิดไฟล์เพื่อเขียนข้อมููล
    if (!file) { // เงื่อนไขถ้าเปิดไฟล์ไมาได้
        cout << "Error: Could not open file for writing.\n";
        return;
    }
    file << jsonData.dump(4); // แปลง JSON ให้เป็นสตริง พร้อมฟอร์แมตให้อ่านง่าย (เว้นวรรค 4 ช่อง)
    file.close(); // ปิดไฟล์
    cout << "Data saved successfully!\n";
}

int getNextTaxInvoiceNo(const vector<splitHistory>& historyList) { // ฟังก์ชันหาหมายเลขใบเสร็จล่าสุด
    int maxInvoiceNo = 0;
    for (const auto& record : historyList) {
        if (record.taxInvoiceNo > maxInvoiceNo) {
            maxInvoiceNo = record.taxInvoiceNo;
        }
    }
    return maxInvoiceNo + 1;
}

splitHistory getNewRecord(const vector<splitHistory>& historyList) { // ฟังก์ชันรับข้อมูลใหม่
    splitHistory data;
    data.date = getCurrent();
    data.taxInvoiceNo = getNextTaxInvoiceNo(historyList);
    data.taxRate = 0.0;

    cout << "Date: " << data.date << endl;
    cout << "Please enter your name: ";
    cin >> ws;
    getline(cin, data.user);

    while(true) {
        cout << "Please add PromptPay: ";
        cin >> data.numberPromptPay;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a valid number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    int totalItems;
    while (true) {
        cout << "Please enter the total number of food items: ";
        cin >> totalItems;
        if (cin.fail() || totalItems <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a valid positive number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }

    char taxChoice;
    while (true) {
        cout << "Do you have tax? (Y/N): ";
        cin >> taxChoice;
        taxChoice = toupper(taxChoice);

        if (taxChoice == 'Y' || taxChoice == 'N') {
            break;
        } else {
            cout << "Invalid input! Please enter 'Y' or 'N'.\n";
        }
    }

    if (taxChoice == 'Y') { 
        while (true) {
            cout << "What is your tax %? ";
            cin >> data.taxRate;
            if (cin.fail() || data.taxRate < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a valid positive number.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
    }

    data.totalBath = 0.0;

    for (int i = 1; i <= totalItems; i++) {
        string menuName;
        double price;
        cout << "Please enter the name of food item " << i << ": ";
        cin >> ws;
        getline(cin, menuName);
        data.menu.push_back(menuName);

        while (true) {
            cout << "Please enter the price of " << menuName << ": ";
            cin >> price;
            if (cin.fail() || price <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a valid positive number.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }
        data.menuPrice[menuName] = price;

        int numPeople;
        while (true) {
            cout << "How many people shared " << menuName << "?: ";
            cin >> numPeople;
            if (cin.fail() || numPeople <= 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input! Please enter a positive number.\n";
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
        }

        vector<string> people(numPeople);
        double splitPrice = price / numPeople;

        data.totalBath += price;

        cout << "Enter the names of " << numPeople << " people sharing " << menuName << ":\n";
        for (int j = 0; j < numPeople; j++) {
            cin >> ws;
            getline(cin, people[j]);
            data.personPayment[people[j]] += splitPrice;
        }
    }
    double discount;
    while (true) {
        cout << "Please enter the discount amount: ";
        cin >> discount;
        if (cin.fail() || discount < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a valid positive number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    data.discount = discount;
    
    int countParticipants = data.personPayment.size();

    double totalAfterDiscount = max(0.0, data.totalBath - data.discount);
    double taxAmount = (data.taxRate / 100.0) * totalAfterDiscount;
    data.VATable = totalAfterDiscount + taxAmount;
    data.totalBath = totalAfterDiscount;
    
// หลังจากคำนวณ totalAfterDiscount, taxAmount, VATable, และ data.totalBath แล้ว
    if (totalAfterDiscount > 0) {
        map<string, double> updatedPersonPayment;
        double sumPerson = 0.0;
        // คำนวณยอดที่แต่ละคนต้องจ่ายแบบไม่ปัดเศษ
        for (auto& entry : data.personPayment) {
            double taxShare = (entry.second / totalAfterDiscount) * taxAmount;
            double newPayment = entry.second + taxShare;
            updatedPersonPayment[entry.first] = newPayment;
            sumPerson += newPayment;
        }
        // คำนวณความต่างที่เกิดจากความผิดพลาดของการปัดเศษ
        double diff = data.VATable - sumPerson;
        // ปรับยอดของคนสุดท้ายให้ยอดรวมเท่ากับ VATable
        if (!updatedPersonPayment.empty()) {
            auto lastIt = updatedPersonPayment.end();
            lastIt--;
            updatedPersonPayment[lastIt->first] += diff;
        }
        data.personPayment = updatedPersonPayment;
    }
data.numberPeople = (countParticipants > 0 ? countParticipants : 1);
return data;
}

void displayBill(const splitHistory& data) { // ฟังก์ชันแสดงบิล
    cout << "\n========== BILL ==========\n";
    cout << "Date: " << data.date << "\n";
    cout << "Tax Invoice No: " << data.taxInvoiceNo << "\n";
    cout << "User: " << data.user << "\n";
    cout << "PromptPay: " << data.numberPromptPay << "\n";
    cout << "Number of People: " << data.numberPeople << "\n";
    cout << "------ Menus ------\n";
    for (const auto& item : data.menuPrice) {
        cout << item.first << " : " << fixed << setprecision(2) << item.second << " THB\n";
    }
    cout << "--------------------\n";
    cout << "Discount: " << data.discount << " THB\n";
    cout << "Total (before tax): " << data.totalBath << " THB\n";
    cout << "Tax (" << fixed << setprecision(2) << data.taxRate << "%): " 
    << fixed << setprecision(2) << (data.taxRate / 100) * data.totalBath << " THB\n";
    cout << "Total (after tax): " << data.VATable << " THB\n";

    cout << "------ Payment Details ------\n";
    for (const auto& entry : data.personPayment) {
        cout << entry.first << " owes: " << fixed << setprecision(2) << entry.second << " THB\n";
    }
    cout << "==========================\n";
}

int main() {
    vector<splitHistory> historyList = loadFromFile();

    splitHistory newRecord = getNewRecord(historyList);
    historyList.push_back(newRecord);
    displayBill(newRecord);
    saveToFile(historyList);

    while (true) {
        string userInput;
        cout << "\nType 'Exit' to quit or press Enter to add a new record: ";
        cin >> ws;
        getline(cin, userInput);

        if (userInput == "Exit" || userInput == "exit") {
            cout << "Exiting program...\n";
            break;
        }

        newRecord = getNewRecord(historyList);
        historyList.push_back(newRecord);
        displayBill(newRecord);
        saveToFile(historyList);
    }

    return 0;
}
