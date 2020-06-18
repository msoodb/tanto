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
	char *json_file = "simple.json";
	char *stream = NULL;

	tjson_read_file(json_file, &stream);

	TJSON_t *json = NULL;
	TJSON_INIT(&json);

	int error;
	if ((error = tjson_parse(&json, &stream)) > 0)
		printf("%s %s %s: %d\n", "ERROR! while parsing file", json_file, "line", error);
	else 
		tjson_print(json);

	
	if (json != NULL) {
		tjson_erase(&json);
		json = NULL;
	}
	if (stream != NULL) {
		free(stream);
		stream = NULL;
	}

	return 0;
}
