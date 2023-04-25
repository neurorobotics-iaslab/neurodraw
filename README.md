# NeuroDraw

C++ graphic library based on SDL and OpenGL to create simple feedbacks for neuroapplications. The library allows to create simple shapes (e.g., line, triangle, rectangle, circle, arc, ring) and efficiently manipulate them through the provided class functions (e.g., move, rotate, set_color, etc...). Furthermore, the library provides a class to handle Window object and to set callbacks according to external events (e.g., key pressed, mouse motion, etc...). Finally, the library also implements a multi-thread engine to automatically draw the shapes on the window.
For convenience, an additional library (neurochrono) is provided to manage timings (this library will be soon separated). 

## Dependencies
To date, the library depends on the old SDL 1.2 library. To install it:
```
sudo apt-get install libsdl1.2-dev
```

## Installation
The library can be installed using cmake. Clone the repository:

```
git clone https://github.com/neurorobotics-iaslab/neurodraw.git
```
Install the neurodraw library:
```
cd neurodraw/build
cmake ..
make
sudo make install
```
Optionally, the library can be installed in a different directory:
```
cd neurodraw/build
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/Local
make
make install
```

## Library interface

### Class Window
The class manages the creation, destruction and properties of the displayed window. It also handles the user-defined callbacks. 

#### Window geometry
The window is created in order to have the following characteristics:
**The origin (0,0) of the frame is always the the center of the window. The smallest dimension of the window is equal to 2 in the drawing  coordinates, so  that a draw made in the box between (-1,-1) and (1,1) always fits in the window.**

The main class member functions provided are:

```cpp
// Create window with the given title
Window(const std::string& caption = "untitled");

// Open the window
bool open(void);

// Close the window
void close(void);

// Clear the window
void clear(void);

// Update the window
void update(void);

// Check if the window is opened
bool is_open(void);

// Set window background color
void set_color(Color color);

// Get current window size
void size(unsigned int* width, unsigned int* height);

// Process the window events and call the related callback (if defined)
void process_events(void);
		
// Register callback associated to a quit event 
void on_quit(std::function<void(void)> func);

// Register callback associate to a redraw event
void on_redraw(std::function<void(void)> func);

// Register callback associated to a keyboard event
void on_keyboard(std::function<void(const KeyboardEvent& kevt)> func);

// Register callback associated to a mouse event
void on_mouse(std::function<void(const MouseEvent& mevt)> func);

```

The library provides a base class ``Shape`` that can be instanciated only as derived class. The main class member functions provided are:
```cpp
// Move the shape to the position (x, y)
void move(float x, float y);

// Increment the position of shape of (dx, dy) 
void relmove(float dx, float dy);

// Rotate the shape of an amount of degree (e.g., 180.0f)
void rotate(float deg);

// Rotate the shape with respect to arbitrary point (refx, refy).
// Last argument determines if the shape should hold the direction 
// while rotating with respect to the arbitrary point
void rotate(float deg, float refx, float refy, bool hold = false);

// Increment the rotation of the shape of the given degrees
void relrotate(float ddeg);

// Increment the rotation of the shape of the given degrees with 
// respect to arbitrary point (refx, refy). Last argument determines 
// if the shape should hold the direction while rotating with respect 
// to the arbitrary point. 
void relrotate(float ddeg, float refx, float refy, bool hold = false);

// Hide the shape
void hide(void);

// Show the shape
void show(void);

// Set the color of the shape
void set_color(Color color);

// Set the color of a given vertex of the shape
void set_color(Color color, unsigned int index);
		
// Set the alpha channel of the shape    
void set_alpha(float alpha);

// Set the alpha channel of a given vertex of the shape
void set_alpha(float alpha, unsigned int index);

```




The provided derived classes are:
```cpp 
Line::Line(float x1, float y1, float x2, float y2, Color color = Palette::white);
Triangle::Triangle(float x1, float y1, float x2, float y2, float x3, float y3, bool is_fille = false, Color color = Palette::white);
Rectangle::Rectangle(float width, float hight, bool is_filled = false, Color color = Palette::white);
Cross::Cross(float size, float thick, Color color = Palette::white);
Arrow::Arrow(float width, float height, float headwidth, float bodyheight, bool is_filled = false, Color color = Palette::white);
RegularShape::RegularShape(float radius, unsigned int nvertices, bool is_filled = false, Color color = Palette::white);
Circle::Circle(float radius, bool is_filled = false, Color color = Palette::white);
Arc::Arc(float radius, float thick, float angle, Color color = Palette::white);
Ring::Ring(float radius, float thick, Color color = Palette::white);
```

