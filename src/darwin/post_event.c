/* libUIOHook: Cross-platfrom userland keyboard and mouse hooking.
 * Copyright (C) 2006-2014 Alexander Barker.  All Rights Received.
 * https://github.com/kwhat/libuiohook/
 *
 * libUIOHook is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libUIOHook is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <ApplicationServices/ApplicationServices.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <uiohook.h>

static unsigned int button_mask_table[10] = {
	kCGEventLeftMouseDown,	// Button 1
	kCGEventRightMouseDown,	// Button 2
	kCGEventOtherMouseDown,	// Button 3
	kCGEventOtherMouseDown,	// Button 4
	kCGEventOtherMouseDown,	// Button 5
	
	kCGEventLeftMouseUp,	// Button 1
	kCGEventRightMouseUp,	// Button 2
	kCGEventOtherMouseUp,	// Button 3
	kCGEventOtherMouseUp,	// Button 4
	kCGEventOtherMouseUp,	// Button 5
};


UIOHOOK_API void hook_post_event(virtual_event * const event) {
	CGEventRef cg_event = NULL;
	
	switch (event->type) {
		case EVENT_KEY_TYPED:

		case EVENT_KEY_PRESSED:
			cg_event = CGEventCreateKeyboardEvent(NULL, 
					(CGKeyCode) scancode_to_keycode(event->data.keyboard.keycode), 
					true);
			CGEventSetFlags(cg_event, (CGEventFlags) 0x00);
			
			if (event->type == EVENT_KEY_PRESSED) {
				break;
			}
			
		case EVENT_KEY_RELEASED:
			cg_event = CGEventCreateKeyboardEvent(NULL, 
					(CGKeyCode) scancode_to_keycode(event->data.keyboard.keycode), 
					true);
			CGEventSetFlags(cg_event, (CGEventFlags) 0x00);
			break;


		case EVENT_MOUSE_CLICKED:
			
		case EVENT_MOUSE_PRESSED:
			CGEventCreateMouseEvent(NULL, 
					button_mask_table[event->data.mouse.button - 1],
					CGPointMake(
						(CGFloat) event->data.mouse.x,
						(CGFloat) event->data.mouse.y
					),
					event->data.mouse.button - 1
			);
			
			if (event->type == EVENT_MOUSE_PRESSED) {
				break;
			}
		case EVENT_MOUSE_RELEASED:
			CGEventCreateMouseEvent(NULL, 
					button_mask_table[event->data.mouse.button - 1],
					CGPointMake(
						(CGFloat) event->data.mouse.x,
						(CGFloat) event->data.mouse.y
					),
					event->data.mouse.button - 1
			);
			break;
			
		case EVENT_MOUSE_MOVED:
			CGEventCreateMouseEvent(NULL, 
					kCGEventMouseMoved,
					CGPointMake(
						(CGFloat) event->data.mouse.x,
						(CGFloat) event->data.mouse.y
					),
					0
			);
			break;
		
		case EVENT_MOUSE_DRAGGED:
			//kCGEventLeftMouseDragged
			//kCGEventRightMouseDragged
			//kCGEventOtherMouseDragged
			break;
					
		case EVENT_MOUSE_WHEEL:
			/*
			CGEventCreateScrollWheelEvent(NULL, 
					CGScrollEventUnit units,
					CGWheelCount wheelCount,
					int32_t wheel1)
			*/
			break;
			
		default:
		break;
	}
	
	CGEventSetFlags(cg_event, (CGEventFlags) 0x00);
	
	CFRelease(cg_event); 
}
