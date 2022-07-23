//
// Created by Fushenshen on 2022/7/23.
//

#ifndef TRAFFICPROCESS_INCLUDE_TRAFFIC_ROI_HPP_
#define TRAFFICPROCESS_INCLUDE_TRAFFIC_ROI_HPP_
#include <string>
#include <map>
#include <optional>
#include <vector>

#include "traffic_type.hpp"

class TrafficRoi {
 public:
  virtual ~TrafficRoi() = default;

  explicit TrafficRoi(TrafficType type) : type_(type) {

  }

  void set_type(const TrafficType &type) {
	this->type_ = type;
  }

  const TrafficType &get_type() const {
	return this->type_;
  }

 private:
  TrafficType type_ = TrafficType::TRAFFIC_UNKNOWN;
};

/**
 * 基础检测区域
 */
class TrafficRoiBase : public TrafficRoi {
 public:
  explicit TrafficRoiBase(TrafficType type) : TrafficRoi(type) {

  }

};

/**
 * 规则检测区域
 */
class TrafficROIJudgement : public TrafficRoi {
 public:
  explicit TrafficROIJudgement(TrafficType type) : TrafficRoi(type) {

  }
};

/**
 * 检测区域管理
 */
class TrafficROIHandle {

 public:
  bool add_roi(const std::string &roi_id, const std::shared_ptr<TrafficRoi> &roi) {
	assert(roi && !roi_id.empty());
	if (this->rois_.find(roi_id) != rois_.end()) {
	  //duplicated key
	  return false;
	}
	this->rois_.insert({roi_id, roi});
	return true;
  }

  bool remove_roi(const std::string &roi_id) {
	assert(!roi_id.empty());
	if (this->rois_.find(roi_id) == rois_.end()) {
	  return false;
	}
	this->rois_.erase(roi_id);
	return true;
  }

  const std::map<std::string, std::shared_ptr<TrafficRoi>> &get_rois() {
	return this->rois_;
  }

  std::optional<std::shared_ptr<TrafficRoi>> get_roi(const std::string &key) {
	assert(!key.empty());
	auto iter = this->rois_.find(key);
	if (iter != this->rois_.end()) {
	  return iter->second;
	} else {
	  return std::nullopt;
	}
  }
 private:
  std::map<std::string, std::shared_ptr<TrafficRoi>> rois_;
};

#endif //TRAFFICPROCESS_INCLUDE_TRAFFIC_ROI_HPP_
