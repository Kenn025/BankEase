#include "../include/BankEase.h"

// ====== common input helpers (avoid empty / space-only) ======
string BankingSystem::readRequiredField(const string &label, bool allowZeroCancel)
{
    while (true)
    {
        cout << string(15, ' ') << label;
        string input;
        getline(cin, input);
        string t = trim(input);

        if (allowZeroCancel && t == "0")
        {
            return "0";
        }

        if (t.empty())
        {
            cout << string(15, ' ')
                 << "This field cannot be empty. Please try again.\n";
            continue;
        }

        return t;
    }
}

string BankingSystem::readGmailField(const string &label, bool allowZeroCancel)
{
    while (true)
    {
        cout << string(15, ' ') << label;
        string input;
        getline(cin, input);
        string t = trim(input);

        if (allowZeroCancel && t == "0")
        {
            return "0";
        }

        if (!isAllowedEmail(t))
        {
            cout << string(15, ' ')
                 << "Invalid email address. Please try again.\n";
            continue;
        }

        return t;
    }
}

BankingSystem::BankingSystem() : isLoggedIn(false)
{
    loadConfig();
    loadSuperAdminAccount();
    loadData();
    loadGmailAppPassword();
    loadCurrencies(); // Load currency data
}

string BankingSystem::getNumericInputLimited(const string &prompt, int maxLen)
{
    cout << string(15, ' ') << prompt;
    cout.flush();

    string input;
    char ch;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);

    while (true)
    {
        ch = _getch();

        if (ch == 13) // ENTER
        {
            cout << "\n";
            break;
        }
        else if (ch == 8) // BACKSPACE
        {
            if (!input.empty())
            {
                input.pop_back();
                cout << "\b \b";
            }
        }
        else if (ch >= '0' && ch <= '9') // DIGITS ONLY
        {
            if ((int)input.length() < maxLen) // limit to max length
            {
                input.push_back(ch);
                cout << ch;
            }
        }
        // ignore everything else
    }

#endif

    return input;
}

void BankingSystem::viewSystemUsers()
{
    clearScreen();
    displayHeader();
    centerText("[System Users]");

    // Calculate total width properly
    const int usernameWidth = 20;
    const int roleWidth = 30;
    const int emailWidth = 34;
    const int totalWidth = 94;

    // Top border
    cout << string(totalWidth, '-') << "\n";

    // Header
    cout << "| " << left << setw(usernameWidth) << "Username"
         << " | " << setw(roleWidth) << "Role"
         << " | " << setw(emailWidth) << "Email" << " |\n";

    // Middle separator
    cout << string(totalWidth, '-') << "\n";

    // Create a vector to sort users
    vector<pair<string, const User *>> sortedUsers;

    for (const auto &pair : users)
    {
        sortedUsers.push_back({pair.first, &pair.second});
    }

    // Sort users: Admins first, then Clients
    // Priority: Super Admin > Admin > Client
    sort(sortedUsers.begin(), sortedUsers.end(),
         [](const pair<string, const User *> &a, const pair<string, const User *> &b)
         {
             const User *userA = a.second;
             const User *userB = b.second;

             // Assign priority values
             int priorityA = userA->isSuperAdmin ? 0 : (userA->isClient ? 2 : 1);
             int priorityB = userB->isSuperAdmin ? 0 : (userB->isClient ? 2 : 1);

             // Sort by priority first, then by username alphabetically
             if (priorityA != priorityB)
             {
                 return priorityA < priorityB;
             }
             return userA->username < userB->username;
         });

    // Data rows - Display sorted users
    for (const auto &pair : sortedUsers)
    {
        const User &u = *pair.second;

        string email;
        string role;

        // Determine email and role based on user type
        if (u.isSuperAdmin)
        {
            email = superAdminEmail;
            role = u.role;
        }
        else if (u.isClient)
        {
            // Get client email from clients map
            auto clientIt = clients.find(u.username);
            if (clientIt != clients.end())
            {
                email = clientIt->second.email;
            }
            else
            {
                email = "(no-email)";
            }
            role = "Client";
        }
        else
        {
            // Regular admin
            email = u.email;
            role = u.role;
        }

        // Truncate if too long (leave space for "...")
        string username = u.username;
        if (username.length() > usernameWidth)
        {
            username = username.substr(0, usernameWidth - 3) + "...";
        }

        if (role.length() > roleWidth)
        {
            role = role.substr(0, roleWidth - 3) + "...";
        }

        if (email.length() > emailWidth)
        {
            email = email.substr(0, emailWidth - 3) + "...";
        }

        // Print row - pad each field exactly to its width
        cout << "| " << left << setw(usernameWidth) << username
             << " | " << setw(roleWidth) << role
             << " | " << setw(emailWidth) << email << " |\n";
    }

    // Bottom border
    cout << string(totalWidth, '-') << "\n";

    cout << "\n"
         << string(20, ' ') << "Press Enter to go back...";

    // Only allow Enter key, block all other input
    while (_getch() != '\r')
        ;
}

