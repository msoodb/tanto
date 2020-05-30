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

int main()
{
	char *json_file = "str.txt";
	char *stream = NULL;

	stream = tjson_read_file(json_file);
	printf("%s\n", stream);

	int status = 0; // 1 IN 0 OUT
	
	char c;
	while (*stream != '\0'){
		c = *stream;

		if (status == 0 && isspace(c)) {
			stream++;
			continue;
		}
		
		switch (c) {
		case '"':
			status = (status == 1) ? 0 : 1;
			break;		
		default:
			break;
		}

		printf("%c", c);
		stream++;
	}

	//printf("%s\n", stream);
	printf("\n");
	return 0;
}
