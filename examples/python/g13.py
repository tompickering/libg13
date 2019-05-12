#!/usr/bin/env python3

import time
import random
import ctypes


# Unfortunately ctypes does not allow us to access enum values, so we'll define them again
(
    G1, G2, G3, G4, G5, G6, G7, G8, G9, G10,
    G11, G12, G13, G14, G15, G16, G17, G18, G19, G20,
    G21, G22,
    M1, M2, M3, MR,
    T1, T2, T3, T4,
    CLICK1, CLICK2,
    ROUND, BRIGHT,
    N_KEYS
) = map(ctypes.c_int, range(35))

# Callback types
ButtonCallbackType = ctypes.CFUNCTYPE(None)
StickCallbackType = ctypes.CFUNCTYPE(None, ctypes.c_uint8, ctypes.c_uint8)

# Load the library
libg13 = ctypes.cdll.LoadLibrary('/usr/lib/libg13.so')


def g1_bind():
    print('G1 pressed')

def stick_bind(x, y):
    print('Stick at {},{}'.format(x, y))

libg13.g13_init()

# Set up function bindings
libg13.g13_bind_key(G1, ButtonCallbackType(g1_bind))
libg13.g13_bind_stick(StickCallbackType(stick_bind))

while True:
    time.sleep(2)
    libg13.g13_set_colour(random.getrandbits(8), random.getrandbits(8), random.getrandbits(8))
