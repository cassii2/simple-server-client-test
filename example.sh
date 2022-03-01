#!/bin/sh
# "_POSIX_C_SOURCE=200112L" in defines in c_cpp_properties.json

./scripted 30630 "$(echo -e "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html>\n\t<h1>hi</h1>\n</html>\n\ngarbage\nasdgl")"
