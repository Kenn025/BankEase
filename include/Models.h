#pragma once

#include <string>

using namespace std;

// Structure for Client
struct Client
{
    string accountNumber;
    string firstName;  // NEW
    string lastName;   // NEW
    string middleName; // NEW
    string suffix;
    string phone;
    string email;
    double balance;
    string pinCode;
    bool locked = false;
    int failedAttempts = 0;
};

// Structure for Transaction
struct Transaction
{
    string accountNumber;
    string type;
    double amount;
    string date;
    double balanceAfter;
};

// Structure for User (System Users)
struct User
{
    string username;
    string password;
    string role;
    bool isClient;
    bool isSuperAdmin;
    string email;
};

// Structure for Message
struct Message
{
    string from;
    string to;
    string content;
    string date;
    bool isRead;
};

// Structure for Currency
struct Currency
{
    string code;   // e.g., "USD", "EUR"
    string name;   // e.g., "US Dollar", "Euro"
    double rate;   // Exchange rate relative to base currency (USD = 1.0)
    string symbol; // e.g., "$", "€"
    bool isActive; // Whether currency is available for exchange
};
