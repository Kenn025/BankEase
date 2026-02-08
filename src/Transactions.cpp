#include "../include/BankEase.h"

void BankingSystem::transactionsMenu()
{
    while (true)
    {
        clearScreen();
        displayHeader();
        cout << "\n";
        centerText("[6] Transactions");
        centerText(string(70, '-'));
        leftCenterText("[1] Deposit");
        leftCenterText("[2] Withdraw");
        leftCenterText("[3] Transfer");
        leftCenterText("[4] View Transaction History");
        leftCenterText("[0] Back to Main Menu");
        centerText(string(70, '-'));
        cout << string(15, ' ') << "Enter your choice: ";

        // Read input character by character, only allow digits
        string choiceStr = "";
        char ch;
        while (true)
        {
#ifdef _WIN32
            ch = _getch(); // Windows
#else
            system("stty raw -echo"); // Linux/Mac
            ch = getchar();
            system("stty cooked echo");
#endif

            // Enter key
            if (ch == '\r' || ch == '\n')
            {
                // Only break if choiceStr is not empty
                if (!choiceStr.empty())
                {
                    cout << endl;
                    break;
                }
                // Silently ignore Enter if choiceStr is empty
            }
            // Backspace key
            else if (ch == 8 || ch == 127)
            {
                if (!choiceStr.empty())
                {
                    choiceStr.pop_back();
                    cout << "\b \b";
                }
            }
            // Allow only single digit (0-4) for valid menu options
            else if (ch >= '0' && ch <= '9')
            {
                // Limit to 1 character to prevent multi-digit invalid inputs
                if (choiceStr.empty())
                {
                    choiceStr += ch;
                    cout << ch;
                }
                // Silently ignore if already has 1 digit
            }
            // Silently ignore all other keys (letters, symbols, special characters)
        }

        // Convert to integer
        int choice = stoi(choiceStr);

        switch (choice)
        {
        case 1:
            deposit();
            break;
        case 2:
            withdraw();
            break;
        case 3:
            transfer();
            break;
        case 4:
            viewTransactionHistory();
            break;
        case 0:
            cout << "\n"
                 << string(15, ' ') << "Returning to Main Menu...";
            Sleep(1000);
            return;
        default:
            cout << "\n"
                 << string(15, ' ') << "Invalid choice! Please try again.";
            Sleep(1000);
            break;
        }
    }
}

