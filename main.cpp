#include <thread>
#include <chrono>
#include "traffic_service.hpp"
#include "traffic_type.hpp"

int main() {
  TrafficService service;
  std::shared_ptr<TrafficRoi> roi_base1 = std::make_shared<TrafficRoiBase>(TrafficType::TRAFFIC_BASE_DETECTION);
  std::shared_ptr<TrafficRoi> roi_base2 = std::make_shared<TrafficRoiBase>(TrafficType::TRAFFIC_BASE_DETECTION);
  service.add_roi("0575", roi_base1);
  service.add_roi("0571", roi_base2);

  std::shared_ptr<TrafficRoi>
	  roi_judgement1 = std::make_shared<TrafficRoiJudgement>(TrafficType::TRAFFIC_CONVERSE_DRIVING);
  std::shared_ptr<TrafficRoi>
	  roi_judgement2 = std::make_shared<TrafficRoiJudgement>(TrafficType::TRAFFIC_CONVERSE_DRIVING);
  std::shared_ptr<TrafficRoi>
	  roi_judgement3 = std::make_shared<TrafficRoiJudgement>(TrafficType::TRAFFIC_ILLEGAL_STOPPING);
  std::shared_ptr<TrafficRoi>
	  roi_judgement4 = std::make_shared<TrafficRoiJudgement>(TrafficType::TRAFFIC_ILLEGAL_STOPPING);

  service.add_roi("0573", roi_judgement1);
  service.add_roi("0574", roi_judgement2);
  service.add_roi("0579", roi_judgement3);
  service.add_roi("0589", roi_judgement4);

  const auto &rois = service.get_rois();

  std::shared_ptr<TrafficProcess> process_base1 = std::make_shared<TrafficProcessBaseDetection>();
  std::shared_ptr<TrafficProcess> process_base2 = std::make_shared<TrafficProcessBaseDetection>();
  std::shared_ptr<TrafficProcess> process_conversing1 = std::make_shared<TrafficProcessConverseDriving>();
  std::shared_ptr<TrafficProcess> process_conversing2 = std::make_shared<TrafficProcessConverseDriving>();
  std::shared_ptr<TrafficProcess> process_conversing3 = std::make_shared<TrafficProcessConverseDriving>();

  std::shared_ptr<TrafficProcess> process_stopping1 = std::make_shared<TrafficProcessIllegalStopping>();
  std::shared_ptr<TrafficProcess> process_stopping2 = std::make_shared<TrafficProcessIllegalStopping>();


  service.RegisterProcess(process_base1);
  service.RegisterProcess(process_base2);
  service.RegisterProcess(process_conversing1);
  service.RegisterProcess(process_conversing2);
  service.RegisterProcess(process_conversing3);

  service.RegisterProcess(process_stopping1);
  service.RegisterProcess(process_stopping2);


  const auto &processes = service.get_processes();
  while (true) {
	using namespace std::chrono_literals;
	Frame frame;
	service.AcceptFrame(frame);
	std::this_thread::sleep_for(20ms);
  }
  return 0;
}