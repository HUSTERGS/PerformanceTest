#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <libpmemlog.h>
#include <iostream>
#include <gtest/gtest.h>
#include <thread>
/* size of the pmemlog pool -- 1 GB */
#define POOL_SIZE ((size_t)(1 << 30))

/*
 * printit -- log processing callback for use with pmemlog_walk()
 */

int multi_thread_test(uint64_t key_size, uint64_t value_size, int nums, PMEMlogpool *plp)
{
	/* create the pmemlog pool or open it if it already exists */

	// std::string key(key_size, '1');
	// std::string value(value_size, '1');
	std::string buf(key_size + value_size, '1');
	// buf.append(reinterpret_cast<char *>(&key_size));
	// buf.append(reinterpret_cast<char *>(&value_size));
	// buf.append(key);
	// buf.append(value);

	for (; nums > 0; nums--)
	{
		if (pmemlog_append(plp, buf.data(), buf.size()) < 0)
		{
			perror("pmemlog_append");
			exit(1);
		}
	}
	return 0;
}

static int
printit(const void *buf, size_t len, void *arg)
{
	fwrite(buf, len, 1, stdout);
	return 0;
}


TEST(a, b)
{
	const char path[] = "/pmem-fs/myfile";
	PMEMlogpool *plp;

	/* create the pmemlog pool or open it if it already exists */
	plp = pmemlog_create(path, POOL_SIZE, 0666);

	if (plp == NULL)
		plp = pmemlog_open(path);

	if (plp == NULL)
	{
		perror(path);
		exit(1);
	}

	pmemlog_rewind(plp);
	std::thread t1(multi_thread_test, 1024, 1024, 100, plp);
	std::thread t2(multi_thread_test, 1024, 1024, 100, plp);
	t1.join();
	t2.join();
	pmemlog_close(plp);
}

int main(int argc, char **argv)
{
	// testing::InitGoogleTest(&argc, argv);
	// return RUN_ALL_TESTS();
	static PMEMlogpool *plp;
	const char path[] = "/pmem-fs/myfile";
	plp = pmemlog_create(path, POOL_SIZE, 0666);

	if (plp == NULL)
		plp = pmemlog_open(path);

	if (plp == NULL)
	{
		perror(path);
		exit(1);
	}
	pmemlog_rewind(plp);
	auto nums = (1 << 20) / 4;
	auto start = std::chrono::high_resolution_clock::now();
	std::thread t1(multi_thread_test, 128, 128, nums, plp);
	std::thread t2(multi_thread_test, 128, 128, nums, plp);
	std::thread t3(multi_thread_test, 128, 128, nums, plp);
	std::thread t4(multi_thread_test, 128, 128, nums, plp);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << "time: " << diff.count() << "output: "<< 256 / diff.count() <<  std::endl;
}
