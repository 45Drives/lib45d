// -*- C++ -*-
/*
 *    Copyright (C) 2021 Joshua Boudreau <jboudreau@45drives.com>
 *
 *    This file is part of lib45d.
 *
 *    lib45d is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    lib45d is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with lib45d.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>
#if __cplusplus >= 201703L
#	include <filesystem>
namespace ffd_internal_fs = std::filesystem;
#else
#	include <boost/filesystem.hpp>
namespace ffd_internal_fs = boost::filesystem;
#endif

namespace ffd {
	/**
	 * @brief Crawls through a directory with multiple worker threads, calling a
	 * calback function on each directory entry found.
	 *
	 */
	class MTDirCrawler {
	public:
		/**
		 * @brief Construct a new MTDirCrawler object
		 *
		 */
		MTDirCrawler() : done_(false), queue_(), workers_(), threads_running_(0), mutex_(), cv_() {}
		/**
		 * @brief Destroy the MTDirCrawler object
		 *
		 */
		~MTDirCrawler() = default;
		/**
		 * @brief Kicks off thread workers and waits for them to finish.
		 *
		 * The callback parameter needs to be a function that receives
		 * a directory_entry and returns a boolean. Return true from this
		 * function if the crawler should recurse into this directory entry
		 * given that it is a directory itself. To pass more parameters to the
		 * callback function, use std::bind().
		 *
		 * Example:
		 * @include tests/MTDirCrawler/find_files.cpp
		 *
		 * @param base_path Path to start the traversal from
		 * @param callback Function to call on each directory entry,
		 * should return true if the directory entry should be recursed into
		 * @param threads Number of worker threads to spawn
		 */
		void crawl(ffd_internal_fs::path base_path,
				   std::function<bool(const ffd_internal_fs::directory_entry &)> callback,
				   int threads) {
			crawl_async(base_path, callback, threads);
			wait();
		}
		/**
		 * @brief Kicks off thread workers. MTDirCrawler::wait() must be called at some point to
		 * join threads.
		 *
		 * @param base_path Path to start the traversal from
		 * @param callback Function to call on each directory entry,
		 * should return true if the directory entry should be recursed into
		 * @param threads Number of worker threads to spawn
		 */
		void crawl_async(ffd_internal_fs::path base_path,
						 std::function<bool(const ffd_internal_fs::directory_entry &)> callback,
						 int threads) {
			seed(base_path);
			threads_running_ = threads;
			for (int i = 0; i < threads; ++i) {
				workers_.emplace_back(&MTDirCrawler::worker, this, callback);
			}
		}
		/**
		 * @brief Wait for threads to finish. Must be called at some point after
		 * MTDirCrawler::crawl_async().
		 *
		 */
		void wait(void) {
			for (std::thread &t : workers_) {
				t.join();
			}
			workers_.clear();
			done_ = false;
		}
	private:
		bool done_;
		std::deque<ffd_internal_fs::directory_entry> queue_;
		std::vector<std::thread> workers_;
		std::atomic<int> threads_running_;
		std::mutex mutex_;
		std::condition_variable cv_;
		void seed(const ffd_internal_fs::path &base_path) {
			queue_.emplace_back(base_path);
		}
		void worker(std::function<bool(const ffd_internal_fs::directory_entry &)> callback) {
			ffd_internal_fs::directory_entry node;
			while (!done_) {
				{
					std::unique_lock<std::mutex> lk(mutex_);
					--threads_running_;
					if (threads_running_ <= 0 && queue_.empty()) {
						done_ = true;
						cv_.notify_all();
						return;
					}
					while (queue_.empty() && !done_) {
						cv_.wait(lk);
					}
					if (done_ && queue_.empty())
						return;
					node = queue_.front();
					queue_.pop_front();
					++threads_running_;
				}
				if (callback(node) && ffd_internal_fs::is_directory(node)) {
#if __cplusplus >= 201703L
					for (auto const &node : ffd_internal_fs::directory_iterator{ node }) {
#else
					for (ffd_internal_fs::directory_iterator ditr{ node };
						 ditr != ffd_internal_fs::directory_iterator{};
						 *ditr++) {
#endif
						{
							std::lock_guard<std::mutex> lk(mutex_);
#if __cplusplus >= 201703L
							queue_.emplace_back(node);
#else
							queue_.emplace_back(*ditr);
#endif
						}
						cv_.notify_one();
					}
				}
			}
		}
	};
} // namespace ffd
