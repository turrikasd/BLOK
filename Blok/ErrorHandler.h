#pragma once

#include "GL\glew.h"
#include "SDL.h"
#include <stdio.h>

void PrintGLError();
void sdldie(const char *msg);
void checkSDLError(int line);