#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct  Client {
    string client_name;
    string ssn;
    string bank_account_number;
    double balance;

    Client& operator=(const Client& other)
    {
        if (this != &other)
        {
            client_name = other.client_name;
            ssn = other.ssn;
            bank_account_number = other.bank_account_number;
            balance = other.balance;
        }
        return *this;
    }
};


class  Bank {
private:
    int num_clients;
    Client* clients;

public:
    Bank(const string& cinfo); 
    Bank(const Bank& other);
    ~Bank(); 

    void load_clients_info(const string& cinfo);
    const Client* get_clients_info() const;
    bool deposit(const string& account_number, double amount, string& message);
    bool withdraw(const string& account_number, double amount, string& message);
    void saving_info(const string& cinfo);
    const Client* find_client(const string& account_number);
    void add_new_client(const Client& new_client);
    int get_num_clients() const;

};

extern "C" 
{
     void Bank_load_clients_info(Bank& bank, const char* cinfo);
     int Bank_deposit(Bank& bank, const char* account_number, double amount);
     const char* Bank_withdraw(Bank& bank, const char* account_number, double amount);
     void Bank_saving_info(Bank& bank, const char* cinfo);
     bool Bank_find_client(
    Bank& bank,
    const char* account_number,
    char* client_name,
    int client_name_size,
    char* ssn,
    int ssn_size,
    char* bank_account_number,
    int bank_account_number_size,
    double* balance
    );
     void Bank_add_new_client(Bank& bank, const Client* new_client);
     int Bank_get_num_clients(Bank& bank);
}


#endif