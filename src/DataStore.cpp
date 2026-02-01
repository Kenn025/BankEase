#include "../include/BankEase.h"

string BankingSystem::generateAccountNumber()
{
    lastAccountNumber++;
    stringstream ss;
    ss << setfill('0') << setw(10) << lastAccountNumber;
    return ss.str();
}

string BankingSystem::dataPath(const string &filename)
{
    if (config.dataDir.empty())
    {
        return filename;
    }
    return config.dataDir + "/" + filename;
}

void BankingSystem::loadConfig()
{
    config.allowedDomains = "*";
    config.smtpHost = "smtp.gmail.com";
    config.smtpPort = 465;
    config.smtpUseSsl = true;
    config.smtpFromEmail = "";
    config.smtpPassword = "";
    config.adminRoleLabel = "Administrator";
    config.superAdminRoleLabel = "Super Administrator";
    config.dataDir = "data";

    ifstream file("data/config.txt");
    if (!file.is_open())
    {
        setupConfig();
        saveConfig();
        return;
    }

    string line;
    while (getline(file, line))
    {
        line = trim(line);
        if (line.empty() || line[0] == '#')
        {
            continue;
        }
        size_t pos = line.find('=');
        if (pos == string::npos)
        {
            continue;
        }
        string key = trim(line.substr(0, pos));
        string value = trim(line.substr(pos + 1));
        if (key == "allowed_domains")
            config.allowedDomains = value;
        else if (key == "smtp_host")
            config.smtpHost = value;
        else if (key == "smtp_port")
            config.smtpPort = atoi(value.c_str());
        else if (key == "smtp_ssl")
            config.smtpUseSsl = (value == "1" || value == "true" || value == "yes");
        else if (key == "smtp_from")
            config.smtpFromEmail = value;
        else if (key == "smtp_password")
            config.smtpPassword = decrypt(value);
        else if (key == "admin_role")
            config.adminRoleLabel = value;
        else if (key == "super_admin_role")
            config.superAdminRoleLabel = value;
        else if (key == "data_dir")
            config.dataDir = value;
    }
}

void BankingSystem::saveConfig()
{
    ofstream file("data/config.txt");
    file << "allowed_domains=" << config.allowedDomains << "\n";
    file << "smtp_host=" << config.smtpHost << "\n";
    file << "smtp_port=" << config.smtpPort << "\n";
    file << "smtp_ssl=" << (config.smtpUseSsl ? "true" : "false") << "\n";
    file << "smtp_from=" << config.smtpFromEmail << "\n";
    file << "smtp_password=" << encrypt(config.smtpPassword) << "\n";
    file << "admin_role=" << config.adminRoleLabel << "\n";
    file << "super_admin_role=" << config.superAdminRoleLabel << "\n";
    file << "data_dir=" << config.dataDir << "\n";
}

void BankingSystem::setupConfig()
{
    clearScreen();
    centerText("First-Time Setup");
    centerText(string(70, '-'));

    cout << string(15, ' ') << "Allowed email domains (* for any)\n";
    cout << string(15, ' ') << "Default: *\n";
    cout << string(15, ' ') << "Enter domains (comma-separated): ";
    string input;
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        config.allowedDomains = input;
    }

    cout << "\n";
    cout << string(15, ' ') << "SMTP host (default: smtp.gmail.com): ";
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        config.smtpHost = input;
    }

    cout << string(15, ' ') << "SMTP port (default: 465): ";
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        int port = atoi(input.c_str());
        if (port > 0)
        {
            config.smtpPort = port;
        }
    }

    cout << string(15, ' ') << "Use SSL? (y/n, default: y): ";
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        config.smtpUseSsl = (input == "y" || input == "Y");
    }

    cout << string(15, ' ') << "SMTP from email: ";
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        config.smtpFromEmail = input;
    }

    config.smtpPassword = getHiddenInput(string(15, ' ') + "SMTP app password: ");
    config.smtpPassword = trim(config.smtpPassword);

    cout << "\n";
    cout << string(15, ' ') << "Admin role label (default: Administrator): ";
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        config.adminRoleLabel = input;
    }

    cout << string(15, ' ') << "Super Admin role label (default: Super Administrator): ";
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        config.superAdminRoleLabel = input;
    }

    cout << string(15, ' ') << "Data directory (default: data): ";
    getline(cin, input);
    input = trim(input);
    if (!input.empty())
    {
        config.dataDir = input;
    }
}

