"""
    Here is a little program to register the information about the different internship
    that you applied for.
"""

import csv
import os

CSV_FILENAME = "internship.csv"

FIELDNAMES = [
    "Internship Title",
    "Date",
    "Research platform",
    "Demand status",
    "Group/Organization"
]

def write_internship_data(internship_title, date_applied, research_platform, demand_status, group_name):
    """
    Writes new internship application information to a CSV file.
    It checks for duplicates based on 'Internship Title' and 'Research Platform'
    before adding a new entry.

    Args:
        internship_title (str): The title of the internship.
        date_applied (str): The date on which the internship was requested.
        research_platform (str): The platform where you find the internship.
        demand_status (str): Indicates if the internship was demanded or not.
        group_name (str): The name of the internship group.

    Returns:
        None
    """
    new_record = {
        "Internship Title": internship_title,
        "Date": date_applied,
        "Research platform": research_platform,
        "Demand status": demand_status,
        "Group/Organization": group_name
    }

    duplicate_found = False
    try:
        with open(CSV_FILENAME, "r", newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            for row in reader:
                if (row["Internship Title"] == new_record["Internship Title"] and
                    row["Research platform"] == new_record["Research platform"]):
                    duplicate_found = True
                    break
    except FileNotFoundError:
        pass
    except Exception as e:
        print(f"Error reading existing CSV for duplicate check: {e}")
        return

    if duplicate_found:
        print(f"Error: Internship '{internship_title}' from '{research_platform}' already exists.")
        return

    try:
        file_exists = os.path.exists(CSV_FILENAME)
        write_header = not file_exists or os.path.getsize(CSV_FILENAME) == 0

        with open(CSV_FILENAME, "a", newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=FIELDNAMES)
            if write_header:
                writer.writeheader()
            writer.writerow(new_record)
        print(f"Internship '{internship_title}' data successfully saved to {CSV_FILENAME}.")
    except Exception as e:
        print(f"Error writing internship data to CSV: {e}")

# -----------------------------------------------------------------------------------------------------------------------

def display_internship_data():
    """Reads and prints all internship data from the CSV file."""
    print("\n--- All Registered Internships ---")
    try:
        with open(CSV_FILENAME, "r", newline='') as csvfile:
            reader = csv.DictReader(csvfile)
            if not reader.fieldnames:
                print("CSV file is empty or has no header.")
                return

            header_line = " | ".join(reader.fieldnames)
            print(header_line)
            print("-" * len(header_line))

            data_found = False
            for row in reader:
                data_found = True
                row_values = []
                for field in FIELDNAMES:
                    row_values.append(row.get(field, "N/A"))
                print(" | ".join(row_values))
        if not data_found:
            print("No internship records found in the file yet.")

    except FileNotFoundError:
        print(f"The file '{CSV_FILENAME}' does not exist yet.")
    except Exception as e:
        print(f"Error reading and displaying data from CSV: {e}")
    print("----------------------------------\n")

# -----------------------------------------------------------------------------------------------------------------------

def main():
    print("Welcome, buddy, to your Internship Tracker!")
    while True:
        print("\nWhat would you like to do?")
        print("1. Add a new internship")
        print("2. View all registered internships")
        print("3. Exit")

        choice = input("Enter your choice (1, 2, or 3): ").strip()

        match choice:
            case "1":
                print("\n--- Add New Internship ---")
                internship_title = input("The title of the internship: ")
                if not internship_title:
                    print("Internship title cannot be empty. Please try again.")
                    continue

                date_applied = input("Date Applied (YYYY-MM-DD format): ")

                research_platform = input("The research Platform (e.g., HelloWork, Indeed, LinkedIn): ")
                if not research_platform:
                    print("Research platform cannot be empty. Please try again.")
                    continue

                demand_status_options = ['applied', 'pending', 'rejected', 'accepted', 'interview']
                while True:
                    demand_status = input(f"Have you already applied for this internship? (Options: {', '.join(demand_status_options)}): ").lower()
                    if demand_status in demand_status_options:
                        break
                    else:
                        print("Invalid demand status. Please choose from the provided options.")

                group_name = input("The name of the internship group (e.g., BNCP, BNP, General): ")
                if not group_name:
                    print("Group name cannot be empty. Please try again.")
                    continue

                write_internship_data(internship_title, date_applied, research_platform, demand_status, group_name)

            case "2":
                display_internship_data()

            case "3":
                print("Exiting Internship Tracker. Goodbye, buddy!")
                break

            case _:
                print("Invalid choice. Please enter 1, 2, or 3.")

if __name__ == "__main__":
    main()