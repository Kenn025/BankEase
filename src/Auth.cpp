#include "../include/BankEase.h"

bool BankingSystem::confirmLogout()
{
    while (true)
    {
        cout << string(15, ' ') << "Are you sure you want to logout? (y/n): ";

        // Read input character by character
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

            // Only allow y, Y, n, or N
            if (ch == 'y' || ch == 'Y' || ch == 'n' || ch == 'N')
            {
                cout << ch << endl; // Display the valid character
                break;
            }
            // Silently ignore all other keys (numbers, letters, symbols)
        }

        if (ch == 'y' || ch == 'Y')
            return true;
        if (ch == 'n' || ch == 'N')
        {
            cout << string(15, ' ') << "Logout cancelled. Returning to menu...";
            Sleep(1000);
            return false;
        }
    }
}

string BankingSystem::readAccessCode(const string &label)
{
    while (true)
    {
        clearScreen();
        int width = 70;
        int topPad = 7;
        for (int i = 0; i < topPad; ++i)
        {
            cout << "\n";
        }
        int padding = (width - static_cast<int>(label.length())) / 2;
        if (padding < 0)
            padding = 0;
        cout << string(padding, ' ') << label;
        cout.flush();

        string input;
        char ch;
#ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
#endif
        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                if (!input.empty())
                {
                    cout << "\n";
                    break;
                }
                continue;
            }
            if (ch == '\b' && !input.empty())
            {
                input.pop_back();
                cout << "\b \b";
                continue;
            }
            if (ch >= '0' && ch <= '9')
            {
                if (input.size() < 8)
                {
                    input.push_back(ch);
                    cout << u8"\u2022";
                }
            }
        }

        input = trim(input);
        if (input == "0")
        {
            return "0";
        }
        if (input.size() != 8)
        {
            cout << "\n";
            centerText("Access Code must be exactly 8 digits.");
            sleepMs(1200);
            continue;
        }
        return input;
    }
}

bool BankingSystem::setupSuperAdminAccount()
{
    clearScreen();
    cout << "\n";
    centerText(string(70, '-'));
    centerText("SUPER ADMIN SETUP");
    centerText(string(70, '-'));
    cout << "\n";

    cout << string(15, ' ') << "No Super Admin account found.\n";
    cout << string(15, ' ') << "Create one now (0 to cancel).\n\n";

    string username;
    while (true)
    {
        username = readRequiredField("Username: ");
        if (username == "0")
            return false;
        if (username.find(' ') != string::npos)
        {
            cout << string(15, ' ') << "Username cannot contain spaces.\n";
            continue;
        }
        if (users.find(username) != users.end())
        {
            cout << string(15, ' ') << "Username already exists.\n";
            continue;
        }
        break;
    }

    string password;
    while (true)
    {
        password = getHiddenInput(string(15, ' ') + "Password: ");
        password = trim(password);
        if (password == "0")
            return false;
        if (password.empty())
        {
            cout << string(15, ' ') << "Password cannot be empty.\n";
            continue;
        }
        break;
    }

    string email = readGmailField("Email: ");
    if (email == "0")
        return false;

    string accessCode = readAccessCode("Access Code (8 digits): ");
    if (accessCode == "0")
        return false;

    superAdminAccount.username = username;
    superAdminAccount.password = password;
    superAdminAccount.role = config.superAdminRoleLabel;
    superAdminAccount.isClient = false;
    superAdminAccount.isSuperAdmin = true;
    superAdminAccount.email = email;
    superAdminEmail = email;
    superAdminAccessCode = accessCode;
    hasSuperAdminAccount = true;

    users[username] = superAdminAccount;
    saveSuperAdminAccount();

    cout << "\n"
         << string(15, ' ') << "Super Admin account created.\n";
    sleepMs(1200);
    return true;
}

