#include "../include/BankEase.h"

void BankingSystem::showClientList()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[1] Show Client List");

    if (clients.empty())
    {
        centerText(string(105, '-'));
        cout << "\n";
        centerText("No clients in the system.");
    }
    else
    {
        // Top border
        cout << string(106, '-') << "\n";

        // Header row
        cout << "| " << left << setw(13) << "Account No."
             << "| " << setw(30) << "Client Name"
             << "| " << setw(14) << "Phone"
             << "| " << setw(28) << "Email"
             << "| " << setw(10) << "Balance" << "|\n";

        // Separator after header
        cout << string(106, '-') << "\n";

        // Data rows
        for (const auto &pair : clients)
        {
            const Client &c = pair.second;

            // Truncate long strings to fit column width
            string name = formatFullName(c, true); // true = last name first
            if (name.length() > 30)
                name = name.substr(0, 30);
            string phone = c.phone.length() > 14 ? c.phone.substr(0, 14) : c.phone;
            string email = c.email.length() > 28 ? c.email.substr(0, 28) : c.email;

            // Format balance with dollar sign
            ostringstream balanceStream;
            balanceStream << "$" << fixed << setprecision(2) << c.balance;
            string balanceStr = balanceStream.str();

            cout << "| " << left << setw(13) << c.accountNumber
                 << "| " << setw(30) << name
                 << "| " << setw(14) << phone
                 << "| " << setw(28) << email
                 << "| " << left << setw(10) << balanceStr << "|\n";
        }

        // Bottom border
        cout << string(106, '-') << "\n";
    }

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::addNewClient()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[2] Add New Client");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    Client c;

    // Generate account number first
    c.accountNumber = generateAccountNumber();
    cout << string(15, ' ') << "Generated Account Number: " << c.accountNumber << "\n\n";

    // ========= LAST NAME =========
    string lastName;
    while (true)
    {
        cout << string(15, ' ') << "Last Name: ";
        lastName = "";
        char ch;
        bool hasLetter = false;

        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!lastName.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter
                continue;
            }

            // Backspace
            if (ch == '\b' && !lastName.empty())
            {
                char removed = lastName.back();
                if ((removed >= 'a' && removed <= 'z') || (removed >= 'A' && removed <= 'Z'))
                {
                    // Recalculate hasLetter
                    hasLetter = false;
                    for (char c : lastName)
                    {
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                        {
                            hasLetter = true;
                            break;
                        }
                    }
                }
                lastName.pop_back();
                cout << "\b \b";
            }
            // Allow letters, spaces, and '0'
            else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ' || ch == '0')
            {
                if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                {
                    hasLetter = true;
                }
                lastName += ch;
                cout << ch;
            }
        }

        lastName = trim(lastName);

        if (lastName == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        // Validate: must contain at least one letter
        hasLetter = false;
        for (char c : lastName)
        {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                hasLetter = true;
                break;
            }
        }

        if (!hasLetter)
        {
            cout << string(15, ' ') << "Last Name must contain at least one letter.\n";
            continue;
        }

        break;
    }
    c.lastName = lastName;

    // ========= FIRST NAME =========
    string firstName;
    while (true)
    {
        cout << string(15, ' ') << "First Name: ";
        firstName = "";
        char ch;
        bool hasLetter = false;

        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!firstName.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter
                continue;
            }

            // Backspace
            if (ch == '\b' && !firstName.empty())
            {
                char removed = firstName.back();
                if ((removed >= 'a' && removed <= 'z') || (removed >= 'A' && removed <= 'Z'))
                {
                    // Recalculate hasLetter
                    hasLetter = false;
                    for (char c : firstName)
                    {
                        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
                        {
                            hasLetter = true;
                            break;
                        }
                    }
                }
                firstName.pop_back();
                cout << "\b \b";
            }
            // Allow letters, spaces, and '0'
            else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ' || ch == '0')
            {
                if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                {
                    hasLetter = true;
                }
                firstName += ch;
                cout << ch;
            }
        }

        firstName = trim(firstName);

        if (firstName == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        // Validate: must contain at least one letter
        hasLetter = false;
        for (char c : firstName)
        {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                hasLetter = true;
                break;
            }
        }

        if (!hasLetter)
        {
            cout << string(15, ' ') << "First Name must contain at least one letter.\n";
            continue;
        }

        break;
    }
    c.firstName = firstName;

    // ========= MIDDLE NAME (OPTIONAL) =========
    while (true)
    {
        cout << string(15, ' ') << "Middle Name (press Enter to skip): ";
        string middleName = "";
        char ch;

        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                cout << "\n";
                break;
            }
            if (ch == '\b' && !middleName.empty())
            {
                middleName.pop_back();
                cout << "\b \b";
            }
            // Allow letters, spaces, and '0'
            else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ' || ch == '0')
            {
                middleName += ch;
                cout << ch;
            }
        }

        middleName = trim(middleName);

        // Check for cancellation BEFORE checking if empty
        if (middleName == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        // If empty, it's optional - allow it
        if (middleName.empty())
        {
            c.middleName = "";
            break;
        }

        // If not empty, validate: must contain at least one letter
        bool hasLetter = false;
        for (char c : middleName)
        {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            {
                hasLetter = true;
                break;
            }
        }

        if (!hasLetter)
        {
            cout << string(15, ' ') << "Middle Name must contain at least one letter or leave empty.\n";
            continue;
        }

        c.middleName = middleName;
        break;
    }

    // ========= SUFFIX (OPTIONAL) =========
    while (true)
    {
        cout << string(15, ' ') << "Suffix (e.g., Jr., Sr., III - press Enter to skip): ";
        string suffix = "";
        char ch;

        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                cout << "\n";
                break;
            }
            if (ch == '\b' && !suffix.empty())
            {
                suffix.pop_back();
                cout << "\b \b";
            }
            else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                     ch == '.' || ch == ',' || ch == ' ' || (ch >= '0' && ch <= '9'))
            {
                suffix += ch;
                cout << ch;
            }
        }

        suffix = trim(suffix);

        // Check for cancellation
        if (suffix == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        // Suffix is optional - allow empty
        if (suffix.empty())
        {
            c.suffix = "";
            break;
        }

        // Validate suffix format using regex
        // Comprehensive pattern for all common name suffixes
        regex suffixPattern(
            "^("
            // Junior/Senior variants
            "Jr\\.?|JR\\.?|Junior|JUNIOR|"
            "Sr\\.?|SR\\.?|Senior|SENIOR|"

            // Roman numerals I through XX
            "I{1,3}|IV|V|VI{0,3}|IX|X|XI{0,3}|XIV|XV|XVI{0,3}|XIX|XX|"

            // Ordinal numbers (1st through 99th)
            "([1-9]|[1-9][0-9])(st|nd|rd|th)|"
            "([1-9]|[1-9][0-9])(ST|ND|RD|TH)|"

            // Academic/Professional degrees
            "PhD\\.?|PHD\\.?|Ph\\.D\\.?|"
            "MD\\.?|M\\.D\\.?|"
            "DDS\\.?|D\\.D\\.S\\.?|"
            "DMD\\.?|D\\.M\\.D\\.?|"
            "DO\\.?|D\\.O\\.?|"
            "DVM\\.?|D\\.V\\.M\\.?|"
            "JD\\.?|J\\.D\\.?|"
            "LLD\\.?|LL\\.D\\.?|"
            "EdD\\.?|Ed\\.D\\.?|"
            "DPhil\\.?|D\\.Phil\\.?|"
            "MBA\\.?|M\\.B\\.A\\.?|"
            "MA\\.?|M\\.A\\.?|"
            "MS\\.?|M\\.S\\.?|"
            "MSc\\.?|M\\.Sc\\.?|"
            "BS\\.?|B\\.S\\.?|"
            "BA\\.?|B\\.A\\.?|"
            "BSc\\.?|B\\.Sc\\.?|"

            // Religious titles
            "Esq\\.?|ESQ\\.?|Esquire|ESQUIRE|"

            // Military/Honorary
            "Ret\\.?|RET\\.?|Retired|RETIRED|"
            "USA\\.?|USAF\\.?|USN\\.?|USMC\\.?|USCG\\.?|"

            // British honours
            "KBE\\.?|OBE\\.?|MBE\\.?|CBE\\.?|DBE\\.?|GBE\\.?|"
            "VC\\.?|GC\\.?|DSO\\.?|MC\\.?|DFC\\.?|"

            // Clergy
            "CSC\\.?|SJ\\.?|OP\\.?|OSB\\.?|OFM\\.?"
            ")$",
            regex_constants::icase);

        if (!regex_match(suffix, suffixPattern))
        {
            cout << string(15, ' ') << "Invalid suffix format.\n";
            cout << string(15, ' ') << "Examples: Jr, Sr, III, PhD, MD, Esq, 2nd, etc.\n";
            continue;
        }

        c.suffix = suffix;
        break;
    }

    // ========= PHONE (NUMBERS ONLY + EXACTLY 11 DIGITS) =========
    string phone;
    while (true)
    {
        cout << string(15, ' ') << "Phone: ";
        phone = "";
        char ch;

        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!phone.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter
                continue;
            }

            // Backspace
            if (ch == '\b' && !phone.empty())
            {
                phone.pop_back();
                cout << "\b \b";
            }
            // Only allow digits, max 11
            else if (ch >= '0' && ch <= '9' && phone.length() < 11)
            {
                phone += ch;
                cout << ch;
            }
        }

        if (phone == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        if (phone.length() != 11)
        {
            cout << string(15, ' ') << "Phone must be exactly 11 digits.\n";
            continue;
        }

        c.phone = phone;
        break;
    }

    // ========= EMAIL =========
    while (true)
    {
        cout << string(15, ' ') << "Email: ";
        string email = "";
        char ch;

        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!email.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter
                continue;
            }

            // Backspace
            if (ch == '\b' && !email.empty())
            {
                email.pop_back();
                cout << "\b \b";
            }
            // Allow printable characters for email
            else if (ch >= 32 && ch <= 126)
            {
                email += ch;
                cout << ch;
            }
        }

        email = trim(email);

        if (email == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        // Validate email format
        if (!isAllowedEmail(email))
        {
            cout << string(15, ' ') << "Invalid email address. Please try again.\n";
            continue;
        }

        c.email = email;
        break;
    }

    // ========= INITIAL BALANCE (NUMERIC, NO EMPTY ENTER) =========
    while (true)
    {
        cout << string(15, ' ') << "Initial Balance: $";

        string balanceInput = "";
        char ch;
        bool hasDecimal = false;

        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!balanceInput.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter
                continue;
            }

            // Backspace
            if (ch == '\b' && !balanceInput.empty())
            {
                if (balanceInput.back() == '.')
                {
                    hasDecimal = false;
                }
                balanceInput.pop_back();
                cout << "\b \b";
            }
            // Only allow digits and one decimal point
            else if (ch >= '0' && ch <= '9')
            {
                balanceInput += ch;
                cout << ch;
            }
            else if (ch == '.' && !hasDecimal && !balanceInput.empty())
            {
                hasDecimal = true;
                balanceInput += ch;
                cout << ch;
            }
        }

        if (balanceInput == "0" || balanceInput == "0.0" || balanceInput == "0.00")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        // Convert to double
        try
        {
            c.balance = stod(balanceInput);

            if (c.balance < 0)
            {
                cout << string(15, ' ') << "Amount cannot be negative. Try again.\n";
                continue;
            }

            break;
        }
        catch (...)
        {
            cout << string(15, ' ') << "Invalid amount. Please enter a number.\n";
            continue;
        }
    }

    // ========= PIN CODE (HIDDEN, REQUIRED, NO EMPTY ENTER) =========
    while (true)
    {
        cout << string(15, ' ') << "Pin Code: ";
        c.pinCode = "";
        char ch;

        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r' || ch == '\n')
            {
                if (!c.pinCode.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter
                continue;
            }

            // Backspace
            if (ch == '\b' || ch == 127)
            {
                if (!c.pinCode.empty())
                {
                    c.pinCode.pop_back();
                    cout << "\b \b";
                }
                continue;
            }

            // Only accept digits and limit to 6 characters
            if (isdigit(ch) && c.pinCode.length() < 6)
            {
                c.pinCode += ch;
                cout << "\u2022"; // Display bullet symbol •
            }
        }

        if (c.pinCode == "0" || c.pinCode == "000000")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            lastAccountNumber--;
            return;
        }

        // Check if PIN is exactly 6 digits
        if (c.pinCode.length() != 6)
        {
            cout << string(15, ' ') << "PIN Code must be exactly 6 digits. Please try again.\n";
            continue;
        }

        break;
    }

    // ========= SAVE CLIENT =========
    clients[c.accountNumber] = c;
    saveClients();
    saveLastAccountNumber();

    // Add email to client user
    User clientUser;
    clientUser.username = c.accountNumber;
    clientUser.password = c.pinCode;
    clientUser.role = "Client";
    clientUser.isClient = true;
    clientUser.isSuperAdmin = false;
    clientUser.email = c.email;
    users[c.accountNumber] = clientUser;
    saveUsers();

    addTransaction(c.accountNumber, "Initial Deposit", c.balance);
    logActivity("Add Client", "Account " + c.accountNumber);

    {
        string subject = "Your BankEase Account Credentials";
        string senderName = getDisplayNameForUserId(currentUser.username);
        string fromEmail = getEmailForUserId(currentUser.username);

        stringstream emailBody;
        emailBody << "BankEase Account Setup\n\n";
        emailBody << "Hello " << formatFullName(c, false) << ",\n\n";
        emailBody << "Your account has been created. Please keep these details secure.\n\n";
        emailBody << "Account Number: " << c.accountNumber << "\n";
        emailBody << "PIN Code      : " << c.pinCode << "\n\n";
        emailBody << "Login Type    : Client\n";
        emailBody << "Date Created  : " << getCurrentDateTime() << "\n\n";
        emailBody << "For security, change your PIN after first login.\n";
        emailBody << "If you did not request this, contact support immediately.\n\n";
        emailBody << "BankEase Team\n";

        sendRealEmail(fromEmail, c.email, subject, emailBody.str(), "");
    }

    cout << "\n"
         << string(15, ' ') << "Client added successfully!\n";
    cout << string(15, ' ') << "Account Number: " << c.accountNumber << "\n";
    cout << string(15, ' ') << "Client can now login with Account Number and PIN.\n";
    cout << string(15, ' ') << "Returning to menu...";
    Sleep(1000);
}

