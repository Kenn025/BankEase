#include "../include/BankEase.h"

void BankingSystem::centerText(const string &text, int width)
{
    int padding = (width - (int)text.length()) / 2;
    if (padding < 0)
        padding = 0;
    cout << string(padding, ' ') << text << "\n";
}

void BankingSystem::leftCenterText(const string &text, int width)
{
    int padding = (width - (int)text.length()) / 2;
    if (padding < 0)
        padding = 0;
    cout << string(padding, ' ') << text << "\n";
}

string BankingSystem::getCurrentDateTime()
{
    time_t now = time(0);
    now += 8 * 60 * 60; // Philippine Time (UTC+8) without DST
    tm *ltm = gmtime(&now);
    const char *weekdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    stringstream ss;
    ss << setfill('0')
       << weekdays[ltm->tm_wday] << " - "
       << setw(2) << ltm->tm_mday << "/"
       << setw(2) << (1 + ltm->tm_mon) << "/"
       << (1900 + ltm->tm_year) << " - "
       << setw(2) << ltm->tm_hour << ":"
       << setw(2) << ltm->tm_min << ":"
       << setw(2) << ltm->tm_sec;
    return ss.str();
}

void BankingSystem::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void BankingSystem::displayHeader()
{
    cout << "\n";
    centerText(string(70, '-'));
    centerText("Main Menu");
    centerText(string(70, '-'));
    string userLine = "User : " + currentUser.username;
    leftCenterText(userLine);
    string dateLine = "Date : " + getCurrentDateTime();
    leftCenterText(dateLine);
    centerText(string(70, '-'));
}

void BankingSystem::runSuperAdmin()
{
    while (true)
    {
        if (!isLoggedIn)
        {
            if (!superAdminLoginPage())
                continue;
        }

        if (!currentUser.isSuperAdmin)
        {
            isLoggedIn = false;
            continue;
        }

        superAdminMenu();
    }
}

void BankingSystem::run()
{
    while (true)
    {
        if (!isLoggedIn)
        {
            if (!login())
                continue;
        }

        // Route to appropriate menu
        if (currentUser.isClient)
        {
            clientMenu();
        }
        else if (currentUser.isSuperAdmin)
        {
            superAdminMenu(); // Super Admin menu
        }
        else
        {
            adminMenu(); // Regular Admin menu
        }
    }
}
