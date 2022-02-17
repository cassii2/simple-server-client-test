#!/bin/sh

./main 30630 "$(echo -e "HTTP/1.1 200 OK\nContent-Type: text/html\n\n<html>\n\t<h1>hi</h1>\n</html>\n\ngarbage\nasdgl")"
