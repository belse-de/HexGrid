#include <string>
#include <iostream>
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
static std::unordered_map<Hexagon::Hex, glm::vec4> map;
static Hexagon::Layout map_layout;

// display callback for GLUT
void display(void);
void reshape(int w, int h);
void keyPressed(unsigned char key, int x, int y);
void specialKeysPressed(int key, int x, int y);
void mousePressed(int button, int state, int x, int y);

int main(int argc, char **argv) 
{
  // create window with glut
  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_ALPHA | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize(windowSize.x, windowSize.y);
  //glutInitWindowPosition(windowPosition.x, windowPosition.y);
  windowID = glutCreateWindow(title.c_str());
  
  // register callbacks
  // display
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  // input
  glutKeyboardFunc(keyPressed);
  glutSpecialFunc(specialKeysPressed);
  glutMouseFunc(mousePressed);  
# if 0
  glutMotionFunc(mouseMoved);
  glutCreateMenu(menu);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
# endif
  
  //Makes 3D drawing work when something is in front of something else
  glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glClearColor(0.0,0.0,0.0,0.0);
  



  // MAP init  
  srand(0); 
  map_layout = Hexagon::Layout(Hexagon::Layout::pointy, Hexagon::Point(1,1), Hexagon::Point(0,0));
  for (int q = -map_radius; q <= map_radius; q++) {
    int r1 = max(-map_radius, -q - map_radius);
    int r2 = min(map_radius, -q + map_radius);
    for (int r = r1; r <= r2; r++) {
      int s = -q-r;
      float c_r = 0.5f*q/map_radius+0.5f;
      float c_g = 0.5f*r/map_radius+0.5f;
      float c_b = 0.5f*s/map_radius+0.5f;
      float c_rand = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      map.insert(
        std::make_pair<Hexagon::Hex,glm::vec4>(
          Hexagon::Hex(q, r, s), 
          glm::vec4( c_r, c_g, c_b, c_rand)
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
  
  glLineWidth(1.0f);
  glColor3f(+0.5f, +0.5f, +0.5f);
  glutWireCube(1.0);
  
  for (int q = -map_radius; q <= map_radius; q++) {
    int r1 = max(-map_radius, -q - map_radius);
    int r2 = min(map_radius, -q + map_radius);
    for (int r = r1; r <= r2; r++) {
      int s = -q-r;
      Hexagon::Hex field = Hexagon::Hex(q, r, s);
      glm::vec4 colour = map[field];
      //Hexagon::Layout map_layout_loc = Hexagon::Layout(
      //      Hexagon::Layout::pointy, Hexagon::Point(1,1), Hexagon::Point(0,0));         
      vector<Hexagon::Point> corners = field.polygon_corners(map_layout);
            
      glColor3f(colour.x, colour.y, colour.z);
      glBegin(GL_POLYGON);
        for (auto p : corners)
        {
          glVertex3f(p.x, p.y, colour.w);
        }
      glEnd();
      
      glLineWidth(1.0f);
      glColor3f(+0.25f, +0.25f, +0.25f);
      glBegin(GL_LINE_LOOP);
        for (auto p : corners)
        {
          glVertex3f(p.x, p.y, colour.w);
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
      break;
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

//src: 2016-10-12 https://www.opengl.org/archives/resources/faq/technical/selection.htm#sele0010
//src: 2016-10-12 http://nehe.gamedev.net/article/using_gluunproject/16013/
glm::vec3 GetOGLPos(int x, int y)
{
    GLint viewport[4];         // Where The Viewport Values Will Be Stored (x,y,w,h)
    GLdouble modelview[16];    // Where The 16 Doubles Of The Modelview Matrix Are To Be Stored
    GLdouble projection[16];   // Where The 16 Doubles Of The Projection  Matrix Are To Be Stored
    GLfloat winX, winY, winZ;  // Holds Our X, Y and Z Coordinates
    GLdouble posX, posY, posZ; // Hold The Final Values
 
    glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); // Retrieve The Modelview Matrix
    glGetDoublev( GL_PROJECTION_MATRIX, projection ); // Retrieve The Projection Matrix
    glGetIntegerv( GL_VIEWPORT, viewport ); // Retrieves The Viewport Values (X, Y, Width, Height)
 
    winX = (float)x;
    winY = (float)viewport[3] - (float)y; // Subtract The Current Mouse Y Coordinate From The Screen Height.
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ ); // get winZ
 
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    
    /*
    gluUnProject( (GLdouble) x, (GLdouble) realy, 0.0, mvmatrix,
        projmatrix, viewport, &wx1, &wy1, &wz1 );
    gluUnProject( (GLdouble) x, (GLdouble) realy, 1.0, mvmatrix,
        projmatrix, viewport, &wx2, &wy2, &wz2 );
    */
 
    return glm::vec3(posX, posY, posZ);
}

void mousePressed(int button, int state, int x, int y)
{
  //3d Normalised Device Coordinates
  float ray_x = -1.0f + (2.0f * x) / windowSize.x ;
  float ray_y = +1.0f - (2.0f * y) / windowSize.y;
  float ray_z = +1.0f;
  glm::vec3 ray_nds = glm::vec3(ray_x, ray_y, ray_z);
  //4d Homogeneous Clip Coordinates
  glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);


  switch(button)
  {
    case GLUT_LEFT_BUTTON:
      if(state == GLUT_DOWN)
      {
        glm::vec3 target = GetOGLPos (x,y);
        Hexagon::Point target_map = Hexagon::Point(target.x, target.y);
        Hexagon::FractionalHex target_hex_f = target_map.hex( map_layout );
        Hexagon::Hex target_hex = target_hex_f.round2hex();

        map[target_hex].r = 1.f - map[target_hex].r ;
        map[target_hex].g = 1.f - map[target_hex].g ;
        map[target_hex].b = 1.f - map[target_hex].b ;
        glutPostRedisplay();
      } // else GLUT_UP
      break;
    case GLUT_MIDDLE_BUTTON:
    case GLUT_RIGHT_BUTTON:
    case 0x3: // scrole up   - is pressed and released alternatingly
    case 0x4: // scrole down - is pressed and released alternatingly
    default:
      printf("Mouse Key pressed @ (%5d|%5d): button: 0x%02X state: 0x%01X\n", x, y, button, state);
      break;
  }
}

/*
 GLUT_KEY_REPEAT_OFF:0x0000, GLUT_KEY_REPEAT_ON:0x0001, GLUT_KEY_REPEAT_DEFAULT:0x0002.

glut can also handle joysticks with the glutJoystickFunc which has the following constants: GLUT_HAS_JOYSTICK:0x0264, GLUT_OWNS_JOYSTICK:0x0265, GLUT_JOYSTICK_BUTTONS:0x0266, GLUT_JOYSTICK_AXES:0x0267, GLUT_JOYSTICK_POLL_RATE:0x0268, GLUT_JOYSTICK_BUTTON_A:0x0001, GLUT_JOYSTICK_BUTTON_B:0x0002, GLUT_JOYSTICK_BUTTON_C:0x0004, GLUT_JOYSTICK_BUTTON_D:0x0008.

*/

/*
you can use glutGetModifiers ( void ); to get the modifier key state when callbacks were generated (keyboard, mouse)

the return values are:
GLUT_ACTIVE_SHIFT
GLUT_ACTIVE_CTRL
GLUT_ACTIVE_ALT
*/
