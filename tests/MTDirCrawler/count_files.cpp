
/**
 * @code
 */

#include <45d/MTDirCrawler.hpp>
#include <45d/low_overhead_string.hpp>
#include <atomic>
#include <boost/filesystem.hpp>
#include <cassert>
#include <functional>
#include <iostream>
namespace fs = boost::filesystem;

void usage(void) {
	std::cout << "count-files /path/ pattern [# threads]" << std::endl;
}

/* @brief The callback function to push files into a vector
 *
 * @param e Directory entry passed to callback from MTDirCrawler
 * @param vec The vector to insert into passed via std::bind()
 * @param mt The mutex to synchronize insertion passed via std::bind()
 * @return true if e is a directory and is not a symlink (recurse in)
 * @return false if e is a file or special file (do not recurse)
 */
bool callback(const fs::directory_entry &e,
			  const char *comp,
			  std::atomic<unsigned long> &count) {
	if (!fs::is_directory(e)) {
		if (ffd::pattern_match(e.path().c_str(), comp))
			++count;
		return false;
	}
	return !fs::is_symlink(e);
}

int main(int argc, char *argv[]) {
	int threads;
	if (argc == 3) {
		threads = 8;
	} else if (argc == 4) {
		try {
			threads = std::stoi(argv[3]);
		} catch (const std::invalid_argument &) {
			usage();
			exit(1);
		}
	} else {
		usage();
		exit(1);
	}
	std::atomic<unsigned long> count(0);
	std::string path = argv[1];
	std::string pattern = argv[2];
	ffd::MTDirCrawler crawler{};

	try {
	/* Call crawler.crawl() passing base path as the first parameter,
	 * callback() as the second parameter using bind to tack on the remaining callback
	 * parameters, and finally the number of threads to spawn.
	 */
	crawler.crawl(path,
				  std::bind(callback, std::placeholders::_1, pattern.c_str(), std::ref(count)),
				  threads);
	} catch (const boost::filesystem::filesystem_error &e) {
		std::cout << e.what() << std::endl;
		return 1;
	}

	std::cout << count << std::endl;

	return 0;
}

/**
 * @endcode
 *
 */