bool BankingSystem::superAdminLoginPage()
{
    if (!hasSuperAdminAccount)
    {
        if (!setupSuperAdminAccount())
        {
            return false;
        }
    }

    if (superAdminAccessCode.empty())
    {
        string accessCode = readAccessCode("Set Access Code (8 digits): ");
        if (accessCode == "0")
            return false;
        superAdminAccessCode = accessCode;
        saveSuperAdminAccount();
    }

    string accessTry = readAccessCode("Access Code (8 digits): ");
    if (accessTry == "0")
        return false;
    if (accessTry != superAdminAccessCode && accessTry != superAdminAccessCodeCipher)
    {
        cout << "\n"
             << string(15, ' ') << "Access denied. Press Enter to continue...";
        while (_getch() != '\r')
            ;
        return false;
    }

    string username = "";
    while (true)
    {
        clearScreen();
        cout << "\n";
        centerText(string(70, '-'));
        centerText("SUPER ADMIN LOGIN");
        centerText(string(70, '-'));
        cout << "\n";

        if (username.empty())
        {
            cout << string(15, ' ') << "Username: ";
            getline(cin, username);
            username = trim(username);

            if (username.empty())
            {
                cout << "\n"
                     << string(15, ' ') << "Username cannot be empty. Press Enter to try again...";
                while (_getch() != '\r')
                    ;
                continue;
            }
        }
        else
        {
            cout << string(15, ' ') << "Username: " << username << "\n";
        }

        string password = getHiddenInput(string(15, ' ') + "Password: ");
        password = trim(password);

        if (password.empty())
        {
            cout << "\n"
                 << string(15, ' ') << "Password cannot be empty. Press Enter to retype...";
            while (_getch() != '\r')
                ;
            continue;
        }

        if (users.find(username) != users.end() &&
            users[username].password == password &&
            !users[username].isClient &&
            users[username].isSuperAdmin)
        {
            currentUser = users[username];
            isLoggedIn = true;
            logActivity("Login", "Super admin login");
            cout << "\n"
                 << string(15, ' ') << "Login successful! Welcome, " << currentUser.username << "\n";
            sleepMs(1500);
            return true;
        }
        else
        {
            cout << "\n"
                 << string(15, ' ') << "Invalid Super Admin credentials! Press Enter to try again...";
            while (_getch() != '\r')
                ;
            username = "";
            continue;
        }
    }
}

