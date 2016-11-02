#!/bin/sh

echo Test 3 starting
echo Opengroup without receiving, close group, then sender to group 
echo Sender should eventually be unblocked
openwait_test_0 &
send_test_0 
