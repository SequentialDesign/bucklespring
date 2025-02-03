#include <stdio.h>

#include <X11/XKBlib.h>
#include <X11/extensions/record.h>

#include "buckle.h"

#define initial_button_hex 0xf0


void key_pressed_cb(XPointer arg, XRecordInterceptData *d);

int scan(int verbose)
{
	/* Initialize and start Xrecord context */
	
	XRecordRange* rr;
	XRecordClientSpec rcs;
	XRecordContext rc;

	printd("Opening Xrecord context");

	Display *dpy = XOpenDisplay(NULL);
	if(dpy == NULL) {
		fprintf(stderr, "Unable to open display\n");
		return -1;
	}
    
	rr = XRecordAllocRange ();
	if(rr == NULL) {
		fprintf(stderr, "XRecordAllocRange error\n");
		return -1;
	}

	rr->device_events.first = KeyPress;
	rr->device_events.last = ButtonReleaseMask;
	rcs = XRecordAllClients;

	rc = XRecordCreateContext (dpy, 0, &rcs, 1, &rr, 1);
	if(rc == 0) {
		fprintf(stderr, "XRecordCreateContext error\n");
		return -1;
	}

	XFree (rr);

	if(XRecordEnableContext(dpy, rc, key_pressed_cb, NULL) == 0) {
		fprintf(stderr, "XRecordEnableContext error\n");
		return -1;
	}

	/* We never get here */

	return 0;
}


/*
 * Xrecord event callback
 */

void key_pressed_cb(XPointer arg, XRecordInterceptData *d) 
{
    if (d->category != XRecordFromServer)
        return;
    
    int key = ((unsigned char*) d->data)[1];
    int type = ((unsigned char*) d->data)[0] & 0x7F;
    int repeat = d->data[2] & 1;
    
    key -= 8; /* X code to scan code? */
    
    if(!repeat) {
        switch (type) {
        case KeyPress:
            play(key, 1);
            break;
        case KeyRelease:
            play(key, 0);
            break;
        case ButtonPress:
            play(key + 7 + initial_button_hex, 1);
            /*switch(key) {
            case -7: // left-click button 1
                play(0xf0, 1);
                break;
            case -6: // middle-click button 2
                play(0xf1, 1);
                break;
            case -5: // right-click button 3
                play(0xf2, 1);
                break;
            case -4: // scroll up button 4
                play(0xf3, 1);
                break;
            case -3: // scroll down button 5
                play(0xf4, 1);
                break;
            case -2: // button 6 horizontal scroll left
                play(0xf5, 1);
                break;
            case -1: // button 7 horizontal scroll right
                play(0xf6, 1);
                break;
            case 0: // button 8 back
                play(0xf7, 1);
                break;
            case 1: // button 9 forward
                play(0xf8, 1);
                break;
            case 2: // button 10 high
                play(0xf9, 1);
                break;
            case 3: // button 11 low
                play(0xfa, 1);
                break;
            case 4: // button 12 ring
                play(0xfb, 1);
                break;
            default:
                break;
            }*/
            break;
        case ButtonRelease:
            play(key + 7 + initial_button_hex, 1);
            /*switch(key) {
            case -7: // left-click button 1
                play(0xf0, 0);
                break;
            case -6: // middle-click button 2
                play(0xf1, 0);
                break;
            case -5: // right-click button 3
                play(0xf2, 0);
                break;
            case -4: // scroll up button 4
                break;
            case -3: // scroll down button 5
                break;
            case -2: // button 6 horizontal scroll left
                play(0xf5, 0);
                break;
            case -1: // button 7 horizontal scroll right
                play(0xf6, 0);
                break;
            case 0: // button 8 back
                play(0xf7, 0);
                break;
            case 1: // button 9 forward
                play(0xf8, 0);
                break;
            case 2: // button 10 high
                play(0xf9, 0);
                break;
            case 3: // button 11 low
                play(0xfa, 0);
                break;
            case 4: // button 12 ring
                play(0xfb, 0);
                break;
            default:
                break;
            }*/
            break;
        default:
            break;
        }
    }
    
    XRecordFreeData (d);
}


void open_console(void)
{
}