void BankingSystem::deleteClient()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[3] Delete Client");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    // Account number input - only allow digits
    cout << string(15, ' ') << "Enter Account Number: ";
    string accNum = "";
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
            // Only break if accNum is not empty
            if (!accNum.empty())
            {
                cout << endl;
                break;
            }
            // Silently ignore Enter if accNum is empty (prevents spam)
        }
        // Backspace key
        else if (ch == 8 || ch == 127)
        {
            if (!accNum.empty())
            {
                accNum.pop_back();
                cout << "\b \b";
            }
        }
        // Only allow digits (0-9) and limit to 10 characters
        else if (ch >= '0' && ch <= '9')
        {
            if (accNum.length() < 10) // Maximum 10 digits
            {
                accNum += ch;
                cout << ch;
            }
            // Silently ignore if already 10 digits
        }
        // Silently ignore all other keys (letters, symbols, special characters)
    }

    // Trim whitespace
    accNum = trim(accNum);

    if (accNum == "0")
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cout << string(15, ' ') << "Returning to menu...";
        Sleep(1000);
        return;
    }

    auto it = clients.find(accNum);
    if (it != clients.end())
    {
        cout << "\n"
             << string(15, ' ') << "Client: " << formatFullName(it->second, true) << "\n";

        char confirm;
        cout << string(15, ' ') << "Are you sure you want to delete? (y/n): ";

        while (true)
        {
#ifdef _WIN32
            confirm = _getch(); // Windows
#else
            system("stty raw -echo"); // Linux/Mac
            confirm = getchar();
            system("stty cooked echo");
#endif

            // Only accept y, Y, n, N, or 0
            if (confirm == 'y' || confirm == 'Y' || confirm == 'n' || confirm == 'N' || confirm == '0')
            {
                cout << confirm << endl; // Echo the character
                break;
            }
        }

        // Allow 0 to cancel whole operation
        if (confirm == '0')
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            return;
        }

        if (confirm == 'y' || confirm == 'Y')
        {
            clients.erase(it);
            saveClients();

            if (users.find(accNum) != users.end())
            {
                users.erase(accNum);
                saveUsers();
            }

            logActivity("Delete Client", "Account " + accNum);
            cout << "\n"
                 << string(15, ' ') << "Client deleted successfully!\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
        }
        else
        {
            cout << "\n"
                 << string(15, ' ') << "Deletion cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
        }
    }
    else
    {
        cout << "\n"
             << string(15, ' ') << "Client not found!\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        cin.get();
    }
}

