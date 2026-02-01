#include "bank.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

Bank::Bank(const string &cinfo) : num_clients(0), clients(nullptr)
{
    load_clients_info(cinfo);
}

Bank::Bank(const Bank& other) : num_clients(other.num_clients)
{
    clients = new Client[num_clients];
    for (int i = 0; i < num_clients; ++i) 
    {
        clients[i] = other.clients[i];
    }
}

Bank::~Bank() 
{
    delete[] clients;
}

void Bank::load_clients_info(const string &cinfo) 
{
    ifstream file(cinfo);
    if (!file) 
    {
        throw runtime_error("File could not be opened");
    }
    file >> num_clients;

    if (num_clients <= 0)
    {
        clients = nullptr;
        return;
    }

    clients = new Client[num_clients];
    for (int i = 0; i < num_clients; ++i) 
    {
        file >> clients[i].client_name >> clients[i].ssn
             >> clients[i].bank_account_number >> clients[i].balance;
    }
}

const Client* Bank::get_clients_info() const
{
    return clients;
}

bool Bank::deposit(const string& account_number, double amount, string& message) 
{
    for (int i = 0; i < num_clients; ++i) {
        if (clients[i].bank_account_number == account_number) 
        {
            clients[i].balance += amount;
            message = "Deposit successful";
            saving_info("C:\\Users\\ashar\\OneDrive\\Documents\\GitHub\\bank-management-app-cash-flow\\src\\cinfo.txt");
            return true;
        }
    }
    message = "Account not found";
    return false;
}

bool Bank::withdraw(const string& account_number, double amount, string& message) 
{
    for (int i = 0; i < num_clients; ++i) {
        if (clients[i].bank_account_number == account_number) 
        {
            if (clients[i].balance >= amount) 
            {
                clients[i].balance -= amount;
                message = "Withdrawal successful";
                saving_info("C:\\Users\\ashar\\OneDrive\\Documents\\GitHub\\bank-management-app-cash-flow\\src\\cinfo.txt");
                return true;
            } else
            {
                message = "Insufficient funds";
                return false;
            }
        }
    }
    message = "Account not found";
    return false;
}

void Bank::saving_info(const string &cinfo) 
{
    ofstream file(cinfo);
    if (!file) 
    {
        throw runtime_error("File could not be opened");
    }
    file << num_clients << endl;
    for (int i = 0; i < num_clients; ++i) 
    {
        file << clients[i].client_name << " " << clients[i].ssn << " "
             << clients[i].bank_account_number << " " << clients[i].balance << endl;
    }
}

const Client* Bank::find_client(const string& account_number) 
{
    for (int i = 0; i < num_clients; ++i) 
    {
        if (clients[i].bank_account_number == account_number) 
        {
            return &clients[i];
        }
    }
    return nullptr;
}

void Bank::add_new_client(const Client& new_client) 
{
    Client* new_clients = new Client[num_clients + 1];
    for (int i = 0; i < num_clients; ++i) 
    {
        new_clients[i] = clients[i];
    }
    new_clients[num_clients] = new_client;
    delete[] clients;
    clients = new_clients;
    num_clients++;

    saving_info("C:\\Users\\ashar\\OneDrive\\Documents\\GitHub\\bank-management-app-cash-flow\\src\\cinfo.txt");
}

int Bank::get_num_clients() const 
{
    return num_clients;
}

extern "C" 
{
    void Bank_load_clients_info(Bank& bank, const char* cinfo) {
        bank.load_clients_info(cinfo);
    }

    int Bank_deposit(Bank& bank, const char* account_number, double amount) {
        string message;
        if (bank.deposit(account_number, amount, message)) {
            return 1; // Success
        }
        return 0; // Failure
    }

    const char* Bank_withdraw(Bank& bank, const char* account_number, double amount) {
        string message;
        if (bank.withdraw(account_number, amount, message)) {
            char* msg_copy = (char*)malloc(message.size()+ 1);
            strcpy(msg_copy, message.c_str());
            return msg_copy; // Return a copy of the message
        }
         char* msg_copy = (char*)malloc(message.size() + 1); // +1 for the null terminator
         strcpy(msg_copy, message.c_str());
         return msg_copy; // Return the allocated copy
    }

    void Bank_saving_info(Bank& bank, const char* cinfo) {
        bank.saving_info(cinfo);
    }

    const Client* Bank_find_client(Bank& bank, const char* account_number) {
        return bank.find_client(account_number);
    }

    void Bank_add_new_client(Bank& bank, const Client* new_client) {
        bank.add_new_client(*new_client);
    }

    int Bank_get_num_clients(Bank& bank) {
        return bank.get_num_clients();
    }

    void Bank_free_string(const char* str) {
        free((void*)str); // Cast to void* for free
    }


}