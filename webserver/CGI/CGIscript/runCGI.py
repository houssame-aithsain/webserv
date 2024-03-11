#!/usr/bin/env python3
import os

requestMethod = os.environ.get("REQUEST_METHOD", '')
query_string = os.environ.get("QUERY_STRING", '')

