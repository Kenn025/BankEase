#include "../include/BankEase.h"

// Send real email using curl
bool BankingSystem::sendRealEmail(const string &headerFromEmail, const string &toEmail,
                                  const string &subject, const string &body,
                                  const string &replyTo)
{

    if (gmailAppPassword.empty())
    {
        return false; // No app password configured
    }

    string smtpFrom = config.smtpFromEmail;
    string headerFrom = headerFromEmail.empty() ? config.smtpFromEmail : headerFromEmail;
    if (smtpFrom.empty() || headerFrom.empty())
    {
        return false;
    }

    string protocol = config.smtpUseSsl ? "smtps" : "smtp";
    string smtpUrl = protocol + "://" + config.smtpHost + ":" + to_string(config.smtpPort);

    // Create temporary file for email content
    ofstream emailFile("temp_email.txt");
    emailFile << "From: <" << headerFrom << ">\r\n";
    emailFile << "To: <" << toEmail << ">\r\n";
    (void)replyTo;
    emailFile << "Subject: " << subject << "\r\n";
    emailFile << "\r\n";
    emailFile << body << "\r\n";
    emailFile.close();

    string command;

#ifdef _WIN32
    command = "curl --url \"" + smtpUrl + "\" "
                                          "--ssl-reqd --mail-from \"" +
              smtpFrom + "\" "
                     "--mail-rcpt \"" +
              toEmail + "\" "
                        "--upload-file temp_email.txt "
                        "--user \"" +
              smtpFrom + ":" + gmailAppPassword + "\" "
                                              "--insecure 2>nul 1>nul";
#else
    command = "curl --url \"" + smtpUrl + "\" "
                                          "--ssl-reqd --mail-from \"" +
              smtpFrom + "\" "
                     "--mail-rcpt \"" +
              toEmail + "\" "
                        "--upload-file temp_email.txt "
                        "--user \"" +
              smtpFrom + ":" + gmailAppPassword + "\" "
                                              "--insecure 2>/dev/null 1>/dev/null";
#endif

    int result = system(command.c_str());

    // Clean up temp file
    remove("temp_email.txt");

    return (result == 0);
}

void BankingSystem::communication()
{
    while (true)
    {
        clearScreen();
        displayHeader();
        cout << "\n";
        centerText("[8] Communication");
        centerText(string(70, '-'));
        leftCenterText("[1] Send Message (by email)");
        leftCenterText("[2] View Inbox");
        leftCenterText("[0] Back to Main Menu");
        centerText(string(70, '-'));
        cout << string(15, ' ') << "Enter your choice: ";

        string choiceInput = "";
        char ch;
        while (true)
        {
            ch = _getch();

            // Enter key - only accept if input is not empty
            if (ch == '\r')
            {
                if (!choiceInput.empty())
                {
                    cout << "\n";
                    break;
                }
                // If empty, ignore Enter and continue waiting
                continue;
            }

            // Backspace
            if (ch == '\b' && !choiceInput.empty())
            {
                choiceInput.pop_back();
                cout << "\b \b";
            }
            // Only allow digits (limit to 1 character for menu options)
            else if (isdigit(ch))
            {
                if (choiceInput.empty())
                {
                    choiceInput += ch;
                    cout << ch;
                }
                // Silently ignore if already has 1 digit
            }
            // Ignore non-digit characters
        }

        int choice = stoi(choiceInput);

        switch (choice)
        {
        case 1:
            sendMessage();
            break;
        case 2:
            viewInbox();
            break;
        case 0:
            cout << "\n"
                 << string(15, ' ') << "Returning to Main Menu...";
            Sleep(1000); // 1 second delay
            return;      // Exit immediately
        default:
            cout << "\n"
                 << string(15, ' ') << "Invalid choice! Please try again.";
            Sleep(1000); // 1 second delay
            break;       // Continue loop to show menu again
        }
    }
}

void BankingSystem::sendMessage()
{
    clearScreen();
    displayHeader();
    cout << "\n";
    centerText("[Send Message]");
    centerText(string(70, '-'));
    cout << "\n";
    cout << string(15, ' ') << "Type '0' at any field to cancel the operation.\n\n";

    Message m;
    m.from = currentUser.username;

    string toEmail;
    cout << string(15, ' ') << "To (email): ";
    getline(cin, toEmail);

    if (toEmail == "0")
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        cin.get();
        return;
    }

    string toUserId;
    if (!findUserByEmail(toEmail, toUserId))
    {
        cout << "\n"
             << string(15, ' ') << "User with this email not found!\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        cin.get();
        return;
    }

    string subject;
    cout << string(15, ' ') << "Subject (0 to cancel): ";
    getline(cin, subject);
    if (trim(subject) == "0")
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        cin.get();
        return;
    }

    cout << string(15, ' ') << "Message (0 to cancel): ";
    getline(cin, m.content);
    if (trim(m.content) == "0")
    {
        cout << "\n"
             << string(15, ' ') << "Operation cancelled.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        cin.get();
        return;
    }

    m.to = toUserId;
    m.date = getCurrentDateTime();
    m.isRead = false;

    // Save to internal message system
    messages.push_back(m);
    saveMessages();

    // Send real email
    string fromEmail = getEmailForUserId(m.from);
    string senderName = getDisplayNameForUserId(m.from);
    if (trim(fromEmail).empty() || fromEmail == "(no-email)")
    {
        cout << "\n"
             << string(15, ' ') << "Sender email is not set. Please update your account email first.\n";
        cout << string(15, ' ') << "Press Enter to go back...";
        cin.get();
        return;
    }

    if (trim(subject).empty())
    {
        subject = "Banking System Message from " + senderName;
    }

    // Build email body with sender info
    stringstream emailBody;
    emailBody << "BankEase Secure Message\n\n";
    emailBody << "From : " << senderName << "\n";
    emailBody << "Email: " << fromEmail << "\n";
    emailBody << "Date : " << m.date << "\n\n";
    emailBody << "Message:\n";
    emailBody << m.content << "\n\n";
    emailBody << "Sent via BankEase.\n";

    cout << "\n"
         << string(15, ' ') << "Sending email to " << toEmail << "...\n";

    bool emailSent = sendRealEmail(fromEmail, toEmail, subject, emailBody.str(), "");

    if (emailSent)
    {
        cout << string(15, ' ') << "✓ Message sent successfully (Internal + Real Email)!\n";
    }
    else
    {
        cout << string(15, ' ') << "✓ Message saved internally.\n";
        if (gmailAppPassword.empty())
        {
            cout << string(15, ' ') << "⚠ Real email not sent - SMTP password not configured.\n";
            cout << string(15, ' ') << "  Configure it in Super Admin Account Information.\n";
        }
        else
        {
            cout << string(15, ' ') << "⚠ Real email delivery failed. Check internet connection.\n";
        }
    }

    cout << string(15, ' ') << "Press Enter to go back...";
    cin.get();
}