bool BankingSystem::isAllowedEmail(const string &rawEmail)
{
    string email = trim(rawEmail);
    if (email.empty())
        return false;
    if (email.find(' ') != string::npos)
        return false;
    size_t atPos = email.find('@');
    if (atPos == string::npos || atPos == 0)
        return false;
    if (email.find('@', atPos + 1) != string::npos)
        return false;
    string domain = email.substr(atPos + 1);
    if (domain.empty())
        return false;

    string domains = trim(config.allowedDomains);
    if (domains.empty() || domains == "*")
        return true;

    string lowered = domains;
    transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
    string domainLower = domain;
    transform(domainLower.begin(), domainLower.end(), domainLower.begin(), ::tolower);

    stringstream ss(lowered);
    string item;
    while (getline(ss, item, ','))
    {
        item = trim(item);
        if (item == domainLower)
            return true;
    }
    return false;
}

void BankingSystem::loadSuperAdminAccount()
{
    ifstream file(dataPath("superadmin.txt"));
    if (file.is_open())
    {
        string line;
        getline(file, line);
        stringstream ss(line);
        vector<string> parts;
        string part;
        while (getline(ss, part, '|'))
        {
            parts.push_back(part);
        }

        if (parts.size() >= 4)
        {
            superAdminAccount.username = parts[0];
            superAdminAccount.password = decrypt(parts[1]);
            superAdminAccount.role = parts[2];
            superAdminAccount.email = parts[3];
            if (parts.size() >= 5)
            {
                superAdminAccessCodeCipher = parts[4];
                superAdminAccessCode = decrypt(parts[4]);
            }
        }
        else
        {
            superAdminAccount = User();
        }

        if (superAdminAccount.role.empty())
        {
            superAdminAccount.role = config.superAdminRoleLabel;
        }
        superAdminAccount.isClient = false;
        superAdminAccount.isSuperAdmin = true;
        superAdminEmail = superAdminAccount.email;
        hasSuperAdminAccount = !superAdminAccount.username.empty();
        file.close();
    }
    else
    {
        hasSuperAdminAccount = false;
        superAdminAccount = User();
        superAdminAccessCodeCipher = "";
        superAdminAccessCode = "";
        superAdminEmail = "";
    }
}

void BankingSystem::saveSuperAdminAccount()
{
    superAdminAccessCodeCipher = encrypt(superAdminAccessCode);
    ofstream file(dataPath("superadmin.txt"));
    file << superAdminAccount.username << "|"
         << encrypt(superAdminAccount.password) << "|"
         << superAdminAccount.role << "|"
         << superAdminAccount.email << "|"
         << superAdminAccessCodeCipher;
    file.close();
}

void BankingSystem::loadGmailAppPassword()
{
    if (!config.smtpPassword.empty())
    {
        gmailAppPassword = config.smtpPassword;
        return;
    }

    ifstream file(dataPath("gmail_app_password.txt"));
    if (file.is_open())
    {
        string encPassword;
        getline(file, encPassword);
        gmailAppPassword = decrypt(encPassword);
        config.smtpPassword = gmailAppPassword;
        saveConfig();
        file.close();
    }
    else
    {
        gmailAppPassword = "";
    }
}

void BankingSystem::saveGmailAppPassword()
{
    config.smtpPassword = gmailAppPassword;
    saveConfig();
}

