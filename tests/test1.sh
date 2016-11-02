#!/bin/sh

echo Test 1 starting
echo 1 Sender, then 1 Receiver, Group 0
echo Sender and Receiver should return, Receiver get message
send_test_0
receive_test_0 &
