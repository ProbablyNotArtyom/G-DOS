#!/bin/bash

if [ -f ./.config ]; then
	rm -f ./.config
fi

touch ./.config
for arg in "$@"; do
	echo "$arg" >> ./.config
done
