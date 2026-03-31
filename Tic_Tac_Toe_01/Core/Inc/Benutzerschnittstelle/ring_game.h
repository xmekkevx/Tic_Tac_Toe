#ifndef INC_BENUTZERSCHNITTSTELLE_RING_GAME_H_
#define INC_BENUTZERSCHNITTSTELLE_RING_GAME_H_

#include <stdint.h>

void ring_game_init(void);
void ring_game_show_start_player(char player);
void ring_game_update_turns(uint8_t turn_count);
void ring_game_clear(void);
void ring_game_animate_x_win(void);
void ring_game_animate_o_win(void);

#endif /* INC_BENUTZERSCHNITTSTELLE_RING_GAME_H_ */
