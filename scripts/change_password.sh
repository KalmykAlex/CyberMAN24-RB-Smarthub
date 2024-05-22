#!/bin/bash

# Check if the script is run as root
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root"
  exit 1
fi

# Define the user
USER="cyberman"

# Generate a random password
NEW_PASSWORD=$(< /dev/urandom tr -dc 'A-Za-z0-9!@#$%^&*' | head -c 16)

# Change the user's password
echo "$USER:$NEW_PASSWORD" | chpasswd

# Check if the password change was successful
if [ $? -eq 0 ]; then
  echo "Password for user $USER has been successfully changed."
  echo "New password: $NEW_PASSWORD"
else
  echo "Failed to change password for user $USER."
fi
