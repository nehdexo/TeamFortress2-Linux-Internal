#ifndef PREDICTION_HPP
#define PREDICTION_HPP

class Prediction {
public:
  int last_ground{};
  bool in_prediction{};
  bool first_time_predicted{};
  bool old_cl_predict_value{};
  bool engine_paused{};
  int previous_start_frame{};
  int commands_predicted{};
  int server_commands_acknowledged{};
  int previous_ack_had_errors{};
  int incoming_packet_number{};
  float ideal_pitch{};
};

inline static Prediction* prediction;

#endif
