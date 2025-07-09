# auto Ip Updater 🖥️📱

A simple C program that auto-updates your `hosts` file to point a custom domain (like `app.loc`) to your local IP address — perfect for testing websites on mobile devices in the same local network.

## 🔥 Features

- Automatically fetches your computer’s local IPv4 address
- Maps `app.loc` to your IP in the Windows `hosts` file
- Updates every 15 seconds
- Great for local dev + mobile testing
- Written in pure C, no external dependencies

## 📦 Requirements

- Windows
- Dev C++ (or any GCC-compatible compiler)
- Must run as **Administrator** to edit hosts file

## ⚙️ How It Works

- Uses `gethostname()` and `gethostbyname()` to get IP
- Opens and edits `C:\Windows\System32\drivers\etc\hosts`
- Adds/updates the line:
