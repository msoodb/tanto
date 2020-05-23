/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../tanto.h"

/* 
 * create json, add field, and print 
*/
void main()
{
	TJSON_t *json = NULL;
	
	TJSON_t *node1 = tanto_init_node("hello", "world!", 1);
	TJSON_t *node2 = tanto_init_node("parser", "tanto", 1);
	tanto_push(&json, node1);
	tanto_push(&json, node2);
		
	tanto_write_file("hello_world.json", json);
}