void BankingSystem::updateClientInfo()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[4] Update Client Info");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    // Account number (required, 0 = cancel, numbers only - blocks letters at keyboard level)
    string accNum = "";
    cout << string(15, ' ') << "Enter Account Number: ";

    char ch;
    while (true)
    {
#ifdef _WIN32
        ch = _getch(); // Windows - gets character without displaying it
#else
        system("stty raw -echo"); // Linux/Mac - raw mode
        ch = getchar();
        system("stty cooked echo"); // Restore normal mode
#endif

        // Enter key - only accept if input is not empty
        if (ch == '\r' || ch == '\n')
        {
            if (!accNum.empty())
            {
                cout << endl;
                break;
            }
            continue;
        }
        // Backspace key
        else if (ch == 8 || ch == 127)
        {
            if (!accNum.empty())
            {
                accNum.pop_back();
                cout << "\b \b"; // Erase character from screen
            }
        }
        // Only allow digits 0-9 to be typed
        else if (isdigit(ch))
        {
            accNum += ch;
            cout << ch; // Display only numbers
        }
        // All other keys (letters, symbols) are silently ignored - nothing happens
    }

    if (accNum == "0")
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cout << string(15, ' ') << "Returning to menu...";
        Sleep(1000);
        return;
    }

    auto it = clients.find(accNum);
    if (it != clients.end())
    {
        // Work on a copy first, only save when something changes
        Client original = it->second;
        Client updated = original;
        bool hasChanges = false;

        cout << "\n"
             << string(15, ' ') << "Current Info:\n";
        cout << string(15, ' ') << "Name: " << formatFullName(original, true) << "\n";
        cout << string(15, ' ') << "Phone: " << original.phone << "\n";
        cout << string(15, ' ') << "Email: " << original.email << "\n\n";

        string input;

        // ===== New First Name =====
        cout << string(15, ' ') << "New First Name (Enter to keep): ";
        input = "";
        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                cout << "\n";
                break;
            }
            if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
                continue;
            }
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ')
            {
                input += ch;
                cout << ch;
            }
        }
        input = trim(input);
        if (input == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            return;
        }
        if (!input.empty())
        {
            updated.firstName = input;
            hasChanges = true;
        }

        // ===== New Last Name =====
        cout << string(15, ' ') << "New Last Name (Enter to keep): ";
        input = "";
        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                cout << "\n";
                break;
            }
            if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
                continue;
            }
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ')
            {
                input += ch;
                cout << ch;
            }
        }
        input = trim(input);
        if (input == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            return;
        }
        if (!input.empty())
        {
            updated.lastName = input;
            hasChanges = true;
        }

        // ===== New Middle Name (Optional) =====
        cout << string(15, ' ') << "New Middle Name (Enter to keep): ";
        input = "";
        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                cout << "\n";
                break;
            }
            if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
                continue;
            }
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == ' ')
            {
                input += ch;
                cout << ch;
            }
        }
        input = trim(input);
        if (input == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            return;
        }
        if (!input.empty())
        {
            updated.middleName = input;
            hasChanges = true;
        }

        // ===== New Suffix (Optional) =====
        cout << string(15, ' ') << "New Suffix (Enter to keep): ";
        input = "";
        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                cout << "\n";
                break;
            }
            if (ch == '\b' && input.length() > 0)
            {
                input.pop_back();
                cout << "\b \b";
                continue;
            }
            if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
                ch == '.' || ch == ',' || ch == ' ' || (ch >= '0' && ch <= '9'))
            {
                input += ch;
                cout << ch;
            }
        }
        input = trim(input);
        if (input == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            return;
        }
        if (!input.empty())
        {
            updated.suffix = input;
            hasChanges = true;
        }

        // ===== New Email (with retry on invalid format) =====
        while (true)
        {
            cout << string(15, ' ') << "New Email (Enter to keep): ";
            getline(cin, input);
            input = trim(input);

            if (input.empty())
            {
                // Enter pressed without input - keep current email
                break;
            }

            if (input == "0")
            {
                cout << "\n"
                     << string(15, ' ') << "Operation cancelled.\n";
                cout << string(15, ' ') << "Returning to menu...";
                Sleep(1000);
                return;
            }

            if (isAllowedEmail(input))
            {
                updated.email = input;
                hasChanges = true;
                break;
            }
            else
            {
                cout << string(15, ' ')
                     << "Invalid email format! Please try again.\n";
            }
        }

        // ===== Change PIN (Optional) =====
        cout << "\n"
             << string(15, ' ') << "Change Client PIN? (y/n): ";

        string changePinChoice = "";
        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                if (!changePinChoice.empty())
                {
                    cout << "\n";
                    break;
                }
                continue;
            }
            if (ch == '\b' && changePinChoice.length() > 0)
            {
                changePinChoice.pop_back();
                cout << "\b \b";
                continue;
            }
            if (ch == 'y' || ch == 'Y' || ch == 'n' || ch == 'N' || ch == '0')
            {
                changePinChoice += ch;
                cout << ch;
            }
        }

        if (changePinChoice == "0")
        {
            cout << "\n"
                 << string(15, ' ') << "Operation cancelled.\n";
            cout << string(15, ' ') << "Returning to menu...";
            Sleep(1000);
            return;
        }

        if (changePinChoice == "y" || changePinChoice == "Y")
        {
            auto readPin = [](const string &label) -> string
            {
                cout << string(15, ' ') << label;
                string pin;
                char c;
                while (true)
                {
                    c = _getch();
                    if (c == '\r' || c == '\n')
                    {
                        if (!pin.empty())
                        {
                            cout << "\n";
                            return pin;
                        }
                        continue;
                    }
                    if (c == '\b' && pin.length() > 0)
                    {
                        pin.pop_back();
                        cout << "\b \b";
                        continue;
                    }
                    if (isdigit(c) && pin.length() < 6)
                    {
                        pin += c;
                        cout << "\u2022";
                    }
                }
            };

            string newPin = readPin("New PIN (6 digits): ");

            if (newPin == "0")
            {
                cout << string(15, ' ') << "PIN change cancelled.\n";
            }
            else if (newPin.length() != 6)
            {
                cout << string(15, ' ') << "PIN must be exactly 6 digits. PIN not changed.\n";
            }
            else
            {
                string confirmPin = readPin("Confirm New PIN: ");

                if (confirmPin == "0")
                {
                    cout << string(15, ' ') << "PIN change cancelled.\n";
                }
                else if (confirmPin != newPin)
                {
                    cout << string(15, ' ') << "PINs do not match. PIN not changed.\n";
                }
                else
                {
                    updated.pinCode = newPin;
                    hasChanges = true;

                    // Also update in users map if exists
                    if (users.find(accNum) != users.end())
                    {
                        users[accNum].password = newPin;
                        saveUsers();
                    }

                    cout << string(15, ' ') << "PIN changed successfully!\n";
                    logActivity("Change Client PIN", "Account " + accNum + " by Admin");
                }
            }
        }

        if (hasChanges)
        {
            it->second = updated;
            saveClients();
            logActivity("Update Client", "Account " + accNum);
            cout << "\n"
                 << string(15, ' ') << "Client information updated!\n";
        }
        else
        {
            cout << "\n"
                 << string(15, ' ') << "No changes made.\n";
        }
    }
    else
    {
        cout << "\n"
             << string(15, ' ') << "Client not found!\n";
    }

    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::changeClientPin()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Change PIN]");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' to cancel.\n\n";

    string accNum = currentUser.username;
    auto it = clients.find(accNum);
    if (it == clients.end())
    {
        cout << string(15, ' ') << "Client account not found.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    auto readPin = [](const string &label) -> string
    {
        cout << string(15, ' ') << label;
        string pin;
        char ch;
        while (true)
        {
            ch = _getch();
            if (ch == '\r' || ch == '\n')
            {
                if (!pin.empty())
                {
                    cout << "\n";
                    break;
                }
                continue;
            }
            if (ch == '\b' && !pin.empty())
            {
                pin.pop_back();
                cout << "\b \b";
                continue;
            }
            if (isdigit(ch) && pin.size() < 6)
            {
                pin.push_back(ch);
                cout << u8"\u2022";
            }
        }
        return trim(pin);
    };

    string currentPin = readPin("Current PIN: ");
    if (currentPin == "0")
        return;
    if (currentPin != it->second.pinCode)
    {
        cout << string(15, ' ') << "Incorrect PIN.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    string newPin = readPin("New PIN (6 digits): ");
    if (newPin == "0")
        return;
    if (newPin.size() != 6)
    {
        cout << string(15, ' ') << "PIN must be exactly 6 digits.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    string confirmPin = readPin("Confirm New PIN: ");
    if (confirmPin == "0")
        return;
    if (confirmPin != newPin)
    {
        cout << string(15, ' ') << "PINs do not match.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    it->second.pinCode = newPin;
    clients[accNum].pinCode = newPin;
    saveClients();

    if (users.find(accNum) != users.end())
    {
        users[accNum].password = newPin;
        saveUsers();
    }

    logActivity("Change PIN", "Account " + accNum);
    cout << string(15, ' ') << "PIN updated successfully.\n";
    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

void BankingSystem::findClient()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[5] Find Client");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    // Account number input - only allow digits
    cout << string(15, ' ') << "Enter Account Number: ";
    string accNum = "";
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
            // Only break if accNum is not empty
            if (!accNum.empty())
            {
                cout << endl;
                break;
            }
            // Silently ignore Enter if accNum is empty
        }
        // Backspace key
        else if (ch == 8 || ch == 127)
        {
            if (!accNum.empty())
            {
                accNum.pop_back();
                cout << "\b \b";
            }
        }
        // Only allow digits (0-9)
        else if (ch >= '0' && ch <= '9')
        {
            accNum += ch;
            cout << ch;
        }
        // Silently ignore all other keys (letters, symbols, special characters)
    }

    // Trim whitespace
    accNum = trim(accNum);

    if (accNum == "0")
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cout << string(15, ' ') << "Returning to menu...";
        Sleep(1000);
        return;
    }

    if (clients.find(accNum) != clients.end())
    {
        const Client &c = clients[accNum];
        cout << "\n"
             << string(15, ' ') << "Client Found:\n";
        cout << string(15, ' ') << string(50, '-') << "\n";
        cout << string(15, ' ') << "Account Number: " << c.accountNumber << "\n";
        cout << string(15, ' ') << "Client Name: " << formatFullName(c, true) << "\n";
        cout << string(15, ' ') << "Phone: " << c.phone << "\n";
        cout << string(15, ' ') << "Email: " << c.email << "\n";
        cout << string(15, ' ') << "Balance: $" << fixed << setprecision(2) << c.balance << "\n";
    }
    else
    {
        cout << "\n"
             << string(15, ' ') << "Client not found!\n";
    }

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    cin.get();
}

