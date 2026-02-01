#include "../include/BankEase.h"

void BankingSystem::currencyExchange()
{
    clearScreen();
    displayHeader();
    cout << "\n";

    int menuNumber = currentUser.isClient ? 7 : 9;
    string title = "[" + to_string(menuNumber) + "] Currency Exchange";
    centerText(title);
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    // Filter active currencies
    vector<Currency> activeCurrencies;
    for (const auto &pair : currencies)
    {
        if (pair.second.isActive)
        {
            activeCurrencies.push_back(pair.second);
        }
    }

    if (activeCurrencies.empty())
    {
        cout << string(15, ' ') << "No currencies available for exchange.\n";
        cout << string(15, ' ') << "Contact administrator.\n\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        cin.get();
        return;
    }

    cout << string(15, ' ') << "Available currencies:\n";
    for (size_t i = 0; i < activeCurrencies.size(); i++)
    {
        cout << string(15, ' ') << "[" << (i + 1) << "] "
             << activeCurrencies[i].code << " - " << activeCurrencies[i].name
             << " (" << activeCurrencies[i].symbol << ")\n";
    }

    int from, to;
    double amount;

    // ===== FROM CURRENCY =====
    cout << "\n"
         << string(15, ' ') << "Convert from currency (1-" << activeCurrencies.size() << "): ";

    string fromInput = "";
    char ch;
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b' && !fromInput.empty())
        {
            fromInput.pop_back();
            cout << "\b \b";
        }
        else if (isdigit(ch))
        {
            fromInput += ch;
            cout << ch;
        }
    }
    cout << "\n";

    if (fromInput.empty())
    {
        cout << "\n"
             << string(15, ' ') << "Invalid input. Returning...\n";
        cin.get();
        return;
    }

    from = stoi(fromInput);

    if (from == 0)
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cin.get();
        return;
    }

    if (from < 1 || from > (int)activeCurrencies.size())
    {
        cout << "\n"
             << string(15, ' ') << "Invalid currency selection!\n";
        cin.get();
        return;
    }

    // ===== TO CURRENCY =====
    cout << string(15, ' ') << "Convert to currency (1-" << activeCurrencies.size() << "): ";

    string toInput = "";
    while ((ch = _getch()) != '\r')
    {
        if (ch == '\b' && !toInput.empty())
        {
            toInput.pop_back();
            cout << "\b \b";
        }
        else if (isdigit(ch))
        {
            toInput += ch;
            cout << ch;
        }
    }
    cout << "\n";

    if (toInput.empty())
    {
        cout << "\n"
             << string(15, ' ') << "Invalid input. Returning...\n";
        cin.get();
        return;
    }

    to = stoi(toInput);

    if (to == 0)
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cin.get();
        return;
    }

    if (to < 1 || to > (int)activeCurrencies.size())
    {
        cout << "\n"
             << string(15, ' ') << "Invalid currency selection!\n";
        cin.get();
        return;
    }

    // ===== AMOUNT =====
    cout << string(15, ' ') << "Amount: ";
    cin >> amount;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!cin)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "\n"
             << string(15, ' ') << "Invalid amount. Returning...\n";
        cin.get();
        return;
    }

    if (amount == 0)
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cin.get();
        return;
    }

    if (amount < 0)
    {
        cout << "\n"
             << string(15, ' ') << "Amount must be positive!\n";
        cin.get();
        return;
    }

    // Calculate conversion using dynamic rates
    const Currency &fromCurrency = activeCurrencies[from - 1];
    const Currency &toCurrency = activeCurrencies[to - 1];

    double result = amount * (toCurrency.rate / fromCurrency.rate);

    cout << "\n"
         << string(15, ' ') << fixed << setprecision(2) << amount << " "
         << fromCurrency.code << " = " << result << " " << toCurrency.code << "\n";
    cout << string(15, ' ') << "Exchange Rate: 1 " << fromCurrency.code
         << " = " << (toCurrency.rate / fromCurrency.rate) << " " << toCurrency.code << "\n";

    cout << "\n"
         << string(15, ' ') << "Press Enter to go back...";
    cin.get();
}
