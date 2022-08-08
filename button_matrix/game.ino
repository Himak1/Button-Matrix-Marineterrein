/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   game.ino                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: jhille <jhille@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 13:17:49 by jhille        #+#    #+#                 */
/*   Updated: 2022/08/08 13:42:07 by jhille        ########   odam.nl         */
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

int game_state = 0; // Which game_state/TEST game? 0 = running one, 1 = full on/off, 2 = tile toggle

/*
   One time set-up and initialisation of the game.
*/
void gameSetup() {
	Serial.println("Game setup!");
	if (game_state == 0)
		led_on_off(0, true);
}

typdef enum game_states
{
    STARTUP,
    WAIT_INPUT,
    ROUND_LOOP,
    P1_TURN,
    SHOW_SCORE
};

/*
   Globals for the game.
*/
int delayed = 0;
int	led_now = 0;
bool on_or_off = true;
bool blank_panel = true;

int target;
int score = 0;

inline void	wipe_leds()
{
	for (int i = 0; i < N_PANELS; i++)
		led_on_off(i, false);
	blank_panel = true;
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

		case 1:       // this step moves the leds one position
			if (game_state == STARTUP)
			{
				if (blank_panel == false)
					wipe_leds();
				led_on_off(led_now, on_or_off);
				led_now ++;
				if (led_now >= N_PANELS)
				{
                    led_now = 0;
					wipe_leds();
					game_state = WAIT_INPUT;
					blank_panel = false;
				}
			}
			else if (game_state == WAIT_INPUT)
			{
				led_on_off(11, ! led_state(11));
				led_on_off(12, ! led_state(12));
				led_on_off(19, ! led_state(19));
				led_on_off(20, ! led_state(20));
			}
            else if (game_state == ROUND_LOOP)
            {
                target = random(0, 31);
                game_state = P1_TURN;
            }
            else if (game_state == P1_TURN)
            {
                led_on_off(target, ! led_state(target));
            }
			if ( inputPending )
				return 2;	// Go to this step to process input
			break;	// We are going back to step 0 to delay once again.
		case 2:	// this step processes the input
			if (hasInput()) // Should always be true, just in case input has been stolen..
			{
				int pressed = getInput(); //

				if (game_state == WAIT_INPUT)
				{
					if (pressed == 11 || pressed == 12 || pressed == 19 || pressed == 20)
					{
						game_state = ROUND_LOOP;
					}
				}
                else if (game_state == P1_TURN)
                {
                    if (pressed == target)
                    {
                        score++;
                        game_state = SHOW_SCORE;
                    }
                    else
                    {
                        game_state = SHOW_SCORE;
                    }
                }
			}
			else
				Serial.println("Input stolen!");//
			break; // We are going back to step 0 to delay once again.;
	}
	return 0;   // Default return is to state 0;
}