void BankingSystem::loadData()
{
    // Load last account number
    ifstream accFile(dataPath("last_account.txt"));
    if (accFile.is_open())
    {
        accFile >> lastAccountNumber;
        accFile.close();
    }
    else
    {
        lastAccountNumber = 0;
    }

    // Load clients
    ifstream clientFile(dataPath("clients.txt"));
    if (clientFile.is_open())
    {
        string line;
        while (getline(clientFile, line))
        {
            stringstream ss(line);
            Client c;
            string encPass;
            getline(ss, c.accountNumber, '|');
            getline(ss, c.firstName, '|');
            getline(ss, c.lastName, '|');
            getline(ss, c.middleName, '|');
            getline(ss, c.suffix, '|');
            getline(ss, c.phone, '|');
            getline(ss, c.email, '|');
            ss >> c.balance;
            ss.ignore();
            getline(ss, encPass, '|');
            c.pinCode = decrypt(encPass);
            int lockedInt = 0;
            int failedAtt = 0;
            ss >> lockedInt;
            ss.ignore();
            ss >> failedAtt;
            c.locked = (lockedInt == 1);
            c.failedAttempts = failedAtt;
            clients[c.accountNumber] = c;
        }
        clientFile.close();
    }

    // Load users
    ifstream userFile(dataPath("users.txt"));
    if (userFile.is_open())
    {
        string line;
        while (getline(userFile, line))
        {
            stringstream ss(line);
            User u;
            string encPass;
            int isClientInt, isSuperAdminInt;
            getline(ss, u.username, '|');
            getline(ss, encPass, '|');
            getline(ss, u.role, '|');
            ss >> isClientInt;
            ss.ignore();
            ss >> isSuperAdminInt;
            ss.ignore();
            getline(ss, u.email); // Add this line to load email
            u.password = decrypt(encPass);
            u.isClient = (isClientInt == 1);
            u.isSuperAdmin = (isSuperAdminInt == 1);
            if (!u.isSuperAdmin)
            {
                users[u.username] = u;
            }
        }
        userFile.close();
    }
    else
    {
        // No default admin account; admins are created dynamically.
        users.clear();
        saveUsers();
    }

    if (hasSuperAdminAccount && !superAdminAccount.username.empty())
    {
        users[superAdminAccount.username] = superAdminAccount;
    }

    // Load transactions
    ifstream transFile(dataPath("transactions.txt"));
    if (transFile.is_open())
    {
        string line;
        while (getline(transFile, line))
        {
            stringstream ss(line);
            Transaction t;
            getline(ss, t.accountNumber, '|');
            getline(ss, t.type, '|');
            ss >> t.amount;
            ss.ignore();
            getline(ss, t.date, '|');
            ss >> t.balanceAfter;
            transactions.push_back(t);
        }
        transFile.close();
    }

    // Load messages
    ifstream msgFile(dataPath("messages.txt"));
    if (msgFile.is_open())
    {
        string line;
        while (getline(msgFile, line))
        {
            stringstream ss(line);
            Message m;
            getline(ss, m.from, '|');
            getline(ss, m.to, '|');
            getline(ss, m.content, '|');
            getline(ss, m.date, '|');
            ss >> m.isRead;
            messages.push_back(m);
        }
        msgFile.close();
    }
}

void BankingSystem::saveLastAccountNumber()
{
    ofstream file(dataPath("last_account.txt"));
    file << lastAccountNumber;
    file.close();
}

void BankingSystem::saveClients()
{
    ofstream file(dataPath("clients.txt"));
    for (const auto &pair : clients)
    {
        const Client &c = pair.second;
        file << c.accountNumber << "|"
             << c.firstName << "|"
             << c.lastName << "|"
             << c.middleName << "|"
             << c.suffix << "|"
             << c.phone << "|"
             << c.email << "|" << c.balance << "|"
             << encrypt(c.pinCode) << "|"
             << (c.locked ? 1 : 0) << "|"
             << c.failedAttempts << "\n";
    }
    file.close();
}

void BankingSystem::saveUsers()
{
    ofstream file(dataPath("users.txt"));
    for (const auto &pair : users)
    {
        const User &u = pair.second;
        if (u.isSuperAdmin)
        {
            continue;
        }
        file << u.username << "|" << encrypt(u.password) << "|"
             << u.role << "|" << (u.isClient ? 1 : 0) << "|"
             << (u.isSuperAdmin ? 1 : 0) << "|"
             << u.email << "\n";
    }
    file.close();
}

void BankingSystem::saveTransactions()
{
    ofstream file(dataPath("transactions.txt"));
    for (const Transaction &t : transactions)
    {
        file << t.accountNumber << "|" << t.type << "|" << t.amount << "|"
             << t.date << "|" << t.balanceAfter << "\n";
    }
    file.close();
}

void BankingSystem::saveMessages()
{
    ofstream file(dataPath("messages.txt"));
    for (const Message &m : messages)
    {
        file << m.from << "|" << m.to << "|" << m.content << "|"
             << m.date << "|" << m.isRead << "\n";
    }
    file.close();
}

