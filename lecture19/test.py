#!/usr/bin/env python3

import os

fd = os.open('/dev/quote', os.O_RDONLY)
print(os.read(fd, 1024))
