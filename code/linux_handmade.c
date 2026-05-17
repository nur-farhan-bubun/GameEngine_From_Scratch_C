#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

int main(int argc, char **argv)
{
    Display *Display = XOpenDisplay(NULL);
    if(Display == NULL)
    {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int Screen = DefaultScreen(Display);
    Window Root = RootWindow(Display, Screen);

    int WindowWidth = 800;
    int WindowHeight = 600;

    Window Window = XCreateSimpleWindow(Display, Root, 
                                        10, 10, WindowWidth, WindowHeight, 1,
                                        BlackPixel(Display, Screen), 
                                        WhitePixel(Display, Screen));

    XSelectInput(Display, Window, ExposureMask | KeyPressMask | StructureNotifyMask);
    XMapWindow(Display, Window);

   
    Atom WM_DELETE_WINDOW = XInternAtom(Display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(Display, Window, &WM_DELETE_WINDOW, 1);

    printf("Handmade Hero Linux Platform Layer Started\n");

    int Running = 1;
    while(Running)
    {
        XEvent Event;
        XNextEvent(Display, &Event);


        
        if (Event.type == Expose) {
        
            GC gc = XCreateGC(Display, Window, 0, NULL);

          
            Colormap colormap = DefaultColormap(Display, Screen);
            XColor blue_color;
            static char *old_blue_color = "#000000";
            XParseColor(Display, colormap,old_blue_color, &blue_color);
            XAllocColor(Display, colormap, &blue_color);

           
            XSetForeground(Display, gc, blue_color.pixel);

           
            XFillRectangle(Display, Window, gc, 150, 120, 200, 150);

           
            XFreeGC(Display, gc);
        }

       
        if (Event.type == KeyPress) {
            break;
        }

     
        if (Event.type == ClientMessage) {
            if ((Atom)Event.xclient.data.l[0] == WM_DELETE_WINDOW) {
                break;
            }
        }
       
    }

    XCloseDisplay(Display);
    printf("Platform Layer Shutting Down\n");

    return 0;
}
