import subprocess

# A script for run all commands to build the necessaries to use Git,
# first on Linux

GIT_INIT = "git init"
GIT_CONFIG_NAME = "git config --global user.name \"{name}\""
GIT_CONFIG_EMAIL = "git config --global user.email \"{email}\""
GIT_REMOTE_ADD = "git remote add origin {repo_url}"
SSH_KEYGEN_RSA = "ssh-keygen -t rsa"
SSH_KEYGEN_ED25519 = "ssh-keygen -t ed25519"

"""
    Function to display the commands to initiate git, the first time.
"""
def display_git_commands():
    print("--- Git Setup Commands for your local deposit---")
    print(f"1. Initialize a new Git repository: {GIT_INIT}")
    print(f"2. Configure your global Git username: {GIT_CONFIG_NAME.format(name='your_name')}")
    print(f"3. Configure your global Git email: {GIT_CONFIG_EMAIL.format(email='your_email')}")
    print(f"4. Link to your remote repository: {GIT_REMOTE_ADD.format(repo_url='your_repo_url')}")
    print("\n--- SSH Key Generation ---")
    print(f"If you don't have an SSH key, here are the commands to generate one:")
    print(f"For an RSA key: {SSH_KEYGEN_RSA}")
    print(f"For an Ed25519 key: {SSH_KEYGEN_ED25519}")
    print("-----------------------------------\n")

"""
    Function to execute a given shell command.
    It prints the command and its output, handling potential errors.
"""
def execute_command(command_str):
    print(f"\n--- Executing: {command_str} ---")
    try:
        # Use subprocess.run with check=True to raise CalledProcessError on non-zero exit codes
        result = subprocess.run(command_str, shell=True, check=True, capture_output=True, text=True)
        if result.stdout:
            print("STDOUT:\n", result.stdout.strip()) # .strip() removes trailing newlines
        if result.stderr:
            print("STDERR:\n", result.stderr.strip())
        print("---------------------------------")
        return True # Indicate success
    except subprocess.CalledProcessError as e:
        print(f"ERROR: Command failed with exit code {e.returncode}")
        print("STDOUT (on error):\n", e.stdout.strip())
        print("STDERR (on error):\n", e.stderr.strip())
        print("---------------------------------")
        return False # Indicate failure
    except FileNotFoundError:
        print(f"ERROR: Command not found. Make sure '{command_str.split()[0]}' is in your system's PATH.")
        print("---------------------------------")
        return False

def execute_all():
    print("\nNow, let's start! \Enter some information to set up Git:")

    my_name = input("Your name (e.g., John Doe): ").strip()
    my_email = input("Your email (e.g., john.doe@example.com): ").strip()
    my_repo_url = input("Your remote repository URL (SSH or HTTPS, e.g., git@github.com:user/repo.git or https://github.com/user/repo.git): ").strip()

    # Construct the commands with user input
    commands_to_run = [
        GIT_INIT,
        GIT_CONFIG_NAME.format(name=my_name),
        GIT_CONFIG_EMAIL.format(email=my_email),
        GIT_REMOTE_ADD.format(repo_url=my_repo_url)
    ]

    # Decide about SSH key generation
    generate_ssh_key = input("Do you want to generate a new SSH key (recommended if you don't have one)? (y/N): ").strip().lower()
    if generate_ssh_key == 'y':
        key_type = input("Which type of SSH key? (rsa or ed25519, default: ed25519): ").strip().lower()
        if key_type == 'rsa':
            commands_to_run.append(SSH_KEYGEN_RSA)
        else:
            commands_to_run.append(SSH_KEYGEN_ED25519) # Default to ed25519 if not rsa

    print("\n--- Starting Git Setup Process ---")
    for command in commands_to_run:
        if not execute_command(command):
            print("Stopping script due to previous command failure.")
            break # Stop if any command fails

def main():
    print("Welcome to Git Setup Assistant!")
    display_git_commands() # Show an overview first
    execute_all()
    print("\nGit setup process completed (or stopped due to an error).")

if __name__ == "__main__":
    main()