#!/bin/sh

if ! cp bin/linux/momoisay /usr/local/bin/; then
	sudo cp bin/linux/momoisay /usr/local/bin/
fi 2>/dev/null

if ! chmod +x /usr/local/bin/momoisay; then
	sudo chmod +x /usr/local/bin/momoisay
fi 2>/dev/null
