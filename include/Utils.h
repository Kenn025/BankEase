#pragma once

#include <cctype>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

using namespace std;

// Utility function for simple encryption (Caesar cipher for demonstration)
inline string encrypt(const string &text, int shift = 3)
{
    string result = text;
    for (char &c : result)
    {
        if (isalnum(c))
        {
            char base = isdigit(c) ? '0' : (isupper(c) ? 'A' : 'a');
            int range = isdigit(c) ? 10 : 26;
            int offset = (c - base + shift) % range;
            if (offset < 0)
            {
                offset += range;
            }
            c = base + offset;
        }
    }
    return result;
}

inline string decrypt(const string &text, int shift = 3)
{
    return encrypt(text, -shift);
}

// trim helper
inline string trim(const string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos)
        return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// gmail validator
inline bool isValidGmail(const string &rawEmail)
{
    string email = trim(rawEmail);
    if (email.empty())
        return false;
    if (email.find(' ') != string::npos)
        return false;
    size_t atPos = email.find('@');
    if (atPos == string::npos)
        return false;
    if (email.substr(atPos) != "@gmail.com")
        return false;
    if (atPos == 0)
        return false; // nothing before @
    if (email.find('@', atPos + 1) != string::npos)
        return false; // multiple @
    return true;
}

// delay helper
inline void sleepMs(int ms)
{
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

// ===== Hidden input with dot (•) echo =====
inline string getHiddenInput(const string &prompt)
{
    cout << prompt;
    cout.flush();
    string input;

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);

    char ch;
    while (true)
    {
        ch = _getch();

        if (ch == 13)
        {
            cout << "\n";
            break;
        }
        else if (ch == 8)
        {
            if (!input.empty())
            {
                input.pop_back();
                cout << "\b \b";
                cout.flush();
            }
        }
        else if (ch == 0 || ch == -32)
        {
            _getch();
        }
        else if (ch >= 32 && ch <= 126)
        {
            input.push_back(ch);
            cout << "•";
            cout.flush();
        }
    }
#else
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (true)
    {
        int ch = getchar();
        if (ch == '\n' || ch == '\r')
        {
            cout << "\n";
            break;
        }
        else if (ch == 127 || ch == 8)
        {
            if (!input.empty())
            {
                input.pop_back();
                cout << "\b \b";
                cout.flush();
            }
        }
        else if (ch == 27)
        {
            getchar();
            getchar();
        }
        else if (ch >= 32 && ch <= 126)
        {
            input.push_back(static_cast<char>(ch));
            cout << "•";
            cout.flush();
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

    return input;
}
