#ifndef MATH_HELPER
#define MATH_HELPER

#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>

class MathHelper {
public:
	// This is a column-order matrix multiply of matrices m1 and m2.
	static void matrixMult4x4 (GLfloat* result, GLfloat* m1, GLfloat* m2);
	static void matrixMult4x4Column (GLfloat* result, GLfloat* m1, GLfloat* m2);
	// Makes an identity matrix
	static void makeIdentity (GLfloat* result);
	
	// Returns a matrix that translates by x, y and z amount
	static void makeTranslate (GLfloat* result, GLfloat x, GLfloat y, GLfloat z);
	// Updates result to include a translate
	static void translateMatrixBy (GLfloat* result, GLfloat x, GLfloat y, GLfloat z);

	// Returns matrices that rotate about the X, Y and Z axes by a rotation amount (radians)
	static void makeRotateX (GLfloat* result, GLfloat rotation);
	static void makeRotateY (GLfloat* result, GLfloat rotation);
	static void makeRotateZ (GLfloat* result, GLfloat rotation);

	// 1) Fill in the "makeScale" prototype. Note: this should scale across all three axes
	static void makeScale (GLfloat* result, GLfloat x, GLfloat y, GLfloat z);
	
	// Copies the src matrix into the dest matrix
	static void copyMatrix (GLfloat* src, GLfloat* dest);
	static void print4x4Matrix(GLfloat* mat);

private:
};


#endif