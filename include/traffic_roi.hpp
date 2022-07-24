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
class TrafficRoiJudgement : public TrafficRoi
{
public:
	explicit TrafficRoiJudgement(TrafficType type) : TrafficRoi(type)
	{
	}
};

#endif //TRAFFICPROCESS_INCLUDE_TRAFFIC_ROI_HPP_
