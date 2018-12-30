#!/bin/bash
mkdir include
wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
mkdir include/nlohmann
mv json.hpp include/nlohmann/
echo "nlohmann/json has been downloaded, you can link the library now."
