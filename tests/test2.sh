#!/bin/sh

echo Test 2 starting
echo 5 Receivers, then 1 Sender, Group 0
echo Sender and all Receivers should return, all getting message
receive_test_0 &
receive_test_0 &
receive_test_0 &
receive_test_0 &
receive_test_0 &
send_test_0 
echo 5 Receivers, then 1 Sender, Group 1
echo Sender and all Receivers should return, all getting message
receive_test_1 &
receive_test_1 &
receive_test_1 &
receive_test_1 &
receive_test_1 &
send_test_1 