void BankingSystem::loadCurrencies()
{
    ifstream file(dataPath("currencies.txt"));
    if (!file.is_open())
    {
        // Create default currencies if file doesn't exist
        Currency usd{"USD", "US Dollar", 1.0, "$", true};
        Currency eur{"EUR", "Euro", 0.85, "€", true};
        Currency gbp{"GBP", "British Pound", 0.73, "£", true};
        Currency jpy{"JPY", "Japanese Yen", 110.50, "¥", true};
        Currency php{"PHP", "Philippine Peso", 56.25, "₱", true};
        Currency cny{"CNY", "Chinese Yuan", 6.45, "¥", true};
        Currency aud{"AUD", "Australian Dollar", 1.35, "A$", true};
        Currency cad{"CAD", "Canadian Dollar", 1.25, "C$", true};

        currencies["USD"] = usd;
        currencies["EUR"] = eur;
        currencies["GBP"] = gbp;
        currencies["JPY"] = jpy;
        currencies["PHP"] = php;
        currencies["CNY"] = cny;
        currencies["AUD"] = aud;
        currencies["CAD"] = cad;

        saveCurrencies();
        return;
    }

    string line;
    while (getline(file, line))
    {
        line = trim(line);
        if (line.empty())
            continue;

        stringstream ss(line);
        Currency c;
        string active;

        getline(ss, c.code, '|');
        getline(ss, c.name, '|');
        ss >> c.rate;
        ss.ignore();
        getline(ss, c.symbol, '|');
        getline(ss, active);

        c.code = trim(c.code);
        c.name = trim(c.name);
        c.symbol = trim(c.symbol);
        c.isActive = (trim(active) == "1");

        if (!c.code.empty())
        {
            currencies[c.code] = c;
        }
    }
    file.close();
}

void BankingSystem::saveCurrencies()
{
    ofstream file(dataPath("currencies.txt"));
    for (const auto &pair : currencies)
    {
        const Currency &c = pair.second;
        file << c.code << "|" << c.name << "|" << c.rate << "|"
             << c.symbol << "|" << (c.isActive ? "1" : "0") << "\n";
    }
    file.close();
}

void BankingSystem::addTransaction(const string &accountNumber, const string &type, double amount)
{
    Transaction t;
    t.accountNumber = accountNumber;
    t.type = type;
    t.amount = amount;
    t.date = getCurrentDateTime();
    t.balanceAfter = clients[accountNumber].balance;
    transactions.push_back(t);
    saveTransactions();
}

bool BankingSystem::findUserByEmail(const string &email, string &userIdOut)
{
    // Check Super Admin first
    if (email == superAdminEmail)
    {
        for (const auto &pair : users)
        {
            if (pair.second.isSuperAdmin)
            {
                userIdOut = pair.first;
                return true;
            }
        }
    }

    // Check regular admins
    for (const auto &pair : users)
    {
        if (!pair.second.isClient && pair.second.email == email)
        {
            userIdOut = pair.first;
            return true;
        }
    }

    // Check clients
    for (const auto &pair : clients)
    {
        if (pair.second.email == email)
        {
            userIdOut = pair.first;
            return true;
        }
    }

    return false;
}

string BankingSystem::getEmailForUserId(const string &userId)
{
    // Check if it's a Super Admin
    auto userIt = users.find(userId);
    if (userIt != users.end() && userIt->second.isSuperAdmin)
    {
        return superAdminEmail;
    }

    // Check if it's a regular admin
    if (userIt != users.end() && !userIt->second.isClient)
    {
        return userIt->second.email;
    }

    // Check if it's a client
    auto clientIt = clients.find(userId);
    if (clientIt != clients.end())
    {
        return clientIt->second.email;
    }

    return "(no-email)";
}

string BankingSystem::getDisplayNameForUserId(const string &userId)
{
    if (userId == "Admin")
    {
        return "Admin";
    }
    auto it = clients.find(userId);
    if (it != clients.end())
    {
        return formatFullName(it->second, true) + " (" + it->second.accountNumber + ")"; // FIXED
    }
    return userId;
}

// ADD THE HELPER FUNCTION HERE
string BankingSystem::formatFullName(const Client &c, bool lastNameFirst)
{
    if (lastNameFirst)
    {
        // Format: Last Name, First Name Middle Name Suffix
        string fullName = c.lastName + ", " + c.firstName;
        if (!c.middleName.empty())
            fullName += " " + c.middleName;
        if (!c.suffix.empty())
            fullName += " " + c.suffix;
        return fullName;
    }
    else
    {
        // Format: First Name Middle Name Last Name Suffix
        string fullName = c.firstName;
        if (!c.middleName.empty())
            fullName += " " + c.middleName;
        fullName += " " + c.lastName;
        if (!c.suffix.empty())
            fullName += " " + c.suffix;
        return fullName;
    }
}
