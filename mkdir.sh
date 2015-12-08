#!/bin/bash

DIR_NAME=`date +%F_%T | tr -s '-' '_' | tr -s ":" "_"`

mkdir $DIR_NAME
rm -fr today
ln -s $DIR_NAME today
