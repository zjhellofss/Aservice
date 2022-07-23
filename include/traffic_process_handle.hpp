//
// Created by Fushenshen on 2022/7/23.
//

#ifndef TRAFFICPROCESS_INCLUDE_TRAFFIC_PROCESS_HANDLE_HPP_
#define TRAFFICPROCESS_INCLUDE_TRAFFIC_PROCESS_HANDLE_HPP_
#include "traffic_process.hpp"
#include <thread>
#include <vector>
#include <map>
#include <mutex>

#include "traffic_roi.hpp"
#include "traffic_frame.hpp"
#include "traffic_type.hpp"
#include "synchronized_queue.hpp"

class TrafficProcessHandle {
 public:
  TrafficProcessHandle() {
	std::thread t([&]() {
	  this->Run();
	});
	thread_self_ = std::move(t);
  }
  ~TrafficProcessHandle() {
	if (thread_self_.joinable()) {
	  thread_self_.join();
	}
  }

 private:
  void Run() {
	while (true) {
	  Frame frame;
	  this->frames_.pop(frame);
	  std::vector<std::shared_ptr<TrafficProcess>> processes;
	  std::map<std::string, std::shared_ptr<TrafficRoi>> rois;
	  {
		///
		std::unique_lock<std::mutex> lock(this->mutex_);
		rois = this->rois_;
		processes = this->processes_;
	  }
	  if (rois.empty() || processes.empty()) {
		continue;
	  }
	  for (size_t i = 0; i < processes.size(); ++i) {
		std::shared_ptr process_ptr = processes.at(i);
		const auto type = process_ptr->get_type();
		if (type == TrafficType::TRAFFIC_UNKNOWN) {
		  continue;
		  // LOG ERROR
		} else {
		  std::vector<std::shared_ptr<TrafficRoi>> roi_list;
		  for (const auto &pair: rois) {
			//如果检测框类型和事件类型一致
			if (pair.second->get_type() == type) {
			  roi_list.push_back(pair.second);
			}
		  }
		  if (!roi_list.empty()) {
			process_ptr->Run(frame, roi_list);
		  } else {
//			LOG INFO NO ROI RULES
		  }
		}
	  }
	}
  }
 public:
  void AcceptFrame(const Frame &frame) {
	this->frames_.push(frame);
  }

  void RegisterProcess(const std::shared_ptr<TrafficProcess> &process) {
	assert(process);
	/**
	 * 和service线程之间的同步
	 */
	std::unique_lock<std::mutex> lock(this->mutex_);
	this->processes_.push_back(process);
  }

  std::vector<std::shared_ptr<TrafficProcess>> get_processes() {
	std::unique_lock<std::mutex> lock(this->mutex_);
	return this->processes_;
  }

  void set_rois(const std::map<std::string, std::shared_ptr<TrafficRoi>> &rois) {
	/**
	 * 和service线程之间的同步
	 */
	std::unique_lock<std::mutex> lock(this->mutex_);
	this->rois_ = rois;
  }
 private:
  std::mutex mutex_;
  std::map<std::string, std::shared_ptr<TrafficRoi>> rois_;
  SynchronizedQueue<Frame> frames_;
  std::vector<std::shared_ptr<TrafficProcess>> processes_;
  std::thread thread_self_;
};
#endif //TRAFFICPROCESS_INCLUDE_TRAFFIC_PROCESS_HANDLE_HPP_
