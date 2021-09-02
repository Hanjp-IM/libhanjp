It is a library implemented for Hanjp input method engines.
It is designed on GObject type system, so that it is easily portable to many platforms.
You can use GObject Introspection for cross-language interoperability.


## How can we build?
```
git clone https://github.com/Hanjp-IM/libhanjp.git
cd libhanjp
mkdir build
cd build
git submodule add
cmake ..
make
```

## Dependencies
Currently, the library depends on two packages.
```
glib-2.0
libhangul
```
How can we install above?
```
# On ubuntu
sudo apt update && sudo apt install libglib2.0-dev libhangul-dev
```
