cd build
cmake -G "Unix Makefiles" ../src
make
cd ..
python3 tests/run.py
