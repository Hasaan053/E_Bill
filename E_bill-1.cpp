#include <iostream>
#include <fstream>
#include <string>
#include <limits>

using namespace std;

class Customer {
public:
    string name;
    int id;
    float price;

    void readCustomer() {
        cout << "\nEnter customer details: \nName: ";
        cin >> name;
        cout << "Price per unit: ";
        cin >> price;
    }
};

class Bill {
public:
    int bill_id;
    int customer_id;
    string name;
    float price;
    int units;

    Bill() : bill_id(0), customer_id(0), price(0), units(0) {}

    int generateBillID() {
        ifstream file("bi.txt");
        int lastID = 0;
        
        if (file.is_open()) {
            file >> lastID;
        }
        file.close();

        lastID++; // Increment Bill ID
        ofstream outFile("bi.txt");
        outFile << lastID;
        outFile.close();

        return lastID;
    }

    void readBill(int customerID, const Customer &c) {
        bill_id = generateBillID();
        name = c.name;
        price = c.price;
        customer_id = customerID;

        cout << "\nEnter number of units consumed: ";
        cin >> units;

        cout << "\n✅ Bill generated successfully!";
        cout << "\n🆔 Your bill ID is: " << bill_id << endl;
    }
};

int generateCustomerID() {
    ifstream file("cus.txt");
    int lastID = 0;

    if (file.is_open()) {
        file >> lastID;
    }
    file.close();

    lastID++; // Increment Customer ID
    ofstream outFile("cus.txt");
    outFile << lastID;
    outFile.close();

    return lastID;
}

void addCustomer() {
    ofstream outFile("customers.txt", ios::app);
    Customer c;
    c.readCustomer();
    c.id = generateCustomerID();

    cout << "\n✅ Customer added successfully!";
    cout << "\n🆔 Your Customer ID is: " << c.id << endl;

    outFile << c.id << " " << c.name << " " << c.price << endl;
    outFile.close();
}

void generateBill() {
    int customerID;
    ifstream inFile("customers.txt");
    ofstream outFile("bills.txt", ios::app);
    Customer c;
    Bill b;
    bool found = false;

    cout << "\nEnter your Customer ID: ";
    cin >> customerID;

    while (inFile >> c.id >> c.name >> c.price) {
        if (c.id == customerID) {
            found = true;
            b.readBill(c.id, c);
            outFile << b.bill_id << " " << b.customer_id << " " << b.name << " " << b.price << " " << b.units << endl;
            break;
        }
    }
    inFile.close();
    outFile.close();

    if (!found) {
        cout << "\n❌ Error! No customer found with ID: " << customerID << endl;
    }
}

void displayBill() {
    int customerID;
    ifstream inFile("bills.txt");
    Bill b;
    bool found = false;
    float totalAmount = 0;

    cout << "\nEnter your Customer ID: ";
    cin >> customerID;

    while (inFile >> b.bill_id >> b.customer_id >> b.name >> b.price >> b.units) {
        if (b.customer_id == customerID) {
            found = true;
            totalAmount += b.price * b.units;
        }
    }
    inFile.close();

    if (found) {
        cout << "\n💰 Total amount for Customer ID " << customerID << " is: " << totalAmount << endl;
    } else {
        cout << "\n❌ No bills found for Customer ID: " << customerID << endl;
    }
}

void deleteBill() {
    int billID;
    ifstream inFile("bills.txt");
    ofstream outFile("temp.txt");
    Bill b;
    bool found = false;

    cout << "\nEnter the Bill ID to delete: ";
    cin >> billID;

    while (inFile >> b.bill_id >> b.customer_id >> b.name >> b.price >> b.units) {
        if (b.bill_id == billID) {
            cout << "\n✅ Bill with ID " << billID << " deleted successfully!";
            found = true;
        } else {
            outFile << b.bill_id << " " << b.customer_id << " " << b.name << " " << b.price << " " << b.units << endl;
        }
    }

    inFile.close();
    outFile.close();

    if (found) {
        remove("bills.txt");
        rename("temp.txt", "bills.txt");
    } else {
        cout << "\n❌ Error! No bill found with ID: " << billID << endl;
        remove("temp.txt"); // Remove temp file if no deletion occurs
    }
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void login() {
    string username, password;
    int attempts = 3;

    while (attempts > 0) {
        cout << "\nEnter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (username == "Admin" && password == "12345") {
            cout << "\n✅ Login Successful! Welcome, Admin.\n";
            return;
        } else {
            attempts--;
            cout << "\n❌ Invalid credentials! " << attempts << " attempts left.\n";
        }
    }
    cout << "\n🔒 Too many failed attempts! Exiting...\n";
    exit(0);
}

int main() {
    cout << "\n===================================";
    cout << "\n⚡ ELECTRICITY BILL CALCULATOR ⚡";
    cout << "\n===================================\n";

    login();  // Secure login system

    int choice;
    while (true) {
        cout << "\n📌 MAIN MENU";
        cout << "\n1️⃣ Add New Customer";
        cout << "\n2️⃣ Generate Bill";
        cout << "\n3️⃣ Display Total Bill";
        cout << "\n4️⃣ Delete Bill";
        cout << "\n5️⃣ Exit";
        cout << "\n\n👉 Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            clearInputBuffer();
            cout << "\n❌ Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                generateBill();
                break;
            case 3:
                displayBill();
                break;
            case 4:
                deleteBill();
                break;
            case 5:
                cout << "\n🚀 Program Terminated Successfully. Goodbye!\n";
                return 0;
            default:
                cout << "\n❌ Invalid choice! Please select from the menu.\n";
        }
    }
}