void BankingSystem::addAdmin()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Add New Admin]");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    User u;
    u.isClient = false;
    u.isSuperAdmin = false;

    // Username validation regex patterns
    regex usernameRegex("^[a-zA-Z0-9_]{4,15}$");
    regex hasLetter(".*[a-zA-Z].*");

    while (true)
    {
        cout << string(15, ' ') << "Enter username: ";

        // Custom input that prevents empty Enter spam
        u.username = "";
        char c;
        while (true)
        {
            c = _getch();
            if (c == '\r' || c == '\n')
            {
                if (!u.username.empty())
                {
                    cout << "\n";
                    break;
                }
                // Ignore empty Enter - don't move cursor or show error
            }
            else if (c == '\b' && !u.username.empty())
            {
                u.username.pop_back();
                cout << "\b \b";
            }
            else if (c >= 32 && c <= 126)
            {
                u.username += c;
                cout << c;
            }
        }

        if (u.username == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to manage users...";
            Sleep(1000);
            return;
        }

        if (u.username.length() < 4 || u.username.length() > 15)
        {
            cout << string(15, ' ') << "Username must be between 4 and 15 characters.\n";
            cout << string(15, ' ') << "Please try again.\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Add New Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";
            continue;
        }

        if (!regex_match(u.username, usernameRegex))
        {
            cout << string(15, ' ') << "Username can only contain letters, numbers, and underscores.\n";
            cout << string(15, ' ') << "Please try again.\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Add New Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";
            continue;
        }

        if (!regex_match(u.username, hasLetter))
        {
            cout << string(15, ' ') << "Username must contain at least one letter.\n";
            cout << string(15, ' ') << "Please try again.\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Add New Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";
            continue;
        }

        if (users.find(u.username) != users.end())
        {
            cout << string(15, ' ') << "Username already exists!\n";
            cout << string(15, ' ') << "Please try again.\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Add New Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";
            continue;
        }

        break;
    }

    // Password validation regex patterns
    regex minLength(".{8,}");
    regex hasUpper(".*[A-Z].*");
    regex hasLower(".*[a-z].*");
    regex hasDigit(".*[0-9].*");
    regex hasSpecial(".*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?].*");

    while (true)
    {
        cout << string(15, ' ') << "Password: ";

        // Custom password input that prevents empty Enter spam
        u.password = "";
        char c;
        while (true)
        {
            c = _getch();
            if (c == '\r' || c == '\n')
            {
                if (!u.password.empty())
                {
                    cout << "\n";
                    break;
                }
                // Ignore empty Enter - don't move cursor
            }
            else if (c == '\b' && !u.password.empty())
            {
                u.password.pop_back();
                cout << "\b \b";
            }
            else if (c >= 32 && c <= 126)
            {
                u.password += c;
                cout << "\u2022"; // Bullet character for masking
            }
        }

        if (u.password == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to manage users...";
            Sleep(1000);
            return;
        }

        // Validate password
        bool valid = true;
        string errorMsg = "";

        if (!regex_match(u.password, minLength))
        {
            errorMsg += string(15, ' ') + "- At least 8 characters\n";
            valid = false;
        }
        if (!regex_match(u.password, hasUpper))
        {
            errorMsg += string(15, ' ') + "- At least one uppercase letter\n";
            valid = false;
        }
        if (!regex_match(u.password, hasLower))
        {
            errorMsg += string(15, ' ') + "- At least one lowercase letter\n";
            valid = false;
        }
        if (!regex_match(u.password, hasDigit))
        {
            errorMsg += string(15, ' ') + "- At least one digit\n";
            valid = false;
        }
        if (!regex_match(u.password, hasSpecial))
        {
            errorMsg += string(15, ' ') + "- At least one special character (!@#$%^&*...)\n";
            valid = false;
        }

        if (!valid)
        {
            cout << string(15, ' ') << "Password does not meet requirements:\n";
            cout << errorMsg;
            cout << string(15, ' ') << "Please try again.\n";
            continue;
        }

        break;
    }

    u.role = config.adminRoleLabel;

    // Add email field with validation loop
    while (true)
    {
        cout << string(15, ' ') << "Email: ";

        // Custom email input that prevents empty Enter spam
        u.email = "";
        char c;
        while (true)
        {
            c = _getch();
            if (c == '\r' || c == '\n')
            {
                if (!u.email.empty())
                {
                    cout << "\n";
                    break;
                }
                // Ignore empty Enter - don't move cursor
            }
            else if (c == '\b' && !u.email.empty())
            {
                u.email.pop_back();
                cout << "\b \b";
            }
            else if (c >= 32 && c <= 126)
            {
                u.email += c;
                cout << c;
            }
        }

        if (u.email == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to manage users...";
            Sleep(1000);
            return;
        }

        if (!isAllowedEmail(u.email))
        {
            cout << string(15, ' ') << "Invalid email address. Please try again.\n";
            continue;
        }

        // Check for duplicate email (including superadmin)
        bool emailExists = false;
        for (const auto &pair : users)
        {
            if (!pair.second.isClient)
            {
                string existingEmail = pair.second.isSuperAdmin ? superAdminEmail : pair.second.email;
                if (existingEmail == u.email)
                {
                    cout << string(15, ' ') << "This email is already registered to another user!\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    emailExists = true;
                    break;
                }
            }
        }

        if (emailExists)
        {
            continue;
        }

        break;
    }

    users[u.username] = u;
    saveUsers();
    logActivity("Add Admin", "Username " + u.username);

    cout << "\n"
         << string(15, ' ') << "Admin user added successfully!\n";
    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::editAdmin()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Edit Admin]");
    centerText(string(70, '-'));
    cout << "\n";
    centerText("Type '0' at any field to cancel the operation");
    cout << "\n";

    string username;
    auto it = users.end();

    while (it == users.end())
    {
        cout << string(15, ' ') << "Enter username: ";

        // Custom input that prevents empty Enter spam
        username = "";
        char c;
        while (true)
        {
            c = _getch();
            if (c == '\r' || c == '\n')
            {
                if (!username.empty())
                {
                    cout << "\n";
                    break;
                }
                // Ignore empty Enter - don't move cursor
            }
            else if (c == '\b' && !username.empty())
            {
                username.pop_back();
                cout << "\b \b";
            }
            else if (c >= 32 && c <= 126)
            {
                username += c;
                cout << c;
            }
        }

        // Check for cancellation
        if (username == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to manage users...";
            Sleep(1000);
            return;
        }

        it = users.find(username);
        if (it == users.end())
        {
            cout << "\n"
                 << string(15, ' ') << "Admin not found! Please try again.\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;

            // Clear the screen and redisplay the header
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Edit Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            centerText("Type '0' at any field to cancel the operation");
            cout << "\n";
        }
    }

    if (it->second.isClient)
    {
        cout << "\n"
             << string(15, ' ') << "Cannot edit client accounts from here!\n";
        cout << string(15, ' ') << "Press Enter to continue...";
        while (_getch() != '\r')
            ;
        return;
    }

    // Prevent super admin from editing themselves
    if (it->second.username == currentUser.username && it->second.isSuperAdmin)
    {
        cout << "\n"
             << string(15, ' ') << "Use 'My Super Admin Account' to edit your own account!\n";
        cout << string(15, ' ') << "Press Enter to continue...";
        while (_getch() != '\r')
            ;
        return;
    }

    cout << "\n"
         << string(15, ' ') << "Current Information:\n";
    cout << string(15, ' ') << "Username: " << it->second.username << "\n";
    cout << string(15, ' ') << "Role: " << it->second.role << "\n";
    cout << string(15, ' ') << "Email: " << it->second.email << "\n";

    // Main edit loop - allows multiple edits before returning
    while (true)
    {
        int choice = 0;
        string newValue;
        string oldUsername = it->second.username;

        // Loop for choice input with retry option
        while (true)
        {
            cout << "\n"
                 << string(15, ' ') << "What would you like to edit?\n";
            cout << string(15, ' ') << "[1] Username\n";
            cout << string(15, ' ') << "[2] Role\n";
            cout << string(15, ' ') << "[3] Email\n";
            cout << string(15, ' ') << "[4] Password\n";
            cout << "\n";
            cout << string(15, ' ') << "Enter your choice: ";

            string choiceStr = "";
            char ch;
            while (true)
            {
                ch = _getch();
                if (ch == '\r' || ch == '\n')
                {
                    if (!choiceStr.empty())
                    {
                        cout << "\n";
                        break;
                    }
                    continue;
                }
                if (ch == '\b' && !choiceStr.empty())
                {
                    choiceStr.pop_back();
                    cout << "\b \b";
                }
                else if (isdigit(ch))
                {
                    choiceStr += ch;
                    cout << ch;
                }
            }

            choice = stoi(choiceStr);

            // Validate choice
            if (choice == 0)
            {
                cout << "\n"
                     << string(15, ' ') << "Operation cancelled.\n";
                cout << string(15, ' ') << "Returning to manage users...";
                Sleep(1000);
                return;
            }
            else if (choice >= 1 && choice <= 4)
            {
                break; // Valid choice, exit loop
            }
            else
            {
                cout << string(15, ' ') << "Invalid choice! Please enter a number between 1-4.\n";
                cout << string(15, ' ') << "Press Enter to try again...";
                while (_getch() != '\r')
                    ;

                // Clear screen and redisplay
                clearScreen();
                displayHeader();
                cout << "\n";
                centerText("[Edit Admin]");
                centerText(string(70, '-'));
                cout << "\n";
                centerText("Type '0' at any field to cancel the operation");
                cout << "\n\n";
                cout << string(15, ' ') << "Current Information:\n";
                cout << string(15, ' ') << "Username: " << it->second.username << "\n";
                cout << string(15, ' ') << "Role: " << it->second.role << "\n";
                cout << string(15, ' ') << "Email: " << it->second.email << "\n";
            }
        }

        // Validation regex patterns
        regex usernameRegex("^[a-zA-Z0-9_]{4,15}$");
        regex hasLetter(".*[a-zA-Z].*");
        regex minLength(".{8,}");
        regex hasUpper(".*[A-Z].*");
        regex hasLower(".*[a-z].*");
        regex hasDigit(".*[0-9].*");
        regex hasSpecial(".*[!@#$%^&*()_+\\-=\\[\\]{};':\"\\\\|,.<>\\/?].*");

        switch (choice)
        {
        case 1: // Edit Username
        {
            bool cancelled = false;
            while (true)
            {
                cout << string(15, ' ') << "Enter new username (or 'c' to cancel): ";

                newValue = "";
                char c;
                while (true)
                {
                    c = _getch();
                    if (c == '\r' || c == '\n')
                    {
                        if (!newValue.empty())
                        {
                            cout << "\n";
                            break;
                        }
                        continue;
                    }
                    else if (c == '\b' && !newValue.empty())
                    {
                        newValue.pop_back();
                        cout << "\b \b";
                    }
                    else if (c >= 32 && c <= 126)
                    {
                        newValue += c;
                        cout << c;
                    }
                }

                if (newValue == "0")
                {
                    cout << "\n"
                         << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to manage users...";
                    Sleep(1000);
                    return;
                }

                // Check for cancel
                if (newValue == "c" || newValue == "C")
                {
                    cancelled = true;
                    break;
                }

                if (newValue.length() < 4 || newValue.length() > 15)
                {
                    cout << string(15, ' ') << "Username must be between 4 and 15 characters.\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }

                if (!regex_match(newValue, usernameRegex))
                {
                    cout << string(15, ' ') << "Username can only contain letters, numbers, and underscores.\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }

                if (!regex_match(newValue, hasLetter))
                {
                    cout << string(15, ' ') << "Username must contain at least one letter.\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }

                if (newValue == oldUsername)
                {
                    cout << string(15, ' ') << "Username is already '" << oldUsername << "'. No changes made.\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }

                if (users.find(newValue) != users.end())
                {
                    cout << string(15, ' ') << "Username already exists. Please try again.\n";
                    continue;
                }

                break;
            }

            if (cancelled)
            {
                cout << "\n"
                     << string(15, ' ') << "Username edit cancelled.\n";

                // Auto clear and redisplay without pressing Enter
                Sleep(800); // Brief pause to show the message
                clearScreen();
                displayHeader();
                cout << "\n";
                centerText("[Edit Admin]");
                centerText(string(70, '-'));
                cout << "\n";
                centerText("Type '0' at any field to cancel the operation");
                cout << "\n\n";
                cout << string(15, ' ') << "Current Information:\n";
                cout << string(15, ' ') << "Username: " << it->second.username << "\n";
                cout << string(15, ' ') << "Role: " << it->second.role << "\n";
                cout << string(15, ' ') << "Email: " << it->second.email << "\n";
                break;
            }

            User tempUser = it->second;
            tempUser.username = newValue;
            users.erase(it);
            users[newValue] = tempUser;
            it = users.find(newValue); // Update iterator to new username
            oldUsername = newValue;    // Update old username for future edits
            cout << string(15, ' ') << "Username updated successfully!\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;

            // Clear and redisplay with updated info
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Edit Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            centerText("Type '0' at any field to cancel the operation");
            cout << "\n\n";
            cout << string(15, ' ') << "Current Information:\n";
            cout << string(15, ' ') << "Username: " << it->second.username << "\n";
            cout << string(15, ' ') << "Role: " << it->second.role << "\n";
            cout << string(15, ' ') << "Email: " << it->second.email << "\n";
            break;
        }

        case 2: // Edit Role
        {
            string currentRole = it->second.role;
            bool cancelled = false;

            while (true)
            {
                cout << string(15, ' ') << "Enter new role (or 'c' to cancel): ";

                newValue = "";
                char c;
                while (true)
                {
                    c = _getch();
                    if (c == '\r' || c == '\n')
                    {
                        if (!newValue.empty())
                        {
                            cout << "\n";
                            break;
                        }
                        continue;
                    }
                    else if (c == '\b' && !newValue.empty())
                    {
                        newValue.pop_back();
                        cout << "\b \b";
                    }
                    else if (c >= 32 && c <= 126)
                    {
                        newValue += c;
                        cout << c;
                    }
                }

                if (newValue == "0")
                {
                    cout << "\n"
                         << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to manage users...";
                    Sleep(1000);
                    return;
                }

                // Check for cancel
                if (newValue == "c" || newValue == "C")
                {
                    cancelled = true;
                    break;
                }

                string lowerRole = newValue;
                transform(lowerRole.begin(), lowerRole.end(), lowerRole.begin(), ::tolower);

                string adminLabel = config.adminRoleLabel;
                string superLabel = config.superAdminRoleLabel;
                string adminLabelLower = adminLabel;
                string superLabelLower = superLabel;
                transform(adminLabelLower.begin(), adminLabelLower.end(), adminLabelLower.begin(), ::tolower);
                transform(superLabelLower.begin(), superLabelLower.end(), superLabelLower.begin(), ::tolower);

                if (lowerRole == "admin" || lowerRole == "administrator" || lowerRole == adminLabelLower)
                {
                    string newRole = adminLabel;

                    if (currentRole == newRole)
                    {
                        cout << string(15, ' ') << "User already has the role '" << adminLabel << "'. No changes made.\n";
                        cout << string(15, ' ') << "Please try again.\n";
                        continue;
                    }
                    else
                    {
                        it->second.role = newRole;
                        it->second.isSuperAdmin = false;
                        cout << string(15, ' ') << "Role updated successfully!\n";
                    }
                    break;
                }
                else if (lowerRole == "super administrator" || lowerRole == "super admin" ||
                         lowerRole == "superadmin" || lowerRole == superLabelLower)
                {
                    string newRole = superLabel;

                    if (currentRole == newRole)
                    {
                        cout << string(15, ' ') << "User already has the role '" << superLabel << "'. No changes made.\n";
                        cout << string(15, ' ') << "Please try again.\n";
                        continue;
                    }
                    else
                    {
                        it->second.role = newRole;
                        it->second.isSuperAdmin = true;
                        cout << string(15, ' ') << "Role updated successfully!\n";
                    }
                    break;
                }
                else
                {
                    cout << string(15, ' ') << "Invalid role! Use 'Admin' or '" << adminLabel << "', "
                         << "'Super Admin' or '" << superLabel << "'.\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }
            }

            if (cancelled)
            {
                cout << "\n"
                     << string(15, ' ') << "Role edit cancelled.\n";

                // Auto clear and redisplay without pressing Enter
                Sleep(800); // Brief pause to show the message
            }
            else
            {
                cout << string(15, ' ') << "Press Enter to continue...";
                while (_getch() != '\r')
                    ;
            }

            // Clear and redisplay with updated info
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Edit Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            centerText("Type '0' at any field to cancel the operation");
            cout << "\n\n";
            cout << string(15, ' ') << "Current Information:\n";
            cout << string(15, ' ') << "Username: " << it->second.username << "\n";
            cout << string(15, ' ') << "Role: " << it->second.role << "\n";
            cout << string(15, ' ') << "Email: " << it->second.email << "\n";
            break;
        }

        case 3: // Edit Email
        {
            string currentEmail = it->second.email;
            bool cancelled = false;

            while (true)
            {
                cout << string(15, ' ') << "Enter new email (or 'c' to cancel): ";

                newValue = "";
                char c;
                while (true)
                {
                    c = _getch();
                    if (c == '\r' || c == '\n')
                    {
                        if (!newValue.empty())
                        {
                            cout << "\n";
                            break;
                        }
                        continue;
                    }
                    else if (c == '\b' && !newValue.empty())
                    {
                        newValue.pop_back();
                        cout << "\b \b";
                    }
                    else if (c >= 32 && c <= 126)
                    {
                        newValue += c;
                        cout << c;
                    }
                }

                if (newValue == "0")
                {
                    cout << "\n"
                         << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to manage users...";
                    Sleep(1000);
                    return;
                }

                // Check for cancel
                if (newValue == "c" || newValue == "C")
                {
                    cancelled = true;
                    break;
                }

                if (!isAllowedEmail(newValue))
                {
                    cout << string(15, ' ') << "Invalid email format! Please try again.\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }

                if (newValue == currentEmail)
                {
                    cout << string(15, ' ') << "Email is already '" << currentEmail << "'. No changes made.\n";
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }

                bool emailExists = false;
                for (const auto &pair : users)
                {
                    if (!pair.second.isClient && pair.first != oldUsername)
                    {
                        string existingEmail = pair.second.isSuperAdmin ? superAdminEmail : pair.second.email;
                        if (existingEmail == newValue)
                        {
                            cout << string(15, ' ') << "This email is already registered to another user!\n";
                            cout << string(15, ' ') << "Please try again.\n";
                            emailExists = true;
                            break;
                        }
                    }
                }

                if (emailExists)
                {
                    continue;
                }

                break;
            }

            if (!cancelled)
            {
                it->second.email = newValue;
                cout << string(15, ' ') << "Email updated successfully!\n";
                cout << string(15, ' ') << "Press Enter to continue...";
                while (_getch() != '\r')
                    ;
            }
            else
            {
                cout << "\n"
                     << string(15, ' ') << "Email edit cancelled.\n";

                // Auto clear and redisplay without pressing Enter
                Sleep(800); // Brief pause to show the message
            }

            // Clear and redisplay with updated info
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Edit Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            centerText("Type '0' at any field to cancel the operation");
            cout << "\n\n";
            cout << string(15, ' ') << "Current Information:\n";
            cout << string(15, ' ') << "Username: " << it->second.username << "\n";
            cout << string(15, ' ') << "Role: " << it->second.role << "\n";
            cout << string(15, ' ') << "Email: " << it->second.email << "\n";
            break;
        }

        case 4: // Edit Password
        {
            bool cancelled = false;

            while (true)
            {
                cout << string(15, ' ') << "Enter new password (or 'c' to cancel): ";

                newValue = "";
                char c;
                while (true)
                {
                    c = _getch();
                    if (c == '\r' || c == '\n')
                    {
                        if (!newValue.empty())
                        {
                            cout << "\n";
                            break;
                        }
                        continue;
                    }
                    else if (c == '\b' && !newValue.empty())
                    {
                        newValue.pop_back();
                        cout << "\b \b";
                    }
                    else if (c >= 32 && c <= 126)
                    {
                        newValue += c;
                        cout << "\u2022"; // Bullet character for masking
                    }
                }

                if (newValue == "0")
                {
                    cout << "\n"
                         << string(15, ' ') << "Operation cancelled.\n";
                    cout << string(15, ' ') << "Returning to manage users...";
                    Sleep(1000);
                    return;
                }

                // Check for cancel (show it as bullets but check the actual value)
                if (newValue == "c" || newValue == "C")
                {
                    cancelled = true;
                    break;
                }

                bool valid = true;
                string errorMsg = "";

                if (!regex_match(newValue, minLength))
                {
                    errorMsg += string(15, ' ') + "- At least 8 characters\n";
                    valid = false;
                }
                if (!regex_match(newValue, hasUpper))
                {
                    errorMsg += string(15, ' ') + "- At least one uppercase letter\n";
                    valid = false;
                }
                if (!regex_match(newValue, hasLower))
                {
                    errorMsg += string(15, ' ') + "- At least one lowercase letter\n";
                    valid = false;
                }
                if (!regex_match(newValue, hasDigit))
                {
                    errorMsg += string(15, ' ') + "- At least one digit\n";
                    valid = false;
                }
                if (!regex_match(newValue, hasSpecial))
                {
                    errorMsg += string(15, ' ') + "- At least one special character (!@#$%^&*...)\n";
                    valid = false;
                }

                if (!valid)
                {
                    cout << string(15, ' ') << "Password does not meet requirements:\n";
                    cout << errorMsg;
                    cout << string(15, ' ') << "Please try again.\n";
                    continue;
                }

                break;
            }

            if (!cancelled)
            {
                it->second.password = newValue;
                cout << string(15, ' ') << "Password updated successfully!\n";
                cout << string(15, ' ') << "Press Enter to continue...";
                while (_getch() != '\r')
                    ;
            }
            else
            {
                cout << "\n"
                     << string(15, ' ') << "Password edit cancelled.\n";

                // Auto clear and redisplay without pressing Enter
                Sleep(800); // Brief pause to show the message
            }

            // Clear and redisplay with updated info
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Edit Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            centerText("Type '0' at any field to cancel the operation");
            cout << "\n\n";
            cout << string(15, ' ') << "Current Information:\n";
            cout << string(15, ' ') << "Username: " << it->second.username << "\n";
            cout << string(15, ' ') << "Role: " << it->second.role << "\n";
            cout << string(15, ' ') << "Email: " << it->second.email << "\n";
            break;
        }

        default:
            // This should never be reached due to validation loop above
            break;
        }

        // Save changes after each edit
        saveUsers();
        logActivity("Edit Admin", "Username " + it->second.username);
    }
}

void BankingSystem::deleteAdmin()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Delete Admin]");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    string username;
    auto it = users.end();

    while (it == users.end())
    {
        cout << string(15, ' ') << "Enter username: ";

        // Custom input that prevents empty Enter spam
        username = "";
        char c;
        while (true)
        {
            c = _getch();
            if (c == '\r' || c == '\n')
            {
                if (!username.empty())
                {
                    cout << "\n";
                    break;
                }
                // Ignore empty Enter - don't move cursor or show error
            }
            else if (c == '\b' && !username.empty())
            {
                username.pop_back();
                cout << "\b \b";
            }
            else if (c >= 32 && c <= 126)
            {
                username += c;
                cout << c;
            }
        }

        // Check for cancellation
        if (username == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to manage users...";
            Sleep(1000);
            return;
        }

        // Check if trying to delete current user
        if (username == currentUser.username)
        {
            cout << "\n"
                 << string(15, ' ') << "Cannot delete currently in user!\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;

            // Clear and redisplay
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Delete Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";
            continue;
        }

        // Find user
        it = users.find(username);

        // Check if user exists and is not a client
        if (it == users.end() || it->second.isClient)
        {
            cout << "\n"
                 << string(15, ' ') << "Admin not found! Please try again.\n";
            cout << string(15, ' ') << "Press Enter to continue...";
            while (_getch() != '\r')
                ;

            // Clear and redisplay
            clearScreen();
            displayHeader();
            cout << "\n";
            centerText("[Delete Admin]");
            centerText(string(70, '-'));
            cout << "\n";
            cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

            // Reset iterator to continue loop
            it = users.end();
            continue;
        }

        // User found, break the loop
        break;
    }

    // Display user info
    cout << "\n"
         << string(15, ' ') << "Admin: " << it->second.username << "\n";
    cout << string(15, ' ') << "Role: " << it->second.role << "\n";

    // Confirmation prompt
    char confirm;
    while (true)
    {
        cout << string(15, ' ') << "Are you sure you want to delete? (y/n): ";

        // Read single character input
        while (true)
        {
#ifdef _WIN32
            confirm = _getch();
#else
            system("stty raw -echo");
            confirm = getchar();
            system("stty cooked echo");
#endif

            // Only allow y, Y, n, or N
            if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N')
            {
                cout << confirm << endl;
                break;
            }
            // Silently ignore all other keys
        }

        if (confirm == 'y' || confirm == 'Y')
        {
            users.erase(username);
            saveUsers();
            logActivity("Delete Admin", "Username " + username);
            cout << "\n"
                 << string(15, ' ') << "Admin deleted successfully!\n";
            sleepMs(1500);
            return;
        }
        else if (confirm == 'n' || confirm == 'N')
        {
            cout << "\n"
                 << string(15, ' ') << "Deletion cancelled.\n";
            sleepMs(1500);
            return;
        }
    }
}

void BankingSystem::viewInbox()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Inbox]");
    centerText(string(70, '-'));
    cout << "\n";

    bool hasMessages = false;
    for (Message &m : messages)
    {
        if (m.to == currentUser.username)
        {
            hasMessages = true;
            cout << string(15, ' ') << (m.isRead ? "[READ] " : "[NEW] ");
            cout << "From: " << getEmailForUserId(m.from)
                 << " (" << getDisplayNameForUserId(m.from) << ")"
                 << " | " << m.date << "\n";
            cout << string(15, ' ') << "Message: " << m.content << "\n";
            cout << string(15, ' ') << string(50, '-') << "\n";
            m.isRead = true;
        }
    }

    if (!hasMessages)
    {
        centerText("No messages.");
    }
    else
    {
        saveMessages();
    }

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    cin.get();
}

void BankingSystem::clientMenu()
{
    while (isLoggedIn)
    {
        clearScreen();
        displayHeader();
        cout << "\n";
        centerText("Client Main Menu");
        centerText(string(70, '-'));

        // Display all options with aligned numbers
        cout << string(20, ' ') << "[1] View Account Information.\n";
        cout << string(20, ' ') << "[2] Deposit.\n";
        cout << string(20, ' ') << "[3] Withdraw.\n";
        cout << string(20, ' ') << "[4] Transfer.\n";
        cout << string(20, ' ') << "[5] View Transaction History.\n";
        cout << string(20, ' ') << "[6] Communication.\n";
        cout << string(20, ' ') << "[7] Currency Exchange.\n";
        cout << string(20, ' ') << "[8] Change PIN.\n";
        cout << string(20, ' ') << "[9] Logout.\n";

        centerText(string(70, '-'));
        cout << string(15, ' ') << "Enter Your Choice from 1--->9 : ";

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

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!choiceStr.empty()) // Only break if something was typed
                {
                    cout << endl;
                    break;
                }
                // If empty, ignore Enter and continue waiting for input
                continue;
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
            // Allow all digits (0-9)
            else if (ch >= '0' && ch <= '9')
            {
                choiceStr += ch;
                cout << ch;
            }
            // Silently ignore all other keys (letters, symbols, special characters)
        }

        // Convert to integer (no need to check if empty since we require input now)
        int choice = stoi(choiceStr);

        switch (choice)
        {
        case 1:
            viewAccountInfo();
            break;
        case 2:
            deposit();
            break;
        case 3:
            withdraw();
            break;
        case 4:
            transfer();
            break;
        case 5:
            viewTransactionHistory();
            break;
        case 6:
            communication();
            break;
        case 7:
            currencyExchange();
            break;
        case 8:
            changeClientPin();
            break;
        case 9:
            cout << "\n";
            if (confirmLogout())
            {
                centerText("Logging out...");
                logActivity("Logout", "Client logged out");
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
