#include "../interfaces/input.hpp"

void (*client_create_move_original)(void*, int, float, bool);

void client_create_move_hook(void* me, int sequence_number, float input_sample_frametime, bool active) {
  client_create_move_original(me, sequence_number, input_sample_frametime, active);

  user_cmd* user_cmd = input->get_user_cmd(sequence_number);
  if (user_cmd == nullptr) return;
  
  //print("%p - %d - %d - %f - %d\n", user_cmd, user_cmd->tick_count, sequence_number%90, input_sample_frametime, active);

  
  //print("client create move hooked!\n");
}
