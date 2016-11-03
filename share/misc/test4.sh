#!/bin/sh

echo Test 4 starting
echo Script should return and not print failure message.
echo msend should detect deadlock and return -ELOCKED or -208.
deadlock_test_0
