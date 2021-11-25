Committing the output of the gernated portions of `./configure`. These can be reproduced for any architecture type by running the following commands on a computer of that architecture.

```shell script
wget https://github.com/HDFGroup/hdf5/archive/5b9cf732caab9daa6ed1e00f2df4f5a792340196.tar.gz
tar -xf 5b9cf732caab9daa6ed1e00f2df4f5a792340196.tar.gz
cd hdf5-5b9cf732caab9daa6ed1e00f2df4f5a792340196/
./configure && make -j2
cat src/H5lib_settings.c > $(arch)_H5lib_settings.c
cat src/H5pubconf.h > $(arch)_H5pubconf.h
src/H5detect > $(arch)_native.c
```
