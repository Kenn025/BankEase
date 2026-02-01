#include "../include/BankEase.h"

void BankingSystem::manageUsers()
{
    while (true)
    {
        clearScreen();
        displayHeader();
        cout << "\n";
        centerText("[2] Manage Users");
        centerText(string(70, '-'));
        leftCenterText("[1] System Users");
        leftCenterText("[2] Add Admin");
        leftCenterText("[3] Edit Admin");
        leftCenterText("[4] Delete Admin");
        leftCenterText("[0] Back to Main Menu");
        centerText(string(70, '-'));
        cout << string(15, ' ') << "Enter your choice: ";

        // Read input character by character, only allow digits
        string choiceStr = "";
        char ch;
        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!choiceStr.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter and continue waiting
                continue;
            }

            // Backspace
            if (ch == '\b' && !choiceStr.empty())
            {
                choiceStr.pop_back();
                cout << "\b \b";
            }
            // Only allow digits
            else if (isdigit(ch))
            {
                choiceStr += ch;
                cout << ch;
            }
            // Silently ignore letters and other characters
        }

        int choice = stoi(choiceStr);

        switch (choice)
        {
        case 1:
            viewSystemUsers();
            break;
        case 2:
            addAdmin();
            break;
        case 3:
            editAdmin();
            break;
        case 4:
            deleteAdmin();
            break;
        case 0:
            cout << "\n"
                 << string(15, ' ') << "Returning to Main Menu...\n";
            Sleep(1000); // Optional: pause for 1 second so user can see the message
            return;
        default:
            cout << string(15, ' ') << "Invalid choice!\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;
        }
    }
}

void BankingSystem::viewAllAdmins()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[View All Admin Accounts]");
    centerText(string(70, '-'));
    cout << "\n";

    cout << string(15, ' ') << left << setw(25) << "Username"
         << setw(30) << "Role" << "\n";
    cout << string(15, ' ') << string(55, '-') << "\n";

    for (const auto &pair : users)
    {
        if (!pair.second.isClient)
        {
            cout << string(15, ' ') << left << setw(25) << pair.second.username
                 << setw(30) << pair.second.role << "\n";
        }
    }

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::viewAdminAccountInfoReadOnly()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[10] My Account Information");
    centerText(string(70, '-'));
    cout << "\n";

    cout << string(15, ' ') << "Username : " << currentUser.username << "\n";
    cout << string(15, ' ') << "Role     : " << currentUser.role << "\n";
    cout << string(15, ' ') << "Email    : " << currentUser.email << "\n";
    cout << string(15, ' ') << "Type     : " << config.adminRoleLabel << "\n";
    cout << "\n"
         << string(15, ' ') << "Note: Only Super Admin can modify admin accounts.\n";

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::adminMenu()
{
    while (isLoggedIn)
    {
        clearScreen();
        displayHeader();
        cout << "\n";
        centerText("Admin Main Menu");
        centerText(string(70, '-'));

        cout << string(20, ' ') << "[1] Show Client List.\n";
        cout << string(20, ' ') << "[2] Add New Client.\n";
        cout << string(20, ' ') << "[3] Delete Client.\n";
        cout << string(20, ' ') << "[4] Update Client Info.\n";
        cout << string(20, ' ') << "[5] Find Client.\n";
        cout << string(20, ' ') << "[6] Transactions.\n";
        cout << string(20, ' ') << "[7] Login Register.\n";
        cout << string(20, ' ') << "[8] Communication.\n";
        cout << string(20, ' ') << "[9] Currency Exchange.\n";
        cout << string(20, ' ') << "[10] My Account Info.\n";
        cout << string(20, ' ') << "[11] Unlock Client Account(s).\n";
        cout << string(20, ' ') << "[12] Logout.\n";

        centerText(string(70, '-'));
        cout << string(15, ' ') << "Enter Your Choice from 1--->12 : ";

        string choiceStr = "";
        char ch;
        while (true)
        {
            ch = _getch();

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
            showClientList();
            break;
        case 2:
            addNewClient();
            break;
        case 3:
            deleteClient();
            break;
        case 4:
            updateClientInfo();
            break;
        case 5:
            findClient();
            break;
        case 6:
            transactionsMenu();
            break;
        case 7:
            loginRegister();
            break;
        case 8:
            communication();
            break;
        case 9:
            currencyExchange();
            break;
        case 10:
            viewAdminAccountInfoReadOnly(); // New read-only version
            break;
        case 11:
            unlockClientAccounts();
            break;
        case 12:
        case 13:
            cout << "\n";
            if (confirmLogout())
            {
                centerText("Logging out...");
                logActivity("Logout", "Admin logged out");
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
