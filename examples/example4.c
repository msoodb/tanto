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
 * find node from key
*/
void main()
{
	char *stream;
	stream = tanto_read_file("../json/package-min.json");

	TJSON_t *json = NULL;
	TANTO_INIT(&json);

	tanto_parse(&json, stream);
	tanto_print(json);

	//TJSON_t *node = NULL;
	//node = tanto_find(json, "@vue/cli-service");
	//if (node != NULL) {
	//	printf("%s\n", node->value);
	//}
}
