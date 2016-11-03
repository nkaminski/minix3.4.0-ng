#!/bin/sh

echo Test 3 starting
echo Opengroup without receiving and have a proc send to the group, then closegroup. 
echo Sender should not block indefinitely
openwait_test_0 &
sleep 1
send_test_0 
