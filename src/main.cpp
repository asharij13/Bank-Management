#include "bank.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    try {
        // Create the bank and load client information from a file
        Bank bank("cinfo.txt");

        // Display the loaded client information
        const Client* clients = bank.get_clients_info();
        cout << "Clients loaded: " << endl;
        for (int i = 0; i < bank.get_num_clients(); ++i) {
            cout << "Name: " << clients[i].client_name 
                 << ", SSN: " << clients[i].ssn 
                 << ", Account Number: " << clients[i].bank_account_number 
                 << ", Balance: " << clients[i].balance << endl;
        }

        // Test deposit
        string message;
        if (bank.deposit("123456789", 100.0, message)) {
            cout << message << endl;
        } else {
            cout << message << endl;
        }

        // Test withdraw
        if (bank.withdraw("123456789", 50.0, message)) {
            cout << message << endl;
        } else {
            cout << message << endl;
        }

        // Find a client
        const Client* client = bank.find_client("123456789");
        if (client) {
            cout << "Client found: " << client->client_name << endl;
        } else {
            cout << "Client not found." << endl;
        }

        // Save updated client information back to the same file
        bank.saving_info("cinfo.txt");

    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
    } catch (...) {
        cout << "An unexpected error occurred." << endl;
    }

    return 0;
}
