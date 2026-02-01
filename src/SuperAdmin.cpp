#include "../include/BankEase.h"

void BankingSystem::loginRegister()
{
    clearScreen();
    displayHeader();
    cout << "\n";

    // Auto-detect menu number based on user role
    int menuNumber = currentUser.isSuperAdmin ? 4 : 7;
    string title = "[" + to_string(menuNumber) + "] Login Register (Audit Log)";
    centerText(title);
    centerText(string(70, '-'));
    cout << "\n";

    cout << string(15, ' ') << "Feature: View login history and audit trails\n";
    cout << string(15, ' ') << "Current session: " << currentUser.username << "\n";
    cout << string(15, ' ') << "Login time: " << getCurrentDateTime() << "\n";

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::viewSuperAdminAccountInfo()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[6] My Super Admin Account Information");
    centerText(string(70, '-'));
    cout << "\n";

    cout << string(15, ' ') << "Type '0' at any configurable field to cancel the operation.\n\n";

    cout << string(15, ' ') << "Username          : " << currentUser.username << "\n";
    cout << string(15, ' ') << "Role              : " << currentUser.role << "\n";
    cout << string(15, ' ') << "Email             : " << superAdminEmail << "\n";
    cout << string(15, ' ') << "SMTP From Email   : " << (config.smtpFromEmail.empty() ? "(not set)" : config.smtpFromEmail) << "\n";
    cout << string(15, ' ') << "Access Code Set   : " << (superAdminAccessCode.empty() ? "No" : "Yes") << "\n";
    cout << string(15, ' ') << "SMTP Password Set : " << (gmailAppPassword.empty() ? "No" : "Yes") << "\n\n";

    bool changesMade = false;

    // ============================
    // CHANGE SUPER ADMIN EMAIL
    // ============================
    string input;
    cout << string(15, ' ') << "Change Email? (y/n): ";
    while (true)
    {
        input.clear();
        char ch;
        while (true)
        {
            ch = _getch();

            // If Enter is pressed
            if (ch == '\r' || ch == '\n')
            {
                // If input is empty, ignore and don't move cursor
                if (input.empty())
                {
                    continue;
                }
                else
                {
                    cout << "\n"; // Move to next line only when valid input
                    break;
                }
            }
            // If backspace
            else if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
            }
            // Regular character
            else if (ch >= 32 && ch <= 126)
            {
                input += ch;
                cout << ch;
            }
        }

        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input == "0")
        {
            cout << "\n";
            cout << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...\n";
            Sleep(1000);
            return;
        }

        if (input.length() == 1 && (input[0] == 'y' || input[0] == 'Y' ||
                                    input[0] == 'n' || input[0] == 'N'))
        {
            if (input[0] == 'y' || input[0] == 'Y')
            {
                string newEmail = readGmailField("New Email: ");

                if (newEmail == "0")
                {
                    cout << "\n";
                    cout << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to menu...\n";
                    Sleep(1000);
                    return;
                }

                superAdminEmail = newEmail;
                superAdminAccount.email = newEmail;
                saveSuperAdminAccount();
                users[currentUser.username].email = newEmail;
                currentUser.email = newEmail;
                cout << string(15, ' ') << "Email updated.\n";
                changesMade = true;
            }
            break;
        }
        else
        {
            cout << string(15, ' ') << "Invalid input! Please enter 'y', 'n', or '0'.\n";
            cout << string(15, ' ') << "Change Email? (y/n): ";
        }
    }

    // ============================
    // CONFIGURE APP PASSWORD
    // ============================
    cout << "\n"
         << string(15, ' ') << "Configure SMTP Password? (y/n): ";
    while (true)
    {
        input.clear();
        char ch;
        while (true)
        {
            ch = _getch();

            // If Enter is pressed
            if (ch == '\r' || ch == '\n')
            {
                // If input is empty, ignore and don't move cursor
                if (input.empty())
                {
                    continue;
                }
                else
                {
                    cout << "\n"; // Move to next line only when valid input
                    break;
                }
            }
            // If backspace
            else if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
            }
            // Regular character
            else if (ch >= 32 && ch <= 126)
            {
                input += ch;
                cout << ch;
            }
        }

        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input == "0")
        {
            cout << "\n";
            cout << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...\n";
            Sleep(1000);
            return;
        }

        if (input.length() == 1 && (input[0] == 'y' || input[0] == 'Y' ||
                                    input[0] == 'n' || input[0] == 'N'))
        {
            if (input[0] == 'y' || input[0] == 'Y')
            {
                string newPass = getHiddenInput(string(15, ' ') + "SMTP App Password: ");

                // Trim whitespace
                newPass.erase(0, newPass.find_first_not_of(" \t\n\r"));
                newPass.erase(newPass.find_last_not_of(" \t\n\r") + 1);

                if (newPass == "0")
                {
                    cout << "\n";
                    cout << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to menu...\n";
                    Sleep(1000);
                    return;
                }

                if (!newPass.empty())
                {
                    gmailAppPassword = newPass;
                    saveGmailAppPassword();
                    cout << string(15, ' ') << "App Password updated.\n";
                    changesMade = true;
                }
                else
                {
                    cout << string(15, ' ') << "Password cannot be empty. Skipping...\n";
                }
            }
            break;
        }
        else
        {
            cout << string(15, ' ') << "Invalid input! Please enter 'y', 'n', or '0'.\n";
            cout << string(15, ' ') << "Configure SMTP Password? (y/n): ";
        }
    }

    // ============================
    // CHANGE ACCESS CODE
    // ============================
    cout << "\n"
         << string(15, ' ') << "Change Access Code? (y/n): ";
    while (true)
    {
        input.clear();
        char ch;
        while (true)
        {
            ch = _getch();

            // If Enter is pressed
            if (ch == '\r' || ch == '\n')
            {
                // If input is empty, ignore and don't move cursor
                if (input.empty())
                {
                    continue;
                }
                else
                {
                    cout << "\n"; // Move to next line only when valid input
                    break;
                }
            }
            // If backspace
            else if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
            }
            // Regular character
            else if (ch >= 32 && ch <= 126)
            {
                input += ch;
                cout << ch;
            }
        }

        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input == "0")
        {
            cout << "\n";
            cout << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...\n";
            Sleep(1000);
            return;
        }

        if (input.length() == 1 && (input[0] == 'y' || input[0] == 'Y' ||
                                    input[0] == 'n' || input[0] == 'N'))
        {
            if (input[0] == 'y' || input[0] == 'Y')
            {
                string newCode = readAccessCode("New Access Code (8 digits): ");

                if (newCode == "0")
                {
                    cout << "\n";
                    cout << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to menu...\n";
                    Sleep(1000);
                    return;
                }

                if (!newCode.empty())
                {
                    superAdminAccessCode = newCode;
                    saveSuperAdminAccount();
                    cout << string(15, ' ') << "Access code updated.\n";
                    changesMade = true;
                }
                else
                {
                    cout << string(15, ' ') << "Access code cannot be empty. Skipping...\n";
                }
            }
            break;
        }
        else
        {
            cout << string(15, ' ') << "Invalid input! Please enter 'y', 'n', or '0'.\n";
            cout << string(15, ' ') << "Change Access Code? (y/n): ";
        }
    }

    // ============================
    // CHANGE SUPER ADMIN PASSWORD
    // ============================
    cout << "\n"
         << string(15, ' ') << "Change Super Admin Password? (y/n): ";
    while (true)
    {
        input.clear();
        char ch;
        while (true)
        {
            ch = _getch();

            // If Enter is pressed
            if (ch == '\r' || ch == '\n')
            {
                // If input is empty, ignore and don't move cursor
                if (input.empty())
                {
                    continue;
                }
                else
                {
                    cout << "\n"; // Move to next line only when valid input
                    break;
                }
            }
            // If backspace
            else if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
            }
            // Regular character
            else if (ch >= 32 && ch <= 126)
            {
                input += ch;
                cout << ch;
            }
        }

        // Trim whitespace
        input.erase(0, input.find_first_not_of(" \t\n\r"));
        input.erase(input.find_last_not_of(" \t\n\r") + 1);

        if (input == "0")
        {
            cout << "\n";
            cout << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...\n";
            Sleep(1000);
            return;
        }

        if (input.length() == 1 && (input[0] == 'y' || input[0] == 'Y' ||
                                    input[0] == 'n' || input[0] == 'N'))
        {
            if (input[0] == 'y' || input[0] == 'Y')
            {
                string newPass = getHiddenInput(string(15, ' ') + "New Password: ");

                // Trim whitespace
                newPass.erase(0, newPass.find_first_not_of(" \t\n\r"));
                newPass.erase(newPass.find_last_not_of(" \t\n\r") + 1);

                if (newPass == "0")
                {
                    cout << "\n";
                    cout << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to menu...\n";
                    Sleep(1000);
                    return;
                }

                if (!newPass.empty())
                {
                    users[currentUser.username].password = newPass;
                    currentUser.password = newPass;
                    superAdminAccount.password = newPass;
                    saveSuperAdminAccount();
                    cout << string(15, ' ') << "Password updated.\n";
                    changesMade = true;
                }
                else
                {
                    cout << string(15, ' ') << "Password cannot be empty. Skipping...\n";
                }
            }
            break;
        }
        else
        {
            cout << string(15, ' ') << "Invalid input! Please enter 'y', 'n', or '0'.\n";
            cout << string(15, ' ') << "Change Super Admin Password? (y/n): ";
        }
    }

    // Display final message
    if (!changesMade)
    {
        cout << "\n"
             << string(15, ' ') << "No changes made.\n";
    }

    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::superAdminMenu()
{
    while (isLoggedIn)
    {
        clearScreen();
        displayHeader();
        cout << "\n";
        centerText("Super Admin Main Menu");
        centerText(string(70, '-'));

        cout << string(20, ' ') << "[1] View All Clients.\n";
        cout << string(20, ' ') << "[2] Manage Users.\n";
        cout << string(20, ' ') << "[3] System Reports.\n";
        cout << string(20, ' ') << "[4] Login Register (Audit Log).\n";
        cout << string(20, ' ') << "[5] Activity Logs.\n";
        cout << string(20, ' ') << "[6] My Super Admin Account.\n";
        cout << string(20, ' ') << "[7] Logout.\n";

        centerText(string(70, '-'));
        cout << string(15, ' ') << "Enter Your Choice from 1--->7 : ";

        string choiceStr = "";
        char ch;
        while (true)
        {
#ifdef _WIN32
            ch = _getch();
#else
            system("stty raw -echo");
            ch = getchar();
            system("stty cooked echo");
#endif

            if (ch == '\r' || ch == '\n')
            {
                if (!choiceStr.empty())
                {
                    cout << endl;
                    break;
                }
                continue;
            }
            else if (ch == 8 || ch == 127)
            {
                if (!choiceStr.empty())
                {
                    choiceStr.pop_back();
                    cout << "\b \b";
                }
            }
            else if (ch >= '0' && ch <= '9')
            {
                choiceStr += ch;
                cout << ch;
            }
        }

        int choice = stoi(choiceStr);

        switch (choice)
        {
        case 1:
            showClientList(); // View only, no modifications
            break;
        case 2:
            manageUsers(); // Create, delete admins
            break;
        case 3:
            systemReports(); // System statistics and overview
            break;
        case 4:
            loginRegister(); // Audit trail
            break;
        case 5:
            viewActivityLogs();
            break;
        case 6:
            viewSuperAdminAccountInfo(); // Super Admin settings
            break;
        case 7:
            cout << "\n";
            if (confirmLogout())
            {
                centerText("Logging out...");
                logActivity("Logout", "Super admin logged out");
                sleepMs(1500);
                isLoggedIn = false;
                return;
            }
            break;
        default:
            cout << "\n"
                 << string(15, ' ') << "Invalid choice!\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;
        }
    }
}

// System Reports (Super Admin only)
void BankingSystem::systemReports()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[3] System Reports");
    centerText(string(70, '-'));
    cout << "\n";

    // Calculate statistics
    int totalClients = clients.size();
    int totalAdmins = 0;
    int totalSuperAdmins = 0;
    double totalSystemBalance = 0.0;
    int totalTransactions = transactions.size();

    for (const auto &pair : users)
    {
        if (!pair.second.isClient)
        {
            if (pair.second.isSuperAdmin)
                totalSuperAdmins++;
            else
                totalAdmins++;
        }
    }

    for (const auto &pair : clients)
    {
        totalSystemBalance += pair.second.balance;
    }

    cout << string(15, ' ') << "=== SYSTEM OVERVIEW ===\n\n";
    cout << string(15, ' ') << "Total Clients        : " << totalClients << "\n";
    cout << string(15, ' ') << "Total Admins         : " << totalAdmins << "\n";
    cout << string(15, ' ') << "Total Super Admins   : " << totalSuperAdmins << "\n";
    cout << string(15, ' ') << "Total System Balance : $" << fixed << setprecision(2) << totalSystemBalance << "\n";
    cout << string(15, ' ') << "Total Transactions   : " << totalTransactions << "\n";

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}
