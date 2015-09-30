####
	#makefile for rblocks.
	#Copyright (C) 2015 Adam Richardson

	#This program is free software: you can redistribute it and/or modify
	#it under the terms of the GNU General Public License as published by
	#the Free Software Foundation, either version 3 of the License, or
	#(at your option) any later version.

	#This program is distributed in the hope that it will be useful,
	#but WITHOUT ANY WARRANTY; without even the implied warranty of
	#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	#GNU General Public License for more details.

	#You should have received a copy of the GNU General Public License
	#along with this program.  If not, see <http://www.gnu.org/licenses/>.
####

CC = gcc
SOURCE = main.c parse.c
OBJECTS = $(SOURCE:.c=.o)
OUTPUT = aae
CFLAGS = -Wall -Wextra -Werror -O4 -std=c99 -pedantic -ggdb `sdl2-config --cflags --libs`

.PHONY: all
.PHONY: clean

all: $(OUTPUT)

$(OUTPUT): $(OBJECTS)
	$(CC) -o $(OUTPUT) $(OBJECTS) $(CFLAGS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

clean:
	-rm $(OUTPUT) $(OBJECTS)
