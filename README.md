# BankEase: C++ Banking System with Secure Authentication and Communication

## Project Overview

BankEase is a comprehensive console-based banking application built with modern C++ principles, designed to deliver enterprise-grade financial operations with a strong emphasis on secure authentication, role-based access control, and integrated communication features. The system provides a complete banking solution with advanced user management capabilities, extensive client services, sophisticated transaction processing, and built-in messaging infrastructure. Supporting multiple currencies and maintaining comprehensive audit trails, BankEase offers a production-ready foundation for secure banking environments.

![User Menu](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/user-menu.png)

---

## Key Features

### Authentication & Security

- Role-based authentication system with three distinct privilege levels: Super Admin, Admin, and Client
- Encrypted password storage with secure decryption protocols during authentication
- 8-digit access code requirement for Super Admin entry, adding an additional security barrier
- Comprehensive activity logging with timestamps for all login attempts and system actions
- Automatic audit trail generation for security compliance and monitoring

![Authentication](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/login-system.png)

### Client Account Management

- Create, modify, and remove client accounts with complete profile information
- Unique account number generation for each client with automatic sequencing
- Detailed client records including personal information, contact details, and PIN management
- Real-time balance tracking and account status monitoring
- Client search and retrieval functionality by account number or email

![Account Security](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/locked-account.png)

### Transaction Processing

- Support for deposits, withdrawals, and inter-account transfers
- Multi-currency transaction capabilities with automatic conversion
- Detailed transaction history with complete audit trails
- Balance validation and overdraft prevention mechanisms
- Transaction logging with timestamps, amounts, and balance snapshots for reconciliation

![Transaction](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/transaction-menu.png)

### Administrative Control

- Super Admin capabilities for creating and managing administrator accounts
- Dynamic admin creation with configurable permissions and roles
- System-wide reporting and analytics dashboard
- SMTP configuration for email communications
- Activity log viewer for system monitoring and troubleshooting

![Super Admin Control](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/superadmin-control.png)

### Communication Infrastructure

- Internal messaging between admins and clients without external dependencies
- Email notifications via SMTP for account activities and credentials
- Message history and inbox management
- Secure message delivery with recipient validation
- Support for system-generated automated communications

![Communication](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/communication.png)

### Currency Operations

- Comprehensive currency exchange functionality
- Real-time currency conversion with configurable exchange rates
- Support for multiple international currencies
- Currency value updates and management tools
- Seamless integration with transaction processing

![Currency](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/currency-exchange.png)

### Data Management

- Secure file-based data storage for users, clients, transactions, and messages
- Automatic data synchronization and integrity checks
- Separate encrypted storage for sensitive Super Admin credentials
- Transaction log preservation for audit and compliance requirements
- Configuration file management for system settings and SMTP parameters

![Data Management](https://raw.githubusercontent.com/Kenn025/BankEase/main/assets/data-management.png)

---

## Technology Stack

- **C++ (C++17)**

---

## Build & Run

### Requirements

- C++ compiler with C++17 support (GCC / MinGW / MSVC)
- Windows: MinGW-w64 or MSVC
- Linux/Mac: GCC 7+ or Clang 5+

### Compilation

**Option 1: Admin/Client Mode (Standard Build)**

Compile the main application for Admin and Client users:

```bash
g++ -std=c++17 -o bankEase src/*.cpp
```

On Windows with g++:

```bash
g++ -std=c++17 -o bankEase.exe src/*.cpp
```

**Option 2: Super Admin Mode (Separate Build)**

Compile the Super Admin application:

```bash
g++ -std=c++17 -DSUPERADMIN_MAIN -o bankEase-superadmin src/superadmin/SuperAdminMain.cpp src/*.cpp
```

On Windows with g++:

```bash
g++ -std=c++17 -DSUPERADMIN_MAIN -o bankEase-superadmin.exe src/superadmin/SuperAdminMain.cpp src/*.cpp
```

### Execution

**Running Admin/Client Application:**

Linux/Mac:

```bash
./bankEase
```

Windows:

```bash
bankEase.exe
```

This version allows:

- **Admins** to log in with username/password to manage clients, transactions, and system operations
- **Clients** to log in with account number/PIN to access their accounts and perform banking operations

**Running Super Admin Application:**

Linux/Mac:

```bash
./bankEase-superadmin
```

Windows:

```bash
bankEase-superadmin.exe
```

This version provides Super Admin access to:

- Create and manage administrator accounts
- View all system users and clients
- Access system reports and analytics
- Configure system-wide settings
- Monitor activity logs

### First-Time Setup

**Super Admin First Run:**
On the first execution of the Super Admin application, you'll be guided through initial setup:

1. Create Super Admin username
2. Set secure password
3. Configure email address for notifications
4. Set 8-digit access code (additional security layer)

**Admin/Client First Run:**

- No initial setup required
- Super Admin creates the first Admin account
- Admins create Client accounts through the application

---

## Project Structure

```
BankEase/
├─ include/
│  ├─ BankEase.h
│  ├─ Models.h
│  └─ Utils.h
├─ src/
│  ├─ superadmin/
│  │  └─ SuperAdminMain.cpp
│  ├─ Activity.cpp
│  ├─ Admin.cpp
│  ├─ Auth.cpp
│  ├─ Clients.cpp
│  ├─ Communication.cpp
│  ├─ Core.cpp
│  ├─ Currency.cpp
│  ├─ DataStore.cpp
│  ├─ InputHelpers.cpp
│  ├─ SuperAdmin.cpp
│  ├─ Transactions.cpp
│  └─ bankEase.cpp
├─ data/
│  ├─ .gitkeep
│  ├─ clients.txt
│  ├─ users.txt
│  ├─ transactions.txt
│  ├─ messages.txt
│  ├─ last_account.txt
│  ├─ activity_logs.txt
│  ├─ config.txt
│  └─ superadmin.txt
├─ .gitignore
└─ README.md
```

---

## Contributing

Contributions are welcome! Whether you're fixing bugs, improving documentation, or adding new features, we appreciate your help.
