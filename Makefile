
CC = clang++-3.6

ut_tsqueue:
	$(CC) --std=c++11 -Wall -g ./test/unittest_tsqueue.cc -o ./build/ut_tsqueue.out -lgtest -lpthread

ut_ntsqueue:
	$(CC) --std=c++11 -Wall -g ./test/unittest_ntsqueue.cc -o ./build/ut_ntsqueue.out -lpthread

clean:
	rm -f ./build/*