void BankingSystem::deposit()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Deposit]");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    string accNum;
    double amount;

    if (currentUser.isClient)
    {
        accNum = currentUser.username;
        cout << string(15, ' ') << "Account: " << formatFullName(clients[accNum], false) << " (" << accNum << ")\n";
    }
    else
    {
        accNum = readRequiredField("Account Number: ");
        if (accNum == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            return;
        }

        if (clients.find(accNum) == clients.end())
        {
            cout << "\n"
                 << string(15, ' ') << "Client not found!\n";
            return;
        }
    }

    cout << string(15, ' ') << "Amount to deposit: $";

    // Only allow numbers, decimal point, and Enter
    string input = "";
    char ch;
    while (true)
    {
        ch = _getch();

        // Allow Enter to submit ONLY if input is not empty
        if (ch == '\r')
        {
            if (!input.empty()) // Only break if something was typed
            {
                cout << "\n";
                break;
            }
            // If empty, do nothing (ignore Enter)
            continue;
        }

        // Allow backspace
        if (ch == '\b' && input.length() > 0)
        {
            input.pop_back();
            cout << "\b \b"; // Erase character from screen
            continue;
        }

        // Only allow digits and one decimal point
        if ((ch >= '0' && ch <= '9') || (ch == '.' && input.find('.') == string::npos))
        {
            input += ch;
            cout << ch; // Show the character
        }
        // If it's a letter or invalid character, do nothing (block it)
    }

    // Convert string to double
    amount = stod(input);

    if (amount == 0)
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        return;
    }

    if (amount <= 0)
    {
        cout << "\n"
             << string(15, ' ') << "Invalid amount!\n";
        return;
    }

    clients[accNum].balance += amount;
    saveClients();
    addTransaction(accNum, "Deposit", amount);
    {
        ostringstream detail;
        detail << "Account " << accNum << ", Amount " << fixed << setprecision(2) << amount;
        logActivity("Deposit", detail.str());
    }

    cout << "\n"
         << string(15, ' ') << "Deposit successful!\n";
    cout << string(15, ' ') << "New Balance: $" << fixed << setprecision(2) << clients[accNum].balance << "\n";
    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::withdraw()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Withdraw]");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    string accNum;
    double amount;

    if (currentUser.isClient)
    {
        accNum = currentUser.username;
        cout << string(15, ' ') << "Account: " << formatFullName(clients[accNum], false) << " (" << accNum << ")\n";
    }
    else
    {
        accNum = readRequiredField("Account Number: ");
        if (accNum == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            return;
        }

        if (clients.find(accNum) == clients.end())
        {
            cout << "\n"
                 << string(15, ' ') << "Client not found!\n";
            return;
        }
    }

    cout << string(15, ' ') << "Current Balance: $" << fixed << setprecision(2) << clients[accNum].balance << "\n";
    cout << string(15, ' ') << "Amount to withdraw: $";

    // Only allow numbers, decimal point, and Enter
    string input = "";
    char ch;
    while (true)
    {
        ch = _getch();

        // Allow Enter to submit ONLY if input is not empty
        if (ch == '\r')
        {
            if (!input.empty()) // Only break if something was typed
            {
                cout << "\n";
                break;
            }
            // If empty, do nothing (ignore Enter)
            continue;
        }

        // Allow backspace
        if (ch == '\b' && input.length() > 0)
        {
            input.pop_back();
            cout << "\b \b"; // Erase character from screen
            continue;
        }

        // Only allow digits and one decimal point
        if ((ch >= '0' && ch <= '9') || (ch == '.' && input.find('.') == string::npos))
        {
            input += ch;
            cout << ch; // Show the character
        }
        // If it's a letter or invalid character, do nothing (block it)
    }

    // Convert string to double
    amount = stod(input);

    if (amount == 0)
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        return;
    }

    if (amount <= 0 || amount > clients[accNum].balance)
    {
        cout << "\n"
             << string(15, ' ') << "Invalid amount or insufficient funds!\n";
        return;
    }

    clients[accNum].balance -= amount;
    saveClients();
    addTransaction(accNum, "Withdraw", amount);
    {
        ostringstream detail;
        detail << "Account " << accNum << ", Amount " << fixed << setprecision(2) << amount;
        logActivity("Withdraw", detail.str());
    }

    cout << "\n"
         << string(15, ' ') << "Withdrawal successful!\n";
    cout << string(15, ' ') << "New Balance: $" << fixed << setprecision(2) << clients[accNum].balance << "\n";
    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::transfer()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Transfer]");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    string fromAcc, toAcc;
    double amount;

    if (currentUser.isClient)
    {
        fromAcc = currentUser.username;
        cout << string(15, ' ') << "From: " << formatFullName(clients[fromAcc], false) << " (" << fromAcc << ")\n";

        // Block letters, only allow digits for account number
        cout << string(15, ' ') << "To Account Number: ";
        toAcc = "";
        char ch;
        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!toAcc.empty())
                {
                    cout << "\n";
                    break;
                }
                continue;
            }

            // Backspace
            if (ch == '\b' && toAcc.length() > 0)
            {
                toAcc.pop_back();
                cout << "\b \b";
                continue;
            }

            // Only allow digits
            if (ch >= '0' && ch <= '9')
            {
                toAcc += ch;
                cout << ch;
            }
        }
    }
    else
    {
        fromAcc = readRequiredField("From Account Number: ");
        if (fromAcc == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            return;
        }

        // Block letters, only allow digits for account number
        cout << string(15, ' ') << "To Account Number: ";
        toAcc = "";
        char ch;
        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!toAcc.empty())
                {
                    cout << "\n";
                    break;
                }
                continue;
            }

            // Backspace
            if (ch == '\b' && toAcc.length() > 0)
            {
                toAcc.pop_back();
                cout << "\b \b";
                continue;
            }

            // Only allow digits
            if (ch >= '0' && ch <= '9')
            {
                toAcc += ch;
                cout << ch;
            }
        }
    }

    if (toAcc == "0")
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        return;
    }

    if (clients.find(fromAcc) == clients.end() || clients.find(toAcc) == clients.end())
    {
        cout << "\n"
             << string(15, ' ') << "Account not found!\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    cout << string(15, ' ') << "Amount to transfer: $";

    // Only allow numbers and decimal point
    string input = "";
    char ch;
    while (true)
    {
        ch = _getch();

        // Enter key - only accept if input is not empty
        if (ch == '\r')
        {
            if (!input.empty())
            {
                cout << "\n";
                break;
            }
            continue;
        }

        // Backspace
        if (ch == '\b' && input.length() > 0)
        {
            input.pop_back();
            cout << "\b \b";
            continue;
        }

        // Only allow digits and one decimal point
        if ((ch >= '0' && ch <= '9') || (ch == '.' && input.find('.') == string::npos))
        {
            input += ch;
            cout << ch;
        }
    }

    // Convert string to double
    amount = stod(input);

    if (amount == 0)
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        return;
    }

    if (amount <= 0 || amount > clients[fromAcc].balance)
    {
        cout << "\n"
             << string(15, ' ') << "Invalid amount or insufficient funds!\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    clients[fromAcc].balance -= amount;
    clients[toAcc].balance += amount;
    saveClients();

    addTransaction(fromAcc, "Transfer Out to " + toAcc, amount);
    addTransaction(toAcc, "Transfer In from " + fromAcc, amount);
    {
        ostringstream detail;
        detail << "From " << fromAcc << " to " << toAcc << ", Amount " << fixed << setprecision(2) << amount;
        logActivity("Transfer", detail.str());
    }

    cout << "\n"
         << string(15, ' ') << "Transfer successful!\n";
    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::viewTransactionHistory()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Transaction History]");
    centerText(string(70, '-'));
    cout << "\n";

    string accNum;

    if (currentUser.isClient)
    {
        accNum = currentUser.username;
        cout << string(15, ' ') << "Viewing transactions for: " << formatFullName(clients[accNum], true) << "\n\n";
    }
    else
    {
        cout << string(15, ' ') << "Enter Account Number (or 'all', or '0' to cancel): ";
        getline(cin, accNum);
        if (accNum == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Press Enter to go back...";
            while (_getch() != '\r')
                ;
            return;
        }
    }

    bool found = false;
    cout << "\n"
         << string(5, ' ') << left << setw(15) << "Account No."
         << setw(25) << "Type"
         << setw(12) << "Amount" << "Date\n";
    cout << string(5, ' ') << string(90, '-') << "\n";

    for (const Transaction &t : transactions)
    {
        if (accNum == "all" || t.accountNumber == accNum)
        {
            cout << string(5, ' ') << left << setw(15) << t.accountNumber
                 << setw(25) << t.type
                 << "$" << setw(11) << fixed << setprecision(2) << t.amount
                 << t.date << "\n";
            found = true;
        }
    }

    if (!found)
    {
        centerText("No transactions found.");
    }

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}
