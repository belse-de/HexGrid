#include <iostream>
#include <string>
#include <unordered_map>

#include <cstdlib>
#include <cmath>
using std::max;
using std::min;

//#include <GL/glut.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>// glm::vec2
#include <glm/ext.hpp>

#include "libhex.hpp"

static int windowID = -1;
// window size
static glm::vec2 windowSize = glm::vec2(600,600);

// initial window position
static glm::vec2 windowPosition = glm::vec2(100, 100);
static glm::vec3 cameraPosition = glm::vec3(1, 1, 2.5);
static glm::vec3 cameraUp       = glm::vec3(0, 1, 0);
// window title
static std::string title = "Hex Test";

static int map_radius = 20;
static std::unordered_map<Hexagon::Hex, glm::vec3> map;

// display callback for GLUT
void display(void);
void reshape(int w, int h);
void keyPressed(unsigned char key, int x, int y);
void specialKeysPressed(int key, int x, int y);

int main(int argc, char **argv) 
{
  // create window with glut
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize(windowSize.x, windowSize.y);
  glutInitWindowPosition(windowPosition.x, windowPosition.y);
  windowID = glutCreateWindow(title.c_str());
  
  //Makes 3D drawing work when something is in front of something else
  glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0,0.0,0.0,0.0);
  
  // register callbacks
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeysPressed);
# if 0
  glutMotionFunc(mouseMoved);
  glutMouseFunc(mousePressed);
  glutCreateMenu(menu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
# endif

  // MAP init 
  //  std::make_pair<std::string,double>("eggs",6.0)
  
  for (int q = -map_radius; q <= map_radius; q++) {
    int r1 = max(-map_radius, -q - map_radius);
    int r2 = min(map_radius, -q + map_radius);
    for (int r = r1; r <= r2; r++) {
      int s = -q-r;
      float c_r = 0.5f*q/map_radius+0.5f;
      float c_g = 0.5f*r/map_radius+0.5f;
      float c_b = 0.5f*s/map_radius+0.5f;
      map.insert(
        std::make_pair<Hexagon::Hex,glm::vec3>(
          Hexagon::Hex(q, r, s), glm::vec3( c_r, c_g, c_b)
        )
      );
    }
  } 

  //return on exit event to main(freeglut)
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
  // start GLUT event loop
  glutMainLoop();
  exit(EXIT_SUCCESS);
}

void display(void)
{
  
  
  // clear color and depth buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // switch to opengl modelview matrix
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  // position the camera at (0,0,cameraZ) looking down the
  // negative z-axis at (0,0,0)
  gluLookAt(
      cameraPosition.x, cameraPosition.y, cameraPosition.z, 
      0.0, 0.0, 0.0, 
      cameraUp.x, cameraUp.y, cameraUp.z);
	
	glColor3f(+1.0f, +0.0f, +0.0f);
  glBegin(GL_QUADS); //Begin quadrilateral coordinates
	  glVertex3f(+0.5f, +0.5f, +0.75f);
	  glVertex3f(-0.5f, +0.5f, +0.75f);
	  glVertex3f(-0.5f, -0.5f, +0.75f);
	  glVertex3f(+0.5f, -0.5f, +0.75f);
	glEnd(); //End quadrilateral coordinates
  
  
  glColor3f(+0.25f, +0.25f, +0.25f);
  glutSolidCube(1.0);
  
  glLineWidth(5.0f);
  glColor3f(+0.5f, +0.5f, +0.5f);
  glutWireCube(1.0);
  
  for (int q = -map_radius; q <= map_radius; q++) {
    int r1 = max(-map_radius, -q - map_radius);
    int r2 = min(map_radius, -q + map_radius);
    for (int r = r1; r <= r2; r++) {
      int s = -q-r;
      Hexagon::Hex field = Hexagon::Hex(q, r, s);
      glm::vec3 colour = map[field];

      Hexagon::Layout hexLayout = 
          Hexagon::Layout(
            Hexagon::Layout::pointy, 
            Hexagon::Point(1,1), 
            Hexagon::Point(0,0));
            
      vector<Hexagon::Point> corners = field.polygon_corners(hexLayout);
            
      glColor3f(colour.x, colour.y, colour.z);
      glBegin(GL_POLYGON);
        for (auto p : corners)
        {
          glVertex2f(p.x, p.y);
        }
      glEnd();
      
      glLineWidth(3.0f);
      glColor3f(+0.25f, +0.25f, +0.25f);
      glBegin(GL_LINE_STRIP);
        for (int i=0; i<4; i++)
        {
          auto p = corners[i];
          glVertex2f(p.x, p.y);
        }
      glEnd();
      glLineWidth(1.0f);
      
    }
  } 
  
  
  
 
  //display
  glutSwapBuffers();
}

