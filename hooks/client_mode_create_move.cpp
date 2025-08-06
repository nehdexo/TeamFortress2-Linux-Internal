#include "../interfaces/client.hpp"
#include "../interfaces/engine.hpp"
#include "../interfaces/entity_list.hpp"
#include "../interfaces/convar_system.hpp"

#include "../gui/config.hpp"

#include "../classes/player.hpp"
#include "../print.hpp"


#include "../hacks/aimbot/aimbot.cpp"
 
bool (*client_mode_create_move_original)(void*, float, user_cmd*);

bool client_mode_create_move_hook(void* me, float sample_time, user_cmd* user_cmd) {

  bool rc = client_mode_create_move_original(me, sample_time, user_cmd);
  
  if (!user_cmd || !user_cmd->command_number) {
    return rc;
  }

  if (!engine->is_in_game()) {
    return rc;
  }

  Player* localplayer = entity_list->player_from_index(engine->get_localplayer_index());

  if (localplayer == nullptr) {
    print("localplayer is NULL\n");
    return rc;
  }
  
  if (user_cmd->tick_count > 1) {    
    
    aimbot(user_cmd);
      
    static bool bStaticJump = false, bStaticGrounded = false, bLastAttempted = false;
    const bool bLastJump = bStaticJump;
    const bool bLastGrounded = bStaticGrounded;

    bStaticJump = user_cmd->buttons & IN_JUMP;
    const bool bCurJump = bStaticJump;

    bStaticGrounded = localplayer->get_ground_entity();
    const bool bCurGrounded = bStaticGrounded;

    if (bCurJump && bLastJump && (bCurGrounded ? !localplayer->is_ducking() : true)) {
      if (!(bCurGrounded && !bLastGrounded))
	user_cmd->buttons &= ~IN_JUMP;
      
      if (!(user_cmd->buttons & IN_JUMP) && bCurGrounded && !bLastAttempted)
	user_cmd->buttons |= IN_JUMP;
    }    

    
    //no push
    static Convar* nopush = convar_system->find_var("tf_avoidteammates_pushaway");
    if (nopush != nullptr && config.misc.no_push == true) {
      if (nopush->get_int() != 0) {
        nopush->set_int(0);
      }
    } else if (nopush != nullptr && config.misc.no_push == false) {
      if (nopush->get_int() != 1) {
        nopush->set_int(1);
      }
    }

    /*
    //bhop
    static bool was_jumping = false;
    bool on_ground = (localplayer->get_ent_flags() & FL_ONGROUND);

    if(user_cmd->buttons & IN_JUMP && config.misc.bhop == true) {

      if(!was_jumping && !on_ground) {
	user_cmd->buttons &= ~IN_JUMP;
      } else if(was_jumping) {
	was_jumping = false;
      }
      
    } else if(!was_jumping) {
      was_jumping = true;
    }
    */
  } 
  
  if (config.aimbot.silent == true)
    return false;
  else
    return rc;
}
