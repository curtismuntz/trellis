#!/usr/bin/env python3
import ctypes

ecal_so_location = "external/ecal/ecal_wrap.so"
ecal = ctypes.cdll.LoadLibrary(ecal_so_location)

# demangle the function call?
getversion = getattr(ecal, "_Z15ecal_getversionv")
getdate = getattr(ecal, "_Z12ecal_getdatev")

def main():
    print("dir(ecal)")
    print(dir(ecal))
    print()
    print("getversion()")
    print(getversion())
    woo = getversion()


    print("getdate")
    print(getdate)
    print("getdate()")
    print(getdate())
    print("hello world")

if __name__ == '__main__':
    main()
