#pragma once

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

#include "Models.h"
#include "Utils.h"

using namespace std;

class BankingSystem
{
private:
    struct Config
    {
        string allowedDomains;
        string smtpHost;
        int smtpPort = 465;
        bool smtpUseSsl = true;
        string smtpFromEmail;
        string smtpPassword;
        string adminRoleLabel;
        string superAdminRoleLabel;
        string dataDir;
    };

    Config config;
    map<string, Client> clients;
    map<string, User> users;
    vector<Transaction> transactions;
    vector<Message> messages;
    map<string, Currency> currencies; // NEW: Dynamic currency storage
    User currentUser;
    bool isLoggedIn;
    int lastAccountNumber;
    User superAdminAccount;
    bool hasSuperAdminAccount = false;
    string superAdminAccessCode;
    string superAdminAccessCodeCipher;
    string superAdminEmail;
    string gmailAppPassword;

    void centerText(const string &text, int width = 70);

    void leftCenterText(const string &text, int width = 70);

    string getCurrentDateTime();

    string generateAccountNumber();

    string dataPath(const string &filename);
    void loadConfig();
    void saveConfig();
    void setupConfig();
    bool isAllowedEmail(const string &rawEmail);

    void loadSuperAdminAccount();

    void saveSuperAdminAccount();

    void loadGmailAppPassword();

    void saveGmailAppPassword();

    void logActivity(const string &action, const string &detail);

    // Send real email using curl
    bool sendRealEmail(const string &headerFromEmail, const string &toEmail,
                       const string &subject, const string &body,
                       const string &replyTo);

    // ====== common input helpers (avoid empty / space-only) ======
    string readRequiredField(const string &label, bool allowZeroCancel = true);

    string readGmailField(const string &label, bool allowZeroCancel = true);

    bool confirmLogout();
    bool setupSuperAdminAccount();
    bool superAdminLoginPage();
    string readAccessCode(const string &label);

    void loadData();

    void saveLastAccountNumber();

    void saveClients();

    void saveUsers();

    void saveTransactions();

    void saveMessages();

    void loadCurrencies(); // NEW
    void saveCurrencies(); // NEW

    void addTransaction(const string &accountNumber, const string &type, double amount);

    bool findUserByEmail(const string &email, string &userIdOut);

    string getEmailForUserId(const string &userId);

    string getDisplayNameForUserId(const string &userId);

    // ADD THE HELPER FUNCTION HERE
    string formatFullName(const Client &c, bool lastNameFirst = false);

public:
    BankingSystem();

    void clearScreen();

    void displayHeader();

    bool login();

    void showClientList();

    string getNumericInputLimited(const string &prompt, int maxLen);

    void addNewClient();

    void deleteClient();

    void updateClientInfo();

    void findClient();

    void transactionsMenu();

    void deposit();

    void withdraw();

    void transfer();

    void viewTransactionHistory();

    void manageUsers();

    void viewSystemUsers();

    void addAdmin();

    void editAdmin();

    void deleteAdmin();

    void loginRegister();

    void communication();

    void sendMessage();

    void viewInbox();

    void currencyExchange();

    void viewAccountInfo();

    void changeClientPin();

    void viewSuperAdminAccountInfo();

    void viewActivityLogs();

    void superAdminMenu();

    void viewAllAdmins();

    // System Reports (Super Admin only)
    void systemReports();

    void viewAdminAccountInfoReadOnly();

    void adminMenu();

    // Unlock client accounts page
    void unlockClientAccounts();

    void clientMenu();

    void runSuperAdmin();

    void run();
};
