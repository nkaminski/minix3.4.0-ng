#!/bin/sh
cd /home/expect_test
echo "Contents of dir ('persifile' should still be gone):"
ls -al

echo "Press enter to continue..."
read -r REPLY

echo "Undeleting 'persifile'"
undelete persifile
echo "Contents of dir ('persifile' should be recovered):"
ls -al

echo "Press enter to continue..."
read -r REPLY

echo "Removing test file"
rm persifile
echo "Tests done"
