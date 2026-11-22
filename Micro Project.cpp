#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
class Item {
public:
    int id;
    string name;
    string category;
    double rate;
    int godownQty;  
    int shopQty;     
    int minLimit;    
    string expiry;   
};
class Order {
public:
    int orderID;
    int itemID;
    string customer;
    int qty;
    double totalAmount;
    string status;   
};
vector<Item> loadInventory() {
    vector<Item> list;
    ifstream file("products.txt");
    Item temp;
    while (file >> temp.id >> temp.name >> temp.category >> temp.rate
                >> temp.godownQty >> temp.shopQty >> temp.minLimit >> temp.expiry) {
        list.push_back(temp);
    }
    file.close();
    return list;
}
void saveInventory(vector<Item> list) {
    ofstream file("products.txt");
    for (int i = 0; i < list.size(); i++) {
        file << list[i].id << " " << list[i].name << " " << list[i].category << " "
             << list[i].rate << " " << list[i].godownQty << " " << list[i].shopQty << " "
             << list[i].minLimit << " " << list[i].expiry << endl;
    }
    file.close();
}
vector<Order> loadOrders() {
    vector<Order> list;
    ifstream file("orders.txt");
    Order temp;
    while (file >> temp.orderID >> temp.itemID >> temp.customer
                >> temp.qty >> temp.totalAmount >> temp.status) {
        list.push_back(temp);
    }
    file.close();
    return list;
}
void saveOrders(vector<Order> list) {
    ofstream file("orders.txt");
    for (int i = 0; i < list.size(); i++) {
        file << list[i].orderID << " " << list[i].itemID << " " << list[i].customer << " "
             << list[i].qty << " " << list[i].totalAmount << " " << list[i].status << endl;
    }
    file.close();
}
void receiveDelivery() {
    vector<Item> list = loadInventory();
    string name;
    int qty;
    cout << "\n--- RECEIVE DELIVERY / ADD PRODUCT ---\n";
    cout << "Enter Item Name (use_underscores_for_spaces): ";
    cin >> name;
    cout << "Enter Quantity Received (goes to Godown): ";
    cin >> qty;
    if (qty <= 0) {
        cout << "Quantity must be greater than 0!\n";
        return;
    }
    bool found = false;
    for (int i = 0; i < list.size(); i++) {
        if (list[i].name == name) {
            found = true;
            list[i].godownQty += qty;
            cout << "Updated! New Godown stock for " << name << ": " << list[i].godownQty << endl;
        }
    }
    if (!found) {
        Item newItem;
        newItem.id = (list.size() == 0) ? 101 : list[list.size() - 1].id + 1;
        newItem.name = name;
        newItem.godownQty = qty;
        newItem.shopQty = 0;
        cout << "New item! Assigned ID: " << newItem.id << "\n";
        cout << "Enter Category: ";
        cin >> newItem.category;
        cout << "Enter Rate per unit: ";
        cin >> newItem.rate;
        cout << "Enter Reorder Alert Limit: ";
        cin >> newItem.minLimit;
        cout << "Enter Expiry Date (YYYY-MM-DD): ";
        cin >> newItem.expiry;
        list.push_back(newItem);
        cout << "New item added to Godown!\n";
    }
    saveInventory(list);
}
void manageProduct() {
    vector<Item> list = loadInventory();
    int subChoice;
    cout << "\n--- MANAGE PRODUCTS ---\n";
    cout << "1. Search Product (by ID or Name)\n";
    cout << "2. Update Product Details\n";
    cout << "3. Delete Product\n";
    cout << "Enter Choice: ";
    cin >> subChoice;
    if (subChoice == 1) {
        string query;
        cout << "Enter Product Name or ID: ";
        cin >> query;
        bool found = false;
        for (int i = 0; i < list.size(); i++) {
            if (to_string(list[i].id) == query || list[i].name.find(query) != string::npos) {
                found = true;
                cout << "ID: " << list[i].id << " | Name: " << list[i].name
                     << " | Category: " << list[i].category << " | Rate: Rs." << list[i].rate
                     << " | Godown: " << list[i].godownQty << " | Shop: " << list[i].shopQty << endl;
            }
        }
        if (!found) cout << "No matching product found!\n";
    } 
    else if (subChoice == 2) {
        int id;
        cout << "Enter Product ID to Update: ";
        cin >> id;
        bool found = false;
        for (int i = 0; i < list.size(); i++) {
            if (list[i].id == id) {
                found = true;
                cout << "Enter New Name: "; cin >> list[i].name;
                cout << "Enter New Category: "; cin >> list[i].category;
                cout << "Enter New Rate: "; cin >> list[i].rate;
                cout << "Enter New Reorder Limit: "; cin >> list[i].minLimit;
                cout << "Product updated!\n";
            }
        }
        if (!found) cout << "Product ID not found!\n";
    } 
    else if (subChoice == 3) {
        int id;
        cout << "Enter Product ID to Delete: ";
        cin >> id;
        bool found = false;
        for (int i = 0; i < list.size(); i++) {
            if (list[i].id == id) {
                found = true;
                string confirm;
                cout << "Are you sure you want to delete " << list[i].name << "? (yes/no): ";
                cin >> confirm;
                if (confirm == "yes") {
                    list.erase(list.begin() + i);
                    cout << "Product deleted!\n";
                } else {
                    cout << "Deletion cancelled.\n";
                }
                break;
            }
        }
        if (!found) cout << "Product ID not found!\n";
    }
    saveInventory(list);
}
void transferStock() {
    vector<Item> list = loadInventory();
    int id, dir, qty;
    cout << "\n--- WAREHOUSE / GODOWN TRANSFER ---\n";
    cout << "Enter Item ID: ";
    cin >> id;
    cout << "Direction (1. Godown -> Shop | 2. Shop -> Godown): ";
    cin >> dir;
    cout << "Enter Quantity to move: ";
    cin >> qty;
    bool found = false;
    for (int i = 0; i < list.size(); i++) {
        if (list[i].id == id) {
            found = true;
            if (dir == 1 && qty > 0 && qty <= list[i].godownQty) {
                list[i].godownQty -= qty;
                list[i].shopQty += qty;
                cout << "Transferred! Godown: " << list[i].godownQty << " | Shop: " << list[i].shopQty << endl;
            } else if (dir == 2 && qty > 0 && qty <= list[i].shopQty) {
                list[i].shopQty -= qty;
                list[i].godownQty += qty;
                cout << "Transferred! Godown: " << list[i].godownQty << " | Shop: " << list[i].shopQty << endl;
            } else {
                cout << "Invalid direction or insufficient stock!\n";
            }
        }
    }
    if (!found) cout << "Item ID not found!\n";
    saveInventory(list);
}
void recordWastage() {
    vector<Item> list = loadInventory();
    string today;
    cout << "\n--- EXPIRY SCAN & WASTAGE ---\n";
    cout << "Enter Today's Date (YYYY-MM-DD): ";
    cin >> today;
    for (int i = 0; i < list.size(); i++) {
        if (list[i].expiry < today) {
            cout << "[EXPIRED] ID: " << list[i].id << " | " << list[i].name
                 << " (Expired on: " << list[i].expiry << ")\n";
        }
    }
    int id, qty;
    string location;
    cout << "\nEnter Item ID to deduct wastage (or 0 to go back): ";
    cin >> id;
    if (id == 0) return;
    cout << "Where did wastage occur? (Godown / Shop): ";
    cin >> location;
    cout << "Enter Expired/Damaged Quantity: ";
    cin >> qty;
    bool found = false;
    for (int i = 0; i < list.size(); i++) {
        if (list[i].id == id) {
            found = true;
            if (location == "Godown" && qty > 0 && qty <= list[i].godownQty) {
                list[i].godownQty -= qty;
                cout << "Removed " << qty << " wasted units from Godown.\n";
            } else if (location == "Shop" && qty > 0 && qty <= list[i].shopQty) {
                list[i].shopQty -= qty;
                cout << "Removed " << qty << " wasted units from Shop.\n";
            } else {
                cout << "Invalid location or quantity exceeds available stock!\n";
            }
        }
    }
    if (!found) cout << "Item ID not found!\n";
    saveInventory(list);
}
void viewReport() {
    vector<Item> list = loadInventory();
    double grandTotalValue = 0;

    cout << "\n======================== INVENTORY REPORT ========================\n";
    cout << "ID\tName\tRate\tGodown\tShop\tTotal\tValue\tStatus\n";
    cout << "------------------------------------------------------------------\n";

    for (int i = 0; i < list.size(); i++) {
        int totalQty = list[i].godownQty + list[i].shopQty;
        double itemValue = totalQty * list[i].rate;
        grandTotalValue += itemValue;
        string status = (totalQty <= list[i].minLimit) ? "REORDER ALERT!" : "OK";
        cout << list[i].id << "\t" << list[i].name << "\t" << list[i].rate << "\t"
             << list[i].godownQty << "\t" << list[i].shopQty << "\t"
             << totalQty << "\t" << itemValue << "\t" << status << endl;
    }

    cout << "------------------------------------------------------------------\n";
    cout << "TOTAL INVENTORY VALUATION: Rs. " << grandTotalValue << endl;
    cout << "==================================================================\n";
}
void createOrder() {
    vector<Item> inv = loadInventory();
    vector<Order> orders = loadOrders();
    int id, qty;
    string customer;
    cout << "\n--- CREATE NEW ORDER ---\n";
    cout << "Enter Product ID: ";
    cin >> id;
    cout << "Enter Quantity to Order: ";
    cin >> qty;
    bool found = false;
    for (int i = 0; i < inv.size(); i++) {
        if (inv[i].id == id) {
            found = true;
            int totalAvailable = inv[i].godownQty + inv[i].shopQty;
            if (qty <= 0 || qty > totalAvailable) {
                cout << "Invalid quantity or insufficient stock! Available: " << totalAvailable << endl;
                return;
            }
            cout << "Enter Customer Name: ";
            cin >> customer;
            if (inv[i].shopQty >= qty) {
                inv[i].shopQty -= qty;
            } else {
                int remaining = qty - inv[i].shopQty;
                inv[i].shopQty = 0;
                inv[i].godownQty -= remaining;
            }
            Order newOrder;
            newOrder.orderID = (orders.size() == 0) ? 1001 : orders[orders.size() - 1].orderID + 1;
            newOrder.itemID = id;
            newOrder.customer = customer;
            newOrder.qty = qty;
            newOrder.totalAmount = qty * inv[i].rate;
            newOrder.status = "Pending";
            orders.push_back(newOrder);
            cout << "Order #" << newOrder.orderID << " placed! Total: Rs." << newOrder.totalAmount << endl;
            if ((inv[i].godownQty + inv[i].shopQty) <= inv[i].minLimit) {
                cout << "[ALERT] Stock for " << inv[i].name << " is now at or below reorder limit!\n";
            }
        }
    }
    if (!found) cout << "Product ID not found!\n";
    saveInventory(inv);
    saveOrders(orders);
}
void trackOrders() {
    vector<Order> orders = loadOrders();
    vector<Item> inv = loadInventory();
    cout << "\n===================== ALL ORDERS =====================\n";
    cout << "OrdID\tProdID\tCustomer\tQty\tTotal\tStatus\n";
    cout << "------------------------------------------------------\n";
    for (int i = 0; i < orders.size(); i++) {
        cout << orders[i].orderID << "\t" << orders[i].itemID << "\t"
             << orders[i].customer << "\t\t" << orders[i].qty << "\t"
             << orders[i].totalAmount << "\t" << orders[i].status << endl;
    }
    cout << "======================================================\n";
    int ordID;
    cout << "Enter Order ID to update status (or 0 to go back): ";
    cin >> ordID;
    if (ordID == 0) return;
    bool found = false;
    for (int i = 0; i < orders.size(); i++) {
        if (orders[i].orderID == ordID) {
            found = true;
            if (orders[i].status == "Cancelled" || orders[i].status == "Delivered") {
                cout << "Order is already " << orders[i].status << " and cannot be changed.\n";
                return;
            }
            cout << "New Status (Processing / Shipped / Delivered / Cancelled): ";
            cin >> orders[i].status;
            if (orders[i].status == "Cancelled") {
                for (int j = 0; j < inv.size(); j++) {
                    if (inv[j].id == orders[i].itemID) {
                        inv[j].godownQty += orders[i].qty;
                    }
                }
                saveInventory(inv);
                cout << "Order cancelled and stock restored to Godown.\n";
            } else {
                cout << "Order status updated!\n";
            }
        }
    }
    if (!found) cout << "Order ID not found!\n";
    saveOrders(orders);
}
int main() {
    int choice;
    while (true) {
        cout << "\n=== INVENTORY & ORDER MANAGEMENT SYSTEM ===\n";
        cout << "1. Receive Delivery / Add New Product\n";
        cout << "2. Search, Update or Delete Product\n";
        cout << "3. Transfer Stock (Godown <-> Shop)\n";
        cout << "4. Expiry Scan & Record Wastage\n";
        cout << "5. View Stock Levels, Valuation & Reorder Alerts\n";
        cout << "6. Create Customer Order\n";
        cout << "7. View Orders & Update Order Status\n";
        cout << "8. Exit\n";
        cout << "Enter Choice: ";
        if (!(cin >> choice) || choice == 8) {
            cout << "Exiting System...\n";
            break;
        }
        if (choice == 1) receiveDelivery();
        else if (choice == 2) manageProduct();
        else if (choice == 3) transferStock();
        else if (choice == 4) recordWastage();
        else if (choice == 5) viewReport();
        else if (choice == 6) createOrder();
        else if (choice == 7) trackOrders();
        else cout << "Invalid choice!\n";
    }
    return 0;
}