/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.ino                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jhille <jhille@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 13:17:49 by jhille        #+#    #+#                 */
/*   Updated: 2022/08/09 18:19:58 by jhille        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// Routines for the 3 game_state/TEST "games"
/*
	game() should not take too much time or it will make shift
	register polling too slow. Lengthy processes should be
	cut into pieces and can be controlled with the state parameter.
	First call gets state=0, subsequent calls the previous return value.
	input_pending will be true if switch has been pressed. You can use
	s = getInput() to get and remove the next switch pushed
	s = peekInput() to get the next switched pushed but leave it on the queue
	emptyQueue() to empty the queue
*/

typedef enum game_states
{
    STARTUP,
    WAIT_INPUT,
    P1_TARGET,
    P1_TURN,
    MISSED,
	HIT,
    SHOW_SCORE
};

int game_state = STARTUP; // Which game_state/TEST game? 0 = running one, 1 = full on/off, 2 = tile toggle

/*
   One time set-up and initialisation of the game.
*/
void gameSetup() {
	Serial.println("Game setup!");
	if (game_state == 0)
		led_on_off(0, true);
}

/*
   Globals for the game.
*/
int delayed = 0;
int	led_now = 0;
bool on_or_off = true;
bool blank_panel = true;
int	duration = 0;

int target;
int score = 0;

inline void	wipe_leds()
{
	for (int i = 0; i < N_PANELS; i++)
		led_on_off(i, false);
	blank_panel = true;
}

inline void	led_on_off_range(int min, int max)
{
	for (int i = min; i <= max; i++)
		led_on_off(i, ! led_state(i));
}

/*
   Game execution.
*/
int game(int state, bool inputPending)
{
	const uint8_t DELAY_TIME = 120;
	bool led_0;

	switch ( state )
	{
		case 0:       // This step does partial 1 ms delays until done delaying.
			delay(1);
			delayed += 1;
			if (delayed > DELAY_TIME)
			{
				delayed = 0;  // prime for next delay
				return 1;  // Done delaying, move to next step.
			}
			break;

		case 1:       // processing output
			if (game_state == STARTUP)
				startup_output();
			else if (game_state == WAIT_INPUT)
				wait_input_output();
      else if (game_state == P1_TARGET)
				p1_target_output();
      else if (game_state == P1_TURN)
				p1_turn_output();
      else if (game_state == MISSED)
        missed_output();
			else if (game_state == SHOW_SCORE)
				show_score_output();
			if ( inputPending )
				return 2;	// Go to this step to process input
			break;	// We are going back to step 0 to delay once again.
		case 2:	// this step processes the input
			if (hasInput()) // Should always be true, just in case input has been stolen..
			{
				int pressed = getInput(); //

				if (game_state == WAIT_INPUT)
					wait_input_input(pressed);
        else if (game_state == P1_TURN)
					p1_turn_input(pressed);
			}
			else
				Serial.println("Input stolen!");//
			break; // We are going back to step 0 to delay once again.;
	}
	return 0;   // Default return is to state 0;
}
