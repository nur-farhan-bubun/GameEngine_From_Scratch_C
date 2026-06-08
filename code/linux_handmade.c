#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdint.h>



typedef struct {
XImage *Image;
uint32_t *Pixels;
int Width;
int Height;
}Backbuffer;

Backbuffer globalbackbuffer={0};

void ResizeTheWindow(Backbuffer *Buffer, Display *display, int screen, int newWidth, int newHeight)
{
    globalbackbuffer.Width = newWidth;
    globalbackbuffer.Height = newHeight;

    int bytes_per_pixel = 4;
    int buffer_size = newWidth * newHeight * bytes_per_pixel;
    if (globalbackbuffer.Pixels) {
        free(globalbackbuffer.Pixels);
    }
    globalbackbuffer.Pixels = (uint32_t *)calloc(1, buffer_size);

    Visual *visual = DefaultVisual(display, screen);
    int depth = DefaultDepth(display, screen);
    globalbackbuffer.Image = XCreateImage(display, visual, depth, ZPixmap, 0,
                                          (char *)globalbackbuffer.Pixels,
                                          newWidth, newHeight, 32, 0);
}

void RenderTheScreen(Backbuffer *buffer, int x_offset, int y_offset)
{
    uint32_t *row = buffer->Pixels;

    for(int y = 0; y < buffer->Height; ++y)
    {
        uint32_t *pixel = row;
        for(int x = 0; x < buffer->Width; ++x)
        {
            uint8_t blue = (x + x_offset);
            uint8_t green = (y + y_offset);
            
            *pixel++ = ((green << 8) | blue);
        }           
        row += buffer->Width;
    }
}
void DrawRectangle(Backbuffer *buffer, int min_x, int min_y, int max_x, int max_y, uint32_t color)
{
    if (min_x < 0) min_x = 0;
    if (min_y < 0) min_y = 0;
    if (max_x > buffer->Width) max_x = buffer->Width;
    if (max_y > buffer->Height) max_y = buffer->Height;

    for (int y = min_y; y < max_y; ++y)
    {
        uint32_t *pixel = buffer->Pixels + (y * buffer->Width) + min_x;
        for (int x = min_x; x < max_x; ++x)
        {
            *pixel++ = color;
        }
    }
}
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

   XSetWindowBackgroundPixmap(Display, Window, None);
    Atom WM_DELETE_WINDOW = XInternAtom(Display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(Display, Window, &WM_DELETE_WINDOW, 1);

    printf("Handmade Hero Linux Platform Layer Started\n");

 ResizeTheWindow(&globalbackbuffer, Display, Screen, WindowWidth, WindowHeight);
    int Running = 1;
    int x_offset = 0;
    int y_offset = 0;
 GC gc = XCreateGC(Display, Window, 0, NULL);
    while(Running)
    {
         while(XPending(Display)){

        XEvent Event;
        XNextEvent(Display, &Event);

 if(Event.type == ConfigureNotify){

     XConfigureEvent xConfigureEvent=Event.xconfigure;
    if(xConfigureEvent.width!=WindowWidth||xConfigureEvent.height!=WindowHeight ){
    int window_width = xConfigureEvent.width;
    int window_height=xConfigureEvent.height; 
    ResizeTheWindow(&globalbackbuffer, Display, Screen, window_width, window_height);    


     }

 }
{
    
}
        
        if (Event.type == Expose) {
        
           

          
            Colormap colormap = DefaultColormap(Display, Screen);
            XColor blue_color;
            static char *old_blue_color = "#000000";
            XParseColor(Display, colormap,old_blue_color, &blue_color);
            XAllocColor(Display, colormap, &blue_color);

           
            XSetForeground(Display, gc, blue_color.pixel);

           
            XFillRectangle(Display, Window, gc, 150, 120, 200, 150);

           
            
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

         x_offset++;
     y_offset++;
     RenderTheScreen(&globalbackbuffer,x_offset,y_offset);
DrawRectangle(&globalbackbuffer, 150, 120, 150 + 200, 120 + 150, 0x00000000);
     if (globalbackbuffer.Image) 
        {
            XPutImage(Display, Window, gc, globalbackbuffer.Image,
                      0, 0, 0, 0, 
                      globalbackbuffer.Width, globalbackbuffer.Height);
        }
    }
   
    XCloseDisplay(Display);
    printf("Platform Layer Shutting Down\n");

    return 0;
}
