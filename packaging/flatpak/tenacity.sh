#!/usr/bin/env bash

# FIXME following variable needs to be fixed in CMake.
export LD_LIBRARY_PATH=/app/lib/tenacity

exec /app/bin/tenacity