bool BankingSystem::login()
{
    clearScreen();
    cout << "\n";
    centerText(string(70, '-'));
    centerText("LOGIN SYSTEM");
    centerText(string(70, '-'));
    cout << "\n";

    cout << string(15, ' ') << "[1] Admin\n";
    cout << string(15, ' ') << "[2] Client\n\n";
    cout << string(15, ' ') << "Select login type: ";

    // Read input character by character, only allow digits
    string loginTypeStr = "";
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
            cout << endl;
            break;
        }
        // Backspace key
        else if (ch == 8 || ch == 127)
        {
            if (!loginTypeStr.empty())
            {
                loginTypeStr.pop_back();
                cout << "\b \b";
            }
        }
        // Only allow digits 1-2
        else if (ch == '1' || ch == '2')
        {
            if (loginTypeStr.empty()) // Only allow one digit
            {
                loginTypeStr += ch;
                cout << ch;
            }
        }
        // Silently ignore all other keys (letters, symbols, other numbers)
    }

    // Convert to integer
    if (loginTypeStr.empty())
    {
        cout << "\n"
             << string(15, ' ') << "No selection made. Press Enter to try again...";
        cin.get();
        return false;
    }

    if (loginTypeStr == "1")
    {
        // Admin Login Loop
        string username = "";
        while (true)
        {
            clearScreen();
            cout << "\n";
            centerText(string(70, '-'));
            centerText("LOGIN SYSTEM");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "[1] Admin Login\n\n";

            // Only ask for username if it's empty
            if (username.empty())
            {
                cout << string(15, ' ') << "Username: ";
                getline(cin, username);
                username = trim(username);

                if (username.empty())
                {
                    cout << "\n"
                         << string(15, ' ') << "Username cannot be empty. Press Enter to try again...";
                    while (_getch() != '\r')
                        ;
                    continue;
                }
            }
            else
            {
                cout << string(15, ' ') << "Username: " << username << "\n";
            }

            // Password input with empty check and retry (same field)
            string password = getHiddenInput(string(15, ' ') + "Password: ");
            password = trim(password);

            if (password.empty())
            {
                cout << "\n"
                     << string(15, ' ') << "Password cannot be empty. Press Enter to retype...";
                while (_getch() != '\r')
                    ;
                continue;
            }

            // Validate Admin credentials (NOT Super Admin)
            if (users.find(username) != users.end() &&
                users[username].password == password &&
                !users[username].isClient &&
                !users[username].isSuperAdmin) // Must NOT be Super Admin
            {
                currentUser = users[username];
                isLoggedIn = true;
                logActivity("Login", "Admin login");
                cout << "\n"
                     << string(15, ' ') << "Login successful! Welcome, " << currentUser.username << "\n";
                sleepMs(1500);
                return true;
            }
            else
            {
                cout << "\n"
                     << string(15, ' ') << "Invalid Admin credentials! Press Enter to try again...";
                while (_getch() != '\r')
                    ;
                username = ""; // Reset username for invalid credentials
                continue;
            }
        }
    }
    else if (loginTypeStr == "2")
    {
        // Client Login Loop
        string accountNumber = "";
        while (true)
        {
            clearScreen();
            cout << "\n";
            centerText(string(70, '-'));
            centerText("LOGIN SYSTEM");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "[2] Client Login\n\n";

            // Only ask for account number if it's empty
            if (accountNumber.empty())
            {
                cout << string(15, ' ') << "Account Number: ";

                // Get account number with only digits allowed (max 10 digits)
                char ch;
                while ((ch = _getch()) != '\r') // Enter key
                {
                    if (ch == '\b' && !accountNumber.empty()) // Backspace
                    {
                        accountNumber.pop_back();
                        cout << "\b \b";
                    }
                    else if (isdigit(ch) && accountNumber.length() < 10) // Only allow digits, max 10
                    {
                        accountNumber += ch;
                        cout << ch;
                    }
                }
                cout << "\n";
                accountNumber = trim(accountNumber);

                if (accountNumber.empty())
                {
                    cout << "\n"
                         << string(15, ' ') << "Account Number cannot be empty. Press Enter to try again...";
                    while (_getch() != '\r')
                        ;
                    continue;
                }
            }
            else
            {
                cout << string(15, ' ') << "Account Number: " << accountNumber << "\n";
            }

            // Check if account exists
            auto clientIt = clients.find(accountNumber);
            if (clientIt != clients.end())
            {
                // Check if locked
                if (clientIt->second.locked)
                {
                    cout << string(15, ' ') << "Account locked after multiple failed logins. Please contact admin.\n";
                    cout << string(15, ' ') << "Press Enter to return...";
                    while (_getch() != '\r')
                        ;
                    return false;
                }
            }

            // Get PIN code with only digits allowed (hidden input with bullet character, max 6 digits)
            string pinCode = "";
            cout << string(15, ' ') << "PIN Code: ";

#ifdef _WIN32
            SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 output for Windows
#endif

            char ch;
            while ((ch = _getch()) != '\r') // Enter key
            {
                if (ch == '\b' && !pinCode.empty()) // Backspace
                {
                    pinCode.pop_back();
                    cout << "\b \b";
                }
                else if (isdigit(ch) && pinCode.length() < 6) // Only allow digits, max 6
                {
                    pinCode += ch;
                    cout << "•"; // Display bullet character
                    cout.flush();
                }
            }
            cout << "\n";

            pinCode = trim(pinCode);

            if (pinCode.empty())
            {
                cout << "\n"
                     << string(15, ' ') << "PIN Code cannot be empty. Press Enter to retype...";
                while (_getch() != '\r')
                    ;
                continue;
            }

            // Validate credentials
            if (clientIt != clients.end())
            {
                if (clientIt->second.pinCode == pinCode)
                {
                    // Reset failed attempts and unlock if previously locked
                    clientIt->second.failedAttempts = 0;
                    clientIt->second.locked = false;
                    saveClients();

                    if (users.find(accountNumber) != users.end())
                    {
                        currentUser = users[accountNumber];
                        currentUser.password = clientIt->second.pinCode;
                        users[accountNumber].password = clientIt->second.pinCode;
                        saveUsers();
                    }
                    else
                    {
                        User clientUser;
                        clientUser.username = accountNumber;
                        clientUser.password = clientIt->second.pinCode;
                        clientUser.role = "Client";
                        clientUser.isClient = true;
                        clientUser.isSuperAdmin = false; // Clients are not Super Admin
                        users[accountNumber] = clientUser;
                        saveUsers();
                        currentUser = clientUser;
                    }

                    isLoggedIn = true;
                    logActivity("Login", "Client login");
                    cout << "\n"
                         << string(15, ' ') << "Login successful! Welcome, "
                         << formatFullName(clientIt->second, false) << "\n";
                    sleepMs(1500);
                    return true;
                }
                else
                {
                    // Increment failed attempts and lock if needed
                    clientIt->second.failedAttempts++;
                    if (clientIt->second.failedAttempts >= 3)
                    {
                        clientIt->second.locked = true;
                        saveClients();
                        cout << "\n"
                             << string(15, ' ') << "Account locked after 3 failed login attempts. Please contact an admin.\n";
                        cout << string(15, ' ') << "Press Enter to go back...";
                        while (_getch() != '\r')
                            ;
                        return false;
                    }
                    else
                    {
                        saveClients();
                        cout << "\n"
                             << string(15, ' ') << "Invalid credentials! Press Enter to try again...";
                        while (_getch() != '\r')
                            ;
                        // Do NOT reset accountNumber, so user can retry PIN for same account
                        continue;
                    }
                }
            }
            else
            {
                cout << "\n"
                     << string(15, ' ') << "Invalid credentials! Press Enter to try again...";
                while (_getch() != '\r')
                    ;
                accountNumber = ""; // Reset account number for invalid credentials
                continue;
            }
        }
    }

    return false;
}
