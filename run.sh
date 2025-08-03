#!/bin/bash

set -e

echo "🔧 Building server..."
make

echo "🚀 Starting server..."
./server