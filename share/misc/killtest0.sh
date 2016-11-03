#!/bin/sh

echo Interrupted syscall test 0 starting
echo 2 receivers in Group 0
echo 2 receiver call receive from Group 0
echo 1 receiver gets killed
echo Sender sends to Group 0
echo EXPECTED OUTPUT: One copy of message
kill_test_3 &
kill_test_4 &
sleep 1
ps | grep -v grep | grep kill_test_3 | awk '{print $1}' | xargs kill
sleep 1
ps | grep -v grep | grep kill_test
echo One receiver should be running
kill_test_5
