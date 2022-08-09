/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   input_game_state.ino                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: jhille <jhille@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/09 13:38:42 by jhille        #+#    #+#                 */
/*   Updated: 2022/08/09 13:43:49 by jhille        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

void    wait_input_input(int pressed)
{
	if (pressed == 11 || pressed == 12 || pressed == 19 || pressed == 20)
	{
		game_state = ROUND_LOOP;
	}
}

void    p1_turn_input(int pressed)
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