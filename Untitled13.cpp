#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;

class Product {
public:
    string productId, name, description, category;
    float price;
    int stock;

    Product(string id, string n, string desc, float p, int s, string cat)
        : productId(id), name(n), description(desc), price(p), stock(s), category(cat) {}
    
    void updateStock(int quantity) { stock -= quantity; }
    void applyDiscount(float percent) { price -= price * (percent / 100); }
};

class Order {
public:
    static int orderCounter;
    int orderId;
    float totalAmount;
    vector<pair<Product*, int>> orderDetails;

    Order(float amount, vector<pair<Product*, int>> details)
        : orderId(++orderCounter), totalAmount(amount), orderDetails(details) {}
    
    void displayOrder() const {
        cout << "\nOrder ID: " << orderId << "\nTotal Amount: " << totalAmount << "\nOrder Details:\n";
        cout << left << setw(12) << "Product ID" << setw(15) << "Name" << setw(10) << "Price" << "Quantity\n";
        for (const auto &item : orderDetails) {
            cout << left << setw(12) << item.first->productId << setw(15) << item.first->name
                 << setw(10) << item.first->price << item.second << endl;
        }
    }
};
int Order::orderCounter = 0;

class ShoppingCart {
private:
    vector<pair<Product*, int>> items;
public:
    void addProduct(Product& product, int quantity) {
        if (quantity <= 0 || quantity > product.stock) {
            cout << "Invalid quantity! Check stock availability.\n";
            return;
        }
        product.updateStock(quantity);
        items.push_back({&product, quantity});
        cout << "Product added successfully!\n";
    }

    void removeProduct(const string& productId) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->first->productId == productId) {
                cout << "Product removed from cart.\n";
                items.erase(it);
                return;
            }
        }
        cout << "Product not found in cart!\n";
    }

    void viewCart(vector<Order>& orders) {
        if (items.empty()) {
            cout << "Shopping cart is empty!\n";
            return;
        }
        cout << "\nShopping Cart:\n";
        cout << left << setw(12) << "Product ID" << setw(15) << "Name" << setw(10) << "Price" << "Quantity\n";
        for (const auto &item : items) {
            cout << left << setw(12) << item.first->productId << setw(15) << item.first->name
                 << setw(10) << item.first->price << item.second << endl;
        }

        string checkoutChoice;
        do {
            cout << "Do you want to check out all the products? (Y/N): ";
            cin >> checkoutChoice;
        } while (checkoutChoice != "Y" && checkoutChoice != "y" && checkoutChoice != "N" && checkoutChoice != "n");
        
        if (checkoutChoice == "Y" || checkoutChoice == "y") {
            float total = getTotalAmount();
            orders.emplace_back(total, checkout());
            cout << "You have successfully checked out the products!\n";
        }
    }
    
    float getTotalAmount() const {
        float total = 0;
        for (const auto &item : items) total += item.first->price * item.second;
        return total;
    }
    
    vector<pair<Product*, int>> checkout() {
        vector<pair<Product*, int>> orderItems = items;
        items.clear();
        return orderItems;
    }
};

class Customer {
public:
    static int customerCounter;
    string customerId, name, email, phone, address;
    ShoppingCart cart;
    
    Customer(string n, string e, string p, string a)
        : customerId("CUST" + to_string(++customerCounter)), name(n), email(e), phone(p), address(a) {}
};
int Customer::customerCounter = 0;

int main() {
    vector<Product> products = {
        {"ABC", "Pen", "Blue ink pen", 10, 50, "Stationery"},
        {"CDE", "Paper", "A4 size paper", 20, 100, "Office Supplies"},
        {"QWE", "Notebook", "100 pages notebook", 30, 70, "Stationery"}
    };
    vector<Order> orders;
    
    string name, email, phone, address;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter Email: ";
    cin >> email;
    cout << "Enter Phone: ";
    cin >> phone;
    cout << "Enter Address: ";
    cin.ignore();
    getline(cin, address);
    Customer customer(name, email, phone, address);
    
    int choice;
    while (true) {
        cout << "\nMenu:\n1. View Products\n2. View Shopping Cart\n3. View Orders\n4. Exit\nEnter choice: ";
        cin >> choice;
        
        if (choice == 1) {
            string addMore;
            do {
                cout << "\nAvailable Products:\n";
                cout << left << setw(12) << "Product ID" << setw(15) << "Name" << "Price\n";
                for (const auto &product : products) {
                    cout << left << setw(12) << product.productId << setw(15) << product.name << product.price << endl;
                }
                
                string prodId;
                cout << "Enter product ID: ";
                cin >> prodId;
                int quantity;
                cout << "Enter quantity: ";
                cin >> quantity;
                
                bool found = false;
                for (auto &p : products) {
                    if (p.productId == prodId) {
                        customer.cart.addProduct(p, quantity);
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Invalid product ID!\n";
                
                cout << "Do you want to add another product? (Y/N): ";
                cin >> addMore;
            } while (addMore == "Y" || addMore == "y");
        }
        else if (choice == 2) {
            customer.cart.viewCart(orders);
        }
        else if (choice == 3) {
            if (orders.empty()) cout << "No orders yet!\n";
            else for (const auto &order : orders) order.displayOrder();
        }
        else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        }
    }
    return 0;
}