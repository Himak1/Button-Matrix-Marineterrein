/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   button_matrix.h                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: jhille <jhille@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/08/08 13:24:43 by jhille        #+#    #+#                 */
/*   Updated: 2022/08/08 13:41:34 by jhille        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUTTON_MATRIX_H
# define BUTTON_MATRIX_H
# include <stdint.h>

inline bool		hasInput();
inline uint8_t	getInput();
inline uint8_t	peekInput();

inline void	led_on_off(uint8_t lednum, bool on_off);
inline bool	led_state(uint8_t lednum);

void        emptyQueue();

#endif
