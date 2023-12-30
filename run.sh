#!/bin/bash
#echo "Do you want to delete the old build folder? (y/n)"
#read delete
#if [ $delete = "y" ]; then
#  rm -rf build
#  mkdir build
#fi
make_output=$(make)
if [ $? -ne 0 ]; then
  echo "\033[31mError occurred during make. Stopping script.\033[0m"
  echo "$make_output"
  exit 1
fi
echo "Do you want to run the program? (y/n)"
read run
if [ $run = "y" ]; then
  echo "Which file do you want to run?"
  files=(build/*)
  files_count=0
  for ((i = 0; i < ${#files[@]}; i++)); do
    if [ -f "${files[i]}" ]; then
      files_count=$((files_count + 1))
    fi
  done
  for ((i = 0; i < files_count; i++)); do
    echo "\033[34m$((i + 1)).\033[0m \033[32m${files[i * 2]}\033[0m"
  done
  read choice
  choice=$((choice - 1))
  while (true); do
    if [ -f "${files[choice * 2]}" ]; then
      echo "\033[32mrunning ${files[choice * 2]}\033[0m"
      ${files[choice * 2]}
      break
    fi
    echo "\033[31mInvalid choice.\033[0m Please choose a valid option:"
    for ((i = 0; i < files_count; i++)); do
      echo "\033[34m$((i + 1)).\033[0m \033[32m${files[i * 2]}\033[0m"
    done
    read choice
  done
fi