// reshape-Callback for GLUT
void reshape(int w, int h)
{
  //reshape
  glViewport(0, 0, w, h);
  windowSize= glm::vec2(w,h);
  
  
  // reshaped window aspect ratio
  float aspect = (float) windowSize.x / (float) windowSize.y;

  // compute z position of camera
  GLfloat fov= 45.0; //The camera angle (field of view)
  //cameraPosition.z = (windowSize.y/100.0f/2.0f) / tan(fov/180.0f);
  // compute near and far plane
  GLfloat nearPlane = 0.125f;// abs(cameraPosition.z/10.0f);
  GLfloat farPlane  = 100.0f;//abs(cameraPosition.z*10.0f);
  
  
  // restore view definition after window reshape
  glMatrixMode(GL_PROJECTION);
  //Reset the camera position
  glLoadIdentity();
  // perspective projection
  gluPerspective(fov, aspect, nearPlane, farPlane);
}


// keyboard callback
void keyPressed(unsigned char key, int x, int y) 
{
  glm::vec2 mousePosition = glm::vec2(x, y);
  switch(key)
  {
    case '\e': // escape key
      glutDestroyWindow ( windowID );
      glutLeaveMainLoop();
      return;
      break;
    case 'p':
    case 'P':
      std::cout << "camera position: " << glm::to_string(cameraPosition) << std::endl;
    default:
      printf("Key pressed @ (%5d|%5d): 0x%02X %c \n", x, y, key, key);
      break;
  }
  glutPostRedisplay();
}

void specialKeysPressed(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_F1:
    case GLUT_KEY_F2:
    case GLUT_KEY_F3:
    case GLUT_KEY_F4:
    case GLUT_KEY_F5:
    case GLUT_KEY_F6:
    case GLUT_KEY_F7:
    case GLUT_KEY_F8:
    case GLUT_KEY_F9:
    case GLUT_KEY_F10:
    case GLUT_KEY_F11:
    case GLUT_KEY_F12:
      printf("Function Key pressed @ (%5d|%5d): 0x%04X \n", x, y, key);
      break;
      
    case GLUT_KEY_LEFT:
      cameraPosition.x -= 0.125f;
      break;
    case GLUT_KEY_UP:
      cameraPosition.y += 0.125f;
      break;
    case GLUT_KEY_RIGHT:
      cameraPosition.x += 0.125f;
      break;
    case GLUT_KEY_DOWN:
      cameraPosition.y -= 0.125f;
      break;
    case GLUT_KEY_PAGE_UP:
      cameraPosition.z += 0.125f;
      break;
    case GLUT_KEY_PAGE_DOWN:
      cameraPosition.z -= 0.125f;
      break;
    case GLUT_KEY_HOME:
      cameraPosition = glm::vec3(0, 0, 80);
      break;
      
    case GLUT_KEY_END:
    case GLUT_KEY_INSERT:
      printf("Direction Key pressed @ (%5d|%5d): 0x%04X \n", x, y, key);
      break;
    
    default:
      printf("Special Key pressed @ (%5d|%5d): 0x%04X \n", x, y, key);
      break;
  }
  glutPostRedisplay();
}

/*
 GLUT_KEY_REPEAT_OFF:0x0000, GLUT_KEY_REPEAT_ON:0x0001, GLUT_KEY_REPEAT_DEFAULT:0x0002.

mouse clicks can be handled with the glutMouseFunc and the constants associated with the mouse buttons are: GLUT_LEFT_BUTTON:0x0000, GLUT_MIDDLE_BUTTON:0x0001, GLUT_RIGHT_BUTTON:0x0002

glut can also handle joysticks with the glutJoystickFunc which has the following constants: GLUT_HAS_JOYSTICK:0x0264, GLUT_OWNS_JOYSTICK:0x0265, GLUT_JOYSTICK_BUTTONS:0x0266, GLUT_JOYSTICK_AXES:0x0267, GLUT_JOYSTICK_POLL_RATE:0x0268, GLUT_JOYSTICK_BUTTON_A:0x0001, GLUT_JOYSTICK_BUTTON_B:0x0002, GLUT_JOYSTICK_BUTTON_C:0x0004, GLUT_JOYSTICK_BUTTON_D:0x0008.

*/

/*
you can use glutGetModifiers ( void ); to get the modifier key state when callbacks were generated (keyboard, mouse)

the return values are:
GLUT_ACTIVE_SHIFT
GLUT_ACTIVE_CTRL
GLUT_ACTIVE_ALT
*/
