#!/usr/bin/bash

# Ruby gems
echo -e "\e[1;92m=====[ Installing Ruby gems ]=====\e[0m"
sudo gem install serialport

# Python packages
echo -e "\e[1;92m=====[ Installing Python packages ]=====\e[0m"
pip install numpy
pip install matplotlib

# Julia packages
echo -e "\e[1;92m====[ Installing Julia packages ]=====\e[0m"
cd ./scripts
./install_julia_packages.jl
