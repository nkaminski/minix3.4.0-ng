#!/bin/sh
cd /home
echo "Make rcdir"
if rcmkdir expect_test; then
    echo "Made rcdir successfully"
else
    echo "Failed to make rcdir"
    exit 1
fi
cd expect_test
echo "Make 1 megabyte test file"
if dd if=/dev/zero of=smallfile count=1 bs=1m; then
    echo "Made 'smallfile' successfully"
else
    echo "Failed to make first test file"
    exit 1
fi
echo "Contents of dir:"
ls -al
echo "Removing test file"
rm smallfile
echo "Contents of dir ('smallfile' should be gone):"
ls -al
echo "Recovering test file"
if undelete smallfile; then
    echo "Undeleted 'smallfile' successfully"
else
    echo "Failed to undelete test file"
    exit 1
fi
echo "Contents of dir ('smallfile' should be restored):"
ls -al

echo "Press enter to continue..."
read -r REPLY

echo "Removing smallfile again"
rm smallfile

echo "Make 100MB file, 'big'"
if dd if=/dev/zero of=big count=100 bs=1m; then
    echo "Made 'big' successfully"
else
    echo "Failed to make large test file"
    exit 1
fi
echo "Contents of dir ('big' should be the only file present):"
ls -al

echo "Press enter to continue..."
read -r REPLY

echo "Deleting 'big'"
rm big
echo "Make 100MB file, 'big2' to trigger GC"
if dd if=/dev/zero of=big2 count=100 bs=1m; then
    echo "Made 'big2' successfully"
else
    echo "Failed to make 2nd large test file"
    exit 1
fi
echo "Contents of dir ('big2' should be the only file present):"
ls -al

echo "Press enter to continue..."
read -r REPLY

echo "GC follows LRU policy -> 'big' and 'testfile' should be irrecoverable"
echo "Undeleting them to make sure"
undelete big
undelete testfile
echo "Contents of dir ('big2' should be the only file present):"
ls -al

echo "Press enter to continue..."
read -r REPLY

echo "Make 100MB file, 'big3' to run out of space"
if dd if=/dev/zero of=big3 count=100 bs=1m; then
    echo "Made 'big3' successfully, something is wrong!"
    exit 1
else
    echo "Full disk detected correctly"
fi

echo "Cleaning up files"
rm big3
rm big2
echo "Test Done"
