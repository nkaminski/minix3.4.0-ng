#!/bin/sh

echo Killing Test 1 starting
echo 2 receivers in Group 0
echo 2 receiver call receive from Group 0
echo 1 receiver gets killed
echo Sender sends to Group 0
kill_test_3 &
kill_test_4 &
kill %2
kill_test_5 &