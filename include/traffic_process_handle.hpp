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
#include <functional>

#include "traffic_roi.hpp"
#include "traffic_frame.hpp"
#include "traffic_type.hpp"
#include "traffic_event.hpp"
#include "synchronized_queue.hpp"

class TrafficProcessHandle {
 public:
  TrafficProcessHandle() {
	std::thread t1([&]() {
	  this->Run();
	});
	std::thread t2([&]() {
	  this->ServiceEventPush();
	});
	threads_.push_back(std::move(t1));
	threads_.push_back(std::move(t2));
  }
  ~TrafficProcessHandle() {
	for (std::thread &t: this->threads_) {
	  if (t.joinable()) {
		t.join();
	  }
	}
  }

 private:
  void Run() {
	std::function<void(const TrafficEvent &event)>
		callback = std::bind(&TrafficProcessHandle::ServiceEventPull, this, std::placeholders::_1);
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
		std::shared_ptr<TrafficProcess> process_ptr = processes.at(i);
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
			process_ptr->Run(frame, roi_list, callback);
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

  void ServiceEventPush() {
	while (true) {
	  // websocket推送交通事件
	  TrafficEvent event;
	  bool pop_success = this->events_.pop(event);
	  if (pop_success) {
		printf("Push one event, type:%d\n", event.get_type());
	  }
	}
  }

  void ServiceEventPull(const TrafficEvent &event) {
	int retry_time = 3; // 尝试推送事件的次数
	bool push_success = false;
	for (int i = 0; i < retry_time; ++i) {
	  push_success = events_.push(event);
	  if (push_success) {
		break;
	  }
	}
	if (!push_success) {
	  // 事件发送失败
	}
  }
 private:
  std::mutex mutex_;
  std::map<std::string, std::shared_ptr<TrafficRoi>> rois_;
  SynchronizedQueue<TrafficEvent> events_;
  SynchronizedQueue<Frame> frames_;
  std::vector<std::shared_ptr<TrafficProcess>> processes_;
  std::vector<std::thread> threads_;
};
#endif //TRAFFICPROCESS_INCLUDE_TRAFFIC_PROCESS_HANDLE_HPP_
