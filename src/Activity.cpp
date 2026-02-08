#include "../include/BankEase.h"

void BankingSystem::logActivity(const string &action, const string &detail)
{
    ofstream file(dataPath("activity_logs.txt"), ios::app);
    if (!file.is_open())
    {
        return;
    }

    string cleanDetail = detail;
    for (char &c : cleanDetail)
    {
        if (c == '\n' || c == '\r')
        {
            c = ' ';
        }
    }

    string role = "Admin";
    if (currentUser.isSuperAdmin)
    {
        role = "SuperAdmin";
    }
    else if (currentUser.isClient)
    {
        role = "Client";
    }

    string user = currentUser.username.empty() ? "(unknown)" : currentUser.username;
    file << getCurrentDateTime() << "|"
         << user << "|"
         << role << "|"
         << action << "|"
         << cleanDetail << "\n";
}

void BankingSystem::viewActivityLogs()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Activity Logs]");
    centerText(string(110, '-'));
    cout << "\n";

    ifstream file(dataPath("activity_logs.txt"));
    if (!file.is_open())
    {
        centerText("No activity logs found.");
        cout << "\n"
             << string(15, ' ') << "Press Enter to go back...";
        while (_getch() != '\r')
            ;
        return;
    }

    cout << left
         << string(2, ' ') << setw(24) << "Date"
         << setw(18) << "User"
         << setw(12) << "Role"
         << setw(18) << "Action"
         << "Detail\n";
    cout << string(110, '-') << "\n";

    string line;
    bool hasAny = false;
    while (getline(file, line))
    {
        if (trim(line).empty())
        {
            continue;
        }
        hasAny = true;
        stringstream ss(line);
        string date, user, role, action, detail;
        getline(ss, date, '|');
        getline(ss, user, '|');
        getline(ss, role, '|');
        getline(ss, action, '|');
        getline(ss, detail);

        if (detail.length() > 48)
        {
            detail = detail.substr(0, 45) + "...";
        }

        cout << string(2, ' ') << setw(24) << date
             << setw(18) << user
             << setw(12) << role
             << setw(18) << action
             << detail << "\n";
    }

    if (!hasAny)
    {
        centerText("No activity logs found.");
    }

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    while (_getch() != '\r')
        ;
}
