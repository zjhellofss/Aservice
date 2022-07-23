//
// Created by Fushenshen on 2022/7/23.
//

#ifndef TRAFFICPROCESS_INCLUDE_TRAFFIC_PROCESS_HPP_
#define TRAFFICPROCESS_INCLUDE_TRAFFIC_PROCESS_HPP_
#include "traffic_frame.hpp"
#include <iostream>
#include <vector>
#include <cassert>
#include "traffic_type.hpp"
#include "traffic_roi.hpp"

class TrafficProcess {
 public:
  explicit TrafficProcess(const TrafficType &type) : type_(type) {
  }

  void set_type(const TrafficType &type) {
	this->type_ = type;
  }

  const TrafficType &get_type() const {
	return this->type_;
  }

  virtual void Run(const Frame &frame, const std::vector<std::shared_ptr<TrafficRoi>> &roi_list) = 0;

 private:
  TrafficType type_ = TrafficType::TRAFFIC_UNKNOWN;
};

/**
 * 基础检测
 */

class TrafficProcessBaseDetection : public TrafficProcess {
 public:
  explicit TrafficProcessBaseDetection(const TrafficType &type = TrafficType::TRAFFIC_BASE_DETECTION) :
	  TrafficProcess(type) {

  }

  void Run(const Frame &frame, const std::vector<std::shared_ptr<TrafficRoi>> &roi_list) override {
	for (int i = 0; i < roi_list.size(); ++i) {
	  const std::shared_ptr<TrafficRoi> &roi = roi_list.at(i);
	  TrafficRoiBase *roi_base = dynamic_cast<TrafficRoiBase *>(roi.get());
	  // CHECK(roi_base!=nullptr)
	  assert(roi_base != nullptr);
	  //检测事件类型和有效范围所属类型是否一致
	  assert(roi_base->get_type() == TrafficProcess::get_type());
	}
	std::cout << "process for base detection\n";
  }
};

/**
 * 车辆逆行
 */
class TrafficProcessConverseDriving : public TrafficProcess {
 public:
  explicit TrafficProcessConverseDriving(const TrafficType &type = TrafficType::TRAFFIC_CONVERSE_DRIVING)
	  : TrafficProcess(type) {

  }

  void Run(const Frame &frame, const std::vector<std::shared_ptr<TrafficRoi>> &roi_list) override {
	for (int i = 0; i < roi_list.size(); ++i) {
	  const std::shared_ptr<TrafficRoi> &roi = roi_list.at(i);
	  TrafficROIJudgement *roi_judgement = dynamic_cast<TrafficROIJudgement *>(roi.get());
	  assert(roi_judgement != nullptr);
	  //检测事件类型和有效范围所属类型是否一致
	  assert(roi_judgement->get_type() == TrafficProcess::get_type());
	}
	std::cout << "process for conversing driving detection\n";
  }
};

/**
 * 非法停车
 */
class TrafficProcessIllegalStopping : public TrafficProcess {
 public:
  explicit TrafficProcessIllegalStopping(const TrafficType &type = TrafficType::TRAFFIC_ILLEGAL_STOPPING)
	  : TrafficProcess(type) {

  }

  void Run(const Frame &frame, const std::vector<std::shared_ptr<TrafficRoi>> &roi_list) override {
	for (int i = 0; i < roi_list.size(); ++i) {
	  const std::shared_ptr<TrafficRoi> &roi = roi_list.at(i);
	  TrafficROIJudgement *roi_judgement = dynamic_cast<TrafficROIJudgement *>(roi.get());
	  assert(roi_judgement != nullptr);
	  //检测事件类型和有效范围所属类型是否一致
	  assert(roi_judgement->get_type() == TrafficProcess::get_type());
	}
	std::cout << "process for illegal stopping\n";
  }
};

#endif //TRAFFICPROCESS_INCLUDE_TRAFFIC_PROCESS_HPP_