void BankingSystem::viewAccountInfo()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[My Account Information]");
    centerText(string(70, '-'));
    cout << "\n";

    const Client &c = clients[currentUser.username];
    cout << string(15, ' ') << "Account Number  : " << c.accountNumber << "\n";
    cout << string(15, ' ') << "Name            : " << formatFullName(c, true) << "\n";
    cout << string(15, ' ') << "Phone           : " << c.phone << "\n";
    cout << string(15, ' ') << "Email           : " << c.email << "\n";
    cout << string(15, ' ') << "Current Balance : $" << fixed << setprecision(2) << c.balance << "\n";

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}

// Unlock client accounts page
void BankingSystem::unlockClientAccounts()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Unlock Client Accounts]");
    centerText(string(70, '-'));
    cout << "\n";

    // List locked clients
    vector<string> lockedAccounts;
    for (const auto &pair : clients)
    {
        if (pair.second.locked)
            lockedAccounts.push_back(pair.first);
    }

    if (lockedAccounts.empty())
    {
        centerText("No locked client accounts.");
        cout << "\n"
             << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    cout << string(15, ' ') << "Locked Accounts:" << endl;
    for (const string &acc : lockedAccounts)
    {
        const Client &c = clients[acc];
        cout << string(15, ' ') << acc << " | " << formatFullName(c, true) << " | Email: " << c.email << endl;
    }
    cout << "\n"
         << string(15, ' ') << "Enter Account Number to unlock (or 0 to go back): ";
    string input;
    getline(cin, input);
    input = trim(input);
    if (input == "0" || input.empty())
        return;
    auto it = clients.find(input);
    if (it != clients.end() && it->second.locked)
    {
        it->second.locked = false;
        it->second.failedAttempts = 0;
        saveClients();
        cout << string(15, ' ') << "Account " << input << " unlocked successfully!\n";
    }
    else
    {
        cout << string(15, ' ') << "Invalid or not locked account.\n";
    }
    cout << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}
