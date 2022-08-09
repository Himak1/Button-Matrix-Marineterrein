/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   output_game_state.ino                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jhille <jhille@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/09 13:27:18 by jhille        #+#    #+#                 */
/*   Updated: 2022/08/09 13:48:12 by jhille        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

void	startup_output()
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

void	wait_input_output()
{
	led_on_off(11, ! led_state(11));
	led_on_off(12, ! led_state(12));
	led_on_off(19, ! led_state(19));
	led_on_off(20, ! led_state(20));
}

void	round_loop_outut()
{
	target = random(0, 31);
    game_state = P1_TURN;
}

void	p1_turn_output()
{
	led_on_off(target, ! led_state(target));
}


