#include <utility>

//
// Created by Fushenshen on 2022/7/23.
//

#ifndef TRAFFICPROCESS_INCLUDE_TRAFFIC_EVENT_HPP_
#define TRAFFICPROCESS_INCLUDE_TRAFFIC_EVENT_HPP_
#include <ctime>
#include "traffic_roi.hpp"

class TrafficEvent {
 public:
  TrafficEvent() = default;
  TrafficEvent(int32_t video_id, TrafficType type, std::string roi_id, std::shared_ptr<TrafficRoi> roi)
	  : video_id_(video_id),
		time_stamp_(time(nullptr)),
		type_(type),
		roi_id_(std::move(roi_id)),
		roi_(std::move(roi)) {
  }
  TrafficType get_type() const { return this->type_; }
 private:
  int32_t video_id_ = -1;// 事件所属的相机
  int64_t time_stamp_ = 0; /// 事件发生的时间
  TrafficType type_ = TrafficType::TRAFFIC_UNKNOWN;/// 事件的类型
  std::string roi_id_; /// 事件所在的区域id
  std::shared_ptr<TrafficRoi> roi_; ///事件发生的区域
};
#endif //TRAFFICPROCESS_INCLUDE_TRAFFIC_EVENT_HPP_
