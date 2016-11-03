#!/bin/sh

echo Killing Test 0 starting
echo 2 receivers in Group 0
echo 1 receiver calls receive from Group 0
echo Sender sends to Group 0
echo Sender gets killed
echo The other receiver calls receive from Group 0
kill_test_0 &
kill_test_1 &
kill_test_2 &
ps | grep -v grep | grep kill_test_2 | awk '{print $1}' | xargs kill
