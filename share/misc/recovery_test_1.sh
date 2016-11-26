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

cd ..
echo "Removing recoverable dir (and permanently deleting contents)"
rm -rf expect_test

echo "Contents of containing dir ('expect_test' should NOT exist):"
ls -al

echo "Available space ('/home' should have over 100mb free now):"
df -h

echo "Press enter to continue..."
read -r REPLY
echo "Tests done"
