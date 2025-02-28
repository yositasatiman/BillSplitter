#include <iostream>
#include <map>
#include <vector>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include "json.hpp"

using namespace std;
using namespace chrono;
using json = nlohmann::json;

struct splitHistory {
    string date;
    int taxInvoiceNo;
    string user;
    int numberPeople;
    vector<string> menu;
    map<string, double> menuPrice;
    map<string, double> personPayment;
    double totalBath;
    double discount;
    int numberPromptPay;
    double taxRate;
    double VATable;

    json to_json() const {
        return {
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


    static splitHistory from_json(const json& j) {
        splitHistory data;
        j.at("date").get_to(data.date);
        j.at("taxInvoiceNo").get_to(data.taxInvoiceNo);
        j.at("user").get_to(data.user);
        j.at("numberPeople").get_to(data.numberPeople);
        j.at("menu").get_to(data.menu);
        data.menuPrice = j.value("menuPrice", map<string, double>{});
        data.personPayment = j.value("personPayment", map<string, double>{});
        j.at("totalBath").get_to(data.totalBath);
        j.at("discount").get_to(data.discount);
        j.at("numberPromptPay").get_to(data.numberPromptPay);
        j.at("taxRate").get_to(data.taxRate);
        j.at("VATable").get_to(data.VATable);
        return data;
    }
};

string getCurrent() {
    auto now = system_clock::now();
    time_t currentTime = system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&currentTime), "%d/%m/%Y %H:%M");
    return ss.str();
}

vector<splitHistory> loadFromFile() {
    ifstream file("data.json");
    if (!file) {
        return {};
    }
    json jsonData;
    file >> jsonData;
    file.close();

    vector<splitHistory> historyList;
    for (const auto& item : jsonData) {
        historyList.push_back(splitHistory::from_json(item));
    }
    return historyList;
}

void saveToFile(const vector<splitHistory>& historyList) {
    json jsonData = json::array();
    for (const auto& data : historyList) {
        jsonData.push_back(data.to_json());
    }

    ofstream file("data.json");
    if (!file) {
        cout << "Error: Could not open file for writing.\n";
        return;
    }
    file << jsonData.dump(4);
    file.close();
    cout << "Data saved successfully!\n";
}

int getNextTaxInvoiceNo(const vector<splitHistory>& historyList) {
    int maxInvoiceNo = 0;
    for (const auto& record : historyList) {
        if (record.taxInvoiceNo > maxInvoiceNo) {
            maxInvoiceNo = record.taxInvoiceNo;
        }
    }
    return maxInvoiceNo + 1;
}

splitHistory getNewRecord(const vector<splitHistory>& historyList) {
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

void displayBill(const splitHistory& data) {
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