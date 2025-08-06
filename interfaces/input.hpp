#ifndef INPUT_HPP
#define INPUT_HPP

#include "client.hpp"

struct CameraThirdData_t {
  float pitch;
  float yaw;
  float dist;
  float lag;
  Vec3 hull_min;
  Vec3 hull_max;
};

class Input {
public:

  user_cmd* get_commands(void) {
    return *(user_cmd**)(this + 0x108);
  }

  user_cmd* get_user_cmd(int sequence_number) {
    user_cmd* commands = this->get_commands();
    user_cmd* usercmd = &commands[ sequence_number % 90 ];

    if ( usercmd->command_number != sequence_number ) {
      return nullptr;	// usercmd was overwritten by newer command
    }

    return usercmd;
  }
  
  void to_thirdperson(void) {
    void** vtable = *(void***)this;

    void (*to_thirdperson_fn)(void*) = (void (*)(void*))vtable[32];

    to_thirdperson_fn(this);
  }

  bool is_thirdperson(void) {
    void** vtable = *(void***)this;

    bool (*is_thirdperson_fn)(void*) = (bool (*)(void*))vtable[31];

    return is_thirdperson_fn(this);
  }  
  
  void to_firstperson(void) {
    void** vtable = *(void***)this;

    void (*to_firstperson_fn)(void*) = (void (*)(void*))vtable[33];

    to_firstperson_fn(this);
  }

};

inline static Input* input;

#endif
