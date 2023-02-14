/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/11 18:01:12 by adouay            #+#    #+#             */
/*   Updated: 2023/02/14 01:55:41 by ilandols         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D.h"

static t_bool	is_vertical_wall(char **map, t_fpos src, t_bool to_right)
{
	t_pos	pos;

	pos.x = src.x / TILE - 1 + to_right;
	pos.y = src.y / TILE;

	if (pos.y < 0 || pos.y >= ft_get_size_array(map))
		return (1);
	return (pos.x < 0 || pos.x > ft_strlen(map[pos.y]) || map[pos.y][pos.x] == '1');
}

static t_bool	is_horizontal_wall(char **map, t_fpos src, t_bool to_down)
{
	t_pos	pos;

	pos.x = src.x / TILE;
	pos.y = src.y / TILE - 1 + to_down;
	return (pos.y < 0 || pos.y >= ft_get_size_array(map) || map[pos.y][pos.x] == '1');
}

static t_fpos	get_vertical_collision(t_game *game, t_fpos start)
{
	t_fpos	point;

	point = start;
	while (!is_vertical_wall(game->map, point, !game->ray.to_left))
	{
		put_point(game, point.x, point.y, GREEN);
		if (game->ray.to_up)
			point.y -= game->ray.offset_v.y;
		else
			point.y += game->ray.offset_v.y;
		if (game->ray.to_left)
			point.x -= game->ray.offset_v.x;
		else
			point.x += game->ray.offset_v.x; 
	}
	return (point);
}

static t_fpos	get_horizontal_collision(t_game *game, t_fpos start)
{
	t_fpos	point;

	point = start;
	while (!is_horizontal_wall(game->map, point, !game->ray.to_up))
	{
		put_point(game, point.x, point.y, GREEN);
		if (game->ray.to_up)
			point.y -= game->ray.offset_h.y;
		else
			point.y += game->ray.offset_h.y;
		if (game->ray.to_left)
			point.x -= game->ray.offset_h.x;
		else
			point.x += game->ray.offset_h.x; 
	}
	return (point);
}

static t_fpos	get_collision(t_game *game, t_fpos start_h, t_fpos start_v)
{
	t_fpos	result_h;
	t_fpos	result_v;
	float	distance_h;
	float	distance_v;
	
	result_h = get_horizontal_collision(game, start_h);	
	result_v = get_vertical_collision(game, start_v);
	distance_h = fabs((game->player.px_x - result_h.x) - (game->player.px_y - result_h.y));
	distance_v = fabs((game->player.px_x - result_v.x) - (game->player.px_y - result_v.y));
	if (distance_h < distance_v)
	{
		put_point(game, result_h.x, result_h.y, RED);
		return (result_h);
	}
	else
	{
		put_point(game, result_v.x, result_v.y, BLUE);
		return (result_v);
	}
}

static	t_fpos	init_start_v(t_game *game)
{
	t_fpos	start;

	if (game->ray.to_left)
	{
		start.x = game->player.px_x - (game->player.map_x * TILE);
		start.y = game->player.px_y + start.x * tan(game->ray.angle);
		start.x = game->player.px_x - start.x;		
	}
	else
	{
		start.x = game->player.map_x * TILE + TILE - game->player.px_x;
		start.y = game->player.px_y - start.x * tan(game->ray.angle);
		start.x = game->player.px_x + start.x;
	}
	return (start);
}

static	t_fpos	init_start_h(t_game *game)
{
	t_fpos	start;

	if (game->ray.to_up)
	{
		start.y = game->player.px_y - (game->player.map_y * TILE);
		start.x = game->player.px_x - start.y / -tan(game->ray.angle);
		start.y = game->player.px_y - start.y;
	}
	else
	{
		start.y = game->player.map_y * TILE + TILE - game->player.px_y;
		start.x = game->player.px_x + start.y / -tan(game->ray.angle) ;
		start.y = game->player.px_y + start.y;		
	}
	return (start);
}

void	ray_casting(t_game *game)
{
	t_fpos	start_h;
	t_fpos	start_v;
	
	/* temp */
	game->ray.angle = game->player.angle;
	game->ray.to_up = (game->ray.angle >= PI_0 && game->ray.angle < M_PI);
	game->ray.to_left = (game->ray.angle >= PI_90 && game->ray.angle < PI_270);
	/**/

	start_h = init_start_h(game);
	if (start_h.x < MAX_MINIMAP)
		game->ray.offset_h.x = fabs(TILE / tan(game->ray.angle));
	game->ray.offset_h.y = TILE;
	start_v = init_start_v(game);
	game->ray.offset_v.y = fabs(TILE * tan(game->ray.angle));
	game->ray.offset_v.x = TILE;
	game->ray.px = get_collision(game, start_h, start_v);	
}
