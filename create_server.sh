#!/bin/bash

if [ -z "$1" ]; then
  exit 1
fi


mkdir "$1"
mkdir "$1/config"
mkdir "$1/src"
