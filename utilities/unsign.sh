#!/bin/bash
find "$1" -type f -exec codesign --remove-signature {} \; 2>/dev/null