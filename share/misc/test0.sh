#!/bin/sh

echo Test 0 starting
echo 1 Receiver, then 1 Sender, Group 0
echo Sender and Receiver should return, Receiver getting message
receive_test_0 &
send_test_0 
