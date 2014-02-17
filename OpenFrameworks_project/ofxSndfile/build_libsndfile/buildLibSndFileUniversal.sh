export CFLAGS="-arch i386"
export CXXFLAGS=$CFLAGS
export LDFLAGS=$CFLAGS

cd libsndfile-1.0.25-i386
./configure --disable-sqlite; make


export CFLAGS="-arch x86_64"
export CXXFLAGS=$CFLAGS
export LDFLAGS=$CFLAGS

cd ../libsndfile-1.0.25-x86_64
./configure --disable-sqlite; make

sudo make install
sudo lipo -create src/.libs/libsndfile.1.dylib ../libsndfile-1.0.25-i386/src/.libs/libsndfile.1.dylib -output /usr/local/lib/libsndfile.1.dylib