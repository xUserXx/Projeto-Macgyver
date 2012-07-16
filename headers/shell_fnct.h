/***************************************************************************
* File: shell_fnct.h                                                       *
* Copyright 15/07/2012 UserX <userx@gmail.com>                             *
*                                                                          *
* This program is free software; you can redistribute it and/or modify     *
* it under the terms of the GNU General Public License as published by     *
* the Free Software Foundation; either version 2 of the License, or        *
* (at your option) any later version.                                      *
*                                                                          *
* This program is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
* GNU General Public License for more details.                             *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with this program; if not, write to the Free Software              *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,               *
* MA 02110-1301, USA.                                                      *
*                                                                          *
*                                                                          *
***************************************************************************/

// File:   shell_fnct :: 

#include <stdlib.h>
#ifndef SHELL_FNCT_H
#define SHELL_FNCT_H

extern void clear(void)
{
        printf("\e[H\e[2J");
}
 
extern void setcolor(int recv)
{
        int x=0;
        int y=0;
 
        if (recv>=0 && recv<=8)
        {
                x=recv+30;
        }
        if (recv>=10 && recv<=18)
        {
                        y=1;
                        x=recv+20;
         }
        printf("\33[%d;%dm",y,x);
}
 
extern void setfont(int bg, int fg)
{
        int x, y, z;
 
        z=y=0;
        if (bg>=0 && bg<=8)
                x=bg+40;
        if (fg>=0 && fg<=8)
                z=fg+30;
        else if (fg>=10 && fg<=18)
                {
                        y=1;
                        z=fg+20;
                }
 
        printf("\33[%d;%d;%dm",y,x,z);
}

void move(int x, int y)
{
	printf("\33[%d;%df",y,x);
} 

void wait(void){
	printf("\n");
	system("read b");
}

#endif
