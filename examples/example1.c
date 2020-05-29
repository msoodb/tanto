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
 * read jsno file, parse and print 
*/
int main()
{
	char *stream = NULL;
	stream = tjson_read_file("../json/simple.json");

	
	TJSON_t *json = NULL;
	TJSON_INIT(&json);

	if (tjson_parse(&json, stream) < 0){
		printf("%s\n", "Error! while parsing file");
	}
	else{
		tjson_print(json);
	}

	if (json != NULL) tjson_erase(&json);
	if (stream != NULL) free(stream);

	return 0;
}
