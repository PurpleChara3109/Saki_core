# Internship Tracker

A simple command-line Python application to help you keep track of internships you've applied for. Register new internship details, including title, application date, research platform, demand status, and group, and view all your recorded applications.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Usage](#usage)
- [CSV File Format](#csv-file-format)

## Features

- **Add New Internship:** Record details for each internship application.
- **Duplicate Check:** Prevents adding the same internship twice based on title and research platform.
- **View All Internships:** Display a list of all previously recorded internship applications.
- **Persistent Storage:** All data is saved to a CSV file (`internship.csv`) for easy management and persistence across sessions.
- **Interactive Menu:** Simple command-line interface for ease of use.
- **Basic Input Validation:** Ensures critical fields are not empty and guides on valid status options.

## Getting Started

Follow these instructions to get a copy of the project up and running on your local machine.

### Prerequisites

You need Python 3 installed on your system.

* [Python 3](https://www.python.org/downloads/) (Version 3.6 or higher is recommended for `match` statement support).

### Installation

No special installation steps are required beyond having Python 3. This project uses only Python's built-in modules (`csv`, `os`).

  **Clone the repository (or download the files):**
    If your project is in a Git repository, use:
    ```bash
    git clone <your-repository-url>
    cd <your-project-directory>
    ```
    Otherwise, simply navigate to the directory where you saved `internship_tracker.py` (or whatever you named your main Python script) and the `README.md` file.

### Usage

Once you have Python set up and your environment activated (if you created one):

1.  **Run the script:**
    ```bash
    python3 register_intership.py
    ```

2.  **Follow the menu prompts:**
    The program will present a menu with options to add new internships, view existing ones, or exit.

    ```
    Welcome, buddy, to your Internship Tracker!

    What would you like to do?
    1. Add a new internship
    2. View all registered internships
    3. Exit
    Enter your choice (1, 2, or 3):
    ```

## CSV File Format

The application stores internship data in a CSV file named `internship.csv` located in the same directory as the script. The file uses the following header structure:

```csv
Internship title,Date,Research platform,Demand status,Group/Organization
