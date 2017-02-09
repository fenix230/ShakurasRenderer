#pragma once
#include "Utility.h"
#include <chrono>
#include <iostream>
#include <unordered_map>


SHAKURAS_BEGIN;


class Profiler {
public:
	Profiler() {
		time_pre_ = std::chrono::steady_clock::now();
		frame_count_ = 0;
		report_span_ = 1000;
	}

public:
	void begin() {
		counters_.clear();
	}

	void end() {
		frame_count_++;

		std::chrono::time_point<std::chrono::steady_clock> time_now = std::chrono::steady_clock::now();
		auto du = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_pre_);

		if (du.count() > report_span_) {
			time_pre_ = time_now;

			//FPS
			float fps = (1.0f * report_span_ * frame_count_) / du.count();
			std::cout << "FPS : " << fps << std::endl;
			frame_count_ = 0;

			//Duration
			std::cout << "Duration In Milliseconds : " << du.count() << std::endl;

			//Counters
			for (auto i = counters_.begin(); i != counters_.end(); i++) {
				std::cout << i->first << " : " << i->second << std::endl;
			}

			//Additions
			for (auto i = additions_.begin(); i != additions_.end(); i++) {
				std::cout << i->first << " : " << i->second << std::endl;
			}

			//Endl
			std::cout << std::endl;
		}
	}

	void count(const std::string& counter_name, int incr = 1) {
		auto pos = counters_.find(counter_name);
		if (pos != counters_.end()) {
			pos->second += incr;
		}
		else {
			counters_[counter_name] = incr;
		}
	}

	void addition(const std::string& add_name, const std::string& add_value) {
		additions_[add_name] = add_value;
	}

private:
	std::chrono::time_point<std::chrono::steady_clock> time_pre_;
	uint32_t report_span_;
	size_t frame_count_;
	std::unordered_map<std::string, int> counters_;
	std::unordered_map<std::string, std::string> additions_;
};


class ScopeProfiling {
public:
	ScopeProfiling(Profiler& profiler) {
		profiler_ = &profiler;
		profiler_->begin();
	}

	~ScopeProfiling() {
		profiler_->end();
	}

private:
	Profiler* profiler_;
};


SHAKURAS_END;