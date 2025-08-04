#!/bin/bash

set -e

echo "🔧 Building server..."
make

echo "🚀 Starting server with args: $@"
./main "$@"