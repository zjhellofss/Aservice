//
// Created by Fushenshen on 2022/7/23.
//

#ifndef TRAFFICPROCESS_INCLUDE_TRAFFIC_SERVICE_HPP_
#define TRAFFICPROCESS_INCLUDE_TRAFFIC_SERVICE_HPP_
#include "traffic_process_handle.hpp"
#include "traffic_roi_handle.hpp"

class TrafficService {
 public:
  void add_roi(const std::string &roi_id, const std::shared_ptr<TrafficRoi> &roi) {
	if (!roi) {
	  return;
	}
	if (roi->get_type() == TrafficType::TRAFFIC_UNKNOWN || roi_id.empty()) {
	  return;
	}

	bool add_success = this->roi_handle_.add_roi(roi_id, roi);
	if (add_success) {
	  const auto &rois = this->get_rois();
	  if (!rois.empty()) {
		this->process_handle_.set_rois(rois);
	  }
	} else {

	}
  }

  void remove_roi(const std::string &roi_id) {
	if (roi_id.empty()) {
	  return;
	}

	bool remove_success = roi_handle_.remove_roi(roi_id);
	if (remove_success) {
	  const auto &rois = this->get_rois();
	  if (!rois.empty()) {
		this->process_handle_.set_rois(rois);
	  }
	} else {

	}
  }

  std::map<std::string, std::shared_ptr<TrafficRoi>> get_rois() {
	const auto &rois = roi_handle_.get_rois();
	return rois;
  }

  std::optional<std::shared_ptr<TrafficRoi>> get_roi(const std::string &key) {
	if (!key.empty()) {
	  return std::nullopt;
	}
	const auto &roi = roi_handle_.get_roi(key);
	if (!roi.has_value()) {
	  return std::nullopt;
	} else {
	  return roi;
	}
  }

  void RegisterProcess(const std::shared_ptr<TrafficProcess> &traffic_process) {
	if (!traffic_process) {
	  return;
	}
	if (traffic_process->get_type() != TrafficType::TRAFFIC_UNKNOWN) {
	  process_handle_.RegisterProcess(traffic_process);
	}
  }

  std::vector<std::shared_ptr<TrafficProcess>> get_processes() {
	return process_handle_.get_processes();
  }

  void AcceptFrame(const Frame &frame) {
	this->process_handle_.AcceptFrame(frame);
  }
 public:
  TrafficService() = default;
  TrafficService(const TrafficService &) = delete;
  TrafficService &operator=(const TrafficService &) = delete;
 private:
  TrafficProcessHandle process_handle_;
  TrafficRoiHandle roi_handle_;
};
#endif //TRAFFICPROCESS_INCLUDE_TRAFFIC_SERVICE_HPP_