## Example of usage
In the following example, the multi-thread version of the library is exploited:

```cpp
#include <thread>
#include "neurodraw/Rectangle.h"
#include "neurodraw/Circle.h"
#include "neurodraw/Ring.h"
#include "neurodraw/Cross.h"
#include "neurodraw/Arrow.h"
#include "neurodraw/Triangle.h"
#include "neurodraw/Line.h"
#include "neurodraw/Arc.h"
#include "neurodraw/RegularShape.h"
#include "neurodraw/Window.h"
#include "neurodraw/Engine.h"
#include "neurodraw/EventKey.h"

// Shapes (global to be used in the callbacks)
neurodraw::Rectangle rect(0.05f, 0.05f, true, neurodraw::Palette::darkblue);
neurodraw::Circle circle(0.2f);
neurodraw::Ring ring(0.3f, 0.1f);
neurodraw::Cross cross(0.2f, 0.05f);
neurodraw::Arrow arrow(0.2f, 0.1f, true, neurodraw::Palette::darkviolet);
neurodraw::Triangle tri(0.1f, 0.2f, false);
neurodraw::Line line(0.0f, -0.05f, 0.0f, 0.05f, neurodraw::Palette::purple);
neurodraw::Arc arc(0.3f, 0.1f, M_PI / 2.0f, neurodraw::Palette::gold);
neurodraw::RegularShape regular(0.2f, 12, true, neurodraw::Palette::grey);

// Pointer to the engine (global to be used in the callbacks)
neurodraw::Engine* engineptr;


void callback_mouse(const neurodraw::MouseEvent& event) {

	unsigned int width, height;
	float min_x, max_x;

	engineptr->winsize(&width, &height);

	float x = float(event.x)/height;
	float y = float(event.y)/height;

	max_x = float(width)/float(height);

	float py = - (2.0f*y - 1.0f);
	float px = 2.0f*x - max_x;

	rect.move(px, py);
}

void callback_keyboard(const neurodraw::KeyboardEvent& event) {
	if(event.state == 1) {
		printf("Key pressed: %d\n", event.sym);
		 switch(event.sym) {
			 case neurodraw::EventKey::ESCAPE:
				 engineptr->quit();
				 break;
			 case neurodraw::EventKey::a:
				 rect.set_color(neurodraw::Palette::tomato);
				 break;
			 case neurodraw::EventKey::s:
				 rect.set_color(neurodraw::Palette::darkblue);
				 break;
			 case neurodraw::EventKey::d:
				 rect.set_color(neurodraw::Palette::darkgreen);
				 break;
			 case neurodraw::EventKey::z:
				 rect.set_alpha(0.0f);
				 break;
			 case neurodraw::EventKey::x:
				 rect.set_alpha(0.5f);
				 break;
			 case neurodraw::EventKey::c:
				 rect.set_alpha(1.0f);
				 break;
			 case neurodraw::EventKey::l:
				 circle.relrotate(45.0f, 0.0f, 0.0f);
				 break;
			 case neurodraw::EventKey::i:
				 line.relrotate(45.0f, 0.0f, 0.0f);
				 break;
		 }
	}
}

int main(int argc, char** argv) {

  // Instantiation of the engine
	neurodraw::Engine engine("Multithread example");
	engineptr = &engine;

  // Setting the callbacks for keyboard and mouse event
	engine.on_keyboard(callback_keyboard);
	engine.on_mouse(callback_mouse);

  // Adding the shapes to the engine
	engine.add(&rect);
	engine.add(&circle);
	engine.add(&ring);
	engine.add(&cross);
	engine.add(&arrow);
	engine.add(&tri);
	engine.add(&line);
	engine.add(&arc);
	engine.add(&regular);
		
  // Setup position of the shapes
	ring.move(0.5f, 0.5f);
	ring.set_color(neurodraw::Palette::blue);
	circle.move(-0.5f, -0.5f);
	cross.set_color(neurodraw::Palette::red);
	arrow.move(0.5f, -0.5f);
	tri.set_color(neurodraw::Palette::yellow);
	tri.move(-0.5f, 0.5f);
	line.move(-0.5f, 0.0f);
	arc.move(0.5f, -0.2f);
	regular.move(0.0f, -0.5f);
	
  // Main loop while the engine is running
	while(engine.is_running()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
```

