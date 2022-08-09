/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   output_game_state.ino                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: jhille <jhille@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/09 13:27:18 by jhille        #+#    #+#                 */
/*   Updated: 2022/08/09 17:56:06 by jhille        ########   odam.nl         */
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

void	p1_target_output()
{
	target = random(0, 31);
  game_state = P1_TURN;
	duration = 0;
	wipe_leds();
}

void	p1_turn_output()
{
	led_on_off(target, ! led_state(target));
}

void	missed_output()
{
  Serial.println("yeet");
	if (duration % 2 == 0)
	{
		for (int i = 2; i < 30; i += 9)
			led_on_off(i, true);
		for (int i = 5; i < 27; i += 7)
			led_on_off(i, false);
		duration++;
	}
	else if (duration < 4)
	{
		for (int i = 2; i < 30; i += 9)
			led_on_off(i, false);
		for (int i = 5; i < 27; i += 7)
			led_on_off(i, true);
		duration++;
	}
	else
	{
		wipe_leds();
		game_state = SHOW_SCORE;
		duration = 0;
	}
}

void	hit_output()
{
	if (duration < 4)
	{
		led_on_off(13, ! led_state(13));
		led_on_off(4, ! led_state(4));
		led_on_off(11, ! led_state(11));
		led_on_off(18, ! led_state(18));
		led_on_off(25, ! led_state(25));
		duration++;
	}
	else
	{
		duration = 0;
		wipe_leds();
		game_state = SHOW_SCORE;
	}
}

void	show_score_output()
{
	if (score == 4)
  {
	score = 0;
		game_state = STARTUP;
  }
	else
  {
		game_state = P1_TARGET;
  }
  Serial.println("Score is");
  Serial.println(score);
}
