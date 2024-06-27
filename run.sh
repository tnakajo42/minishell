#!/bin/bash

# clear
clear

# make re
echo make re
make re &> /dev/null

# check success of make re
if [ $? -eq 0 ]; then
  if [ "$1" == "valgrind" ]; then
    # run executable with valgrind
    echo "valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --suppressions=valgrind.supp ./minishell"
    valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --suppressions=valgrind.supp ./minishell
  else
    # run executable
    echo "./minishell"
    ./minishell
  fi
else
  echo "Error: make failed"
fi

# make fclean
echo "make fclean"
make fclean &> /dev/null

# norminette
if command -v "norminette" &> /dev/null; then
  echo "norminette error: " \
  && norminette | grep Error! | wc -l
fi
