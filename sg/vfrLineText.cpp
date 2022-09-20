//
// vfrLineText
//   Line Stroke Text Object Class
//
#include "vfrLineText.h"

#define MAX_STROKES   256
#define END_OF_LIST   0
#define FONT_BEGIN    1
#define FONT_NEXT     2
#define FONT_END      3
#define FONT_ADVANCE  4
#define LINE_STROKE_SCALE  0.1f


GLfloat vfrLineText::_fontSize[2] = {0.8f, 0.9f};

const GLint lineFont[][1+MAX_STROKES*3] = {
 {
 32,
 FONT_ADVANCE, 6, 0
 },
 {
 33,
 FONT_BEGIN, 2, 1,
 FONT_END, 2, 2,
 FONT_BEGIN, 2, 4,
 FONT_END, 2, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 34,
 FONT_BEGIN, 1, 7,
 FONT_END, 1, 9,
 FONT_BEGIN, 3, 9,
 FONT_END, 3, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 35,
 FONT_BEGIN, 1, 2,
 FONT_END, 1, 7,
 FONT_BEGIN, 3, 7,
 FONT_END, 3, 2,
 FONT_BEGIN, 4, 3,
 FONT_END, 0, 3,
 FONT_BEGIN, 0, 6,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 36,
 FONT_BEGIN, 2, 1,
 FONT_END, 2, 9,
 FONT_BEGIN, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 1, 5,
 FONT_NEXT, 3, 5,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 4, 2,
 FONT_NEXT, 3, 1,
 FONT_NEXT, 1, 1,
 FONT_END, 0, 2,
 FONT_ADVANCE, 6, -1
 },
 {
 37,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 4, 7,
 FONT_END, 4, 8,
 FONT_BEGIN, 1, 8,
 FONT_NEXT, 0, 8,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 1, 7,
 FONT_END, 1, 8,
 FONT_BEGIN, 4, 3,
 FONT_NEXT, 5, 3,
 FONT_NEXT, 5, 2,
 FONT_NEXT, 4, 2,
 FONT_END, 5, 2,
 FONT_ADVANCE, 8, 1
 },
 {
 38,
 FONT_BEGIN, 4, 4,
 FONT_NEXT, 2, 2,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 0, 4,
 FONT_NEXT, 2, 6,
 FONT_NEXT, 2, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 0, 6,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 39,
 FONT_BEGIN, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 1, 9,
 FONT_NEXT, 2, 9,
 FONT_NEXT, 2, 8,
 FONT_END, 1, 8,
 FONT_ADVANCE, 6, 1
 },
 {
 40,
 FONT_BEGIN, 4, 2,
 FONT_NEXT, 2, 4,
 FONT_NEXT, 2, 6,
 FONT_END, 4, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 41,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 2, 4,
 FONT_NEXT, 2, 6,
 FONT_END, 0, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 42,
 FONT_BEGIN, 2, 2,
 FONT_END, 2, 8,
 FONT_BEGIN, 0, 7,
 FONT_END, 4, 3,
 FONT_BEGIN, 4, 5,
 FONT_END, 0, 5,
 FONT_BEGIN, 0, 3,
 FONT_END, 4, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 43,
 FONT_BEGIN, 2, 3,
 FONT_END, 2, 7,
 FONT_BEGIN, 0, 5,
 FONT_END, 4, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 44,
 FONT_BEGIN, 0, 1,
 FONT_NEXT, 1, 2,
 FONT_END, 1, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 45,
 FONT_BEGIN, 0, 5,
 FONT_END, 4, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 46,
 FONT_BEGIN, 1, 2,
 FONT_END, 2, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 47,
 FONT_BEGIN, 0, 3,
 FONT_END, 4, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 48,
 FONT_BEGIN, 0, 3,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_END, 4, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 49,
 FONT_BEGIN, 1, 2,
 FONT_END, 3, 2,
 FONT_BEGIN, 2, 2,
 FONT_NEXT, 2, 8,
 FONT_END, 1, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 50,
 FONT_BEGIN, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 4, 6,
 FONT_NEXT, 0, 2,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 51,
 FONT_BEGIN, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 3, 5,
 FONT_END, 2, 5,
 FONT_BEGIN, 3, 5,
 FONT_NEXT, 4, 6,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 1, 8,
 FONT_END, 0, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 52,
 FONT_BEGIN, 3, 2,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 0, 4,
 FONT_END, 4, 4,
 FONT_ADVANCE, 6, 0
 },
 {
 53,
 FONT_BEGIN, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 4, 5,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 0, 8,
 FONT_END, 4, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 54,
 FONT_BEGIN, 0, 5,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 3, 5,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 2, 8,
 FONT_END, 3, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 55,
 FONT_BEGIN, 0, 8,
 FONT_NEXT, 4, 8,
 FONT_NEXT, 2, 4,
 FONT_END, 2, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 56,
 FONT_BEGIN, 1, 5,
 FONT_NEXT, 0, 4,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 3, 5,
 FONT_NEXT, 1, 5,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 4, 6,
 FONT_END, 3, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 57,
 FONT_BEGIN, 1, 2,
 FONT_NEXT, 2, 2,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 1, 5,
 FONT_END, 4, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 58,
 FONT_BEGIN, 0, 3,
 FONT_END, 1, 3,
 FONT_BEGIN, 1, 6,
 FONT_END, 0, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 59,
 FONT_BEGIN, 0, 1,
 FONT_NEXT, 1, 2,
 FONT_END, 1, 3,
 FONT_BEGIN, 1, 6,
 FONT_END, 0, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 60,
 FONT_BEGIN, 3, 3,
 FONT_NEXT, 1, 5,
 FONT_END, 3, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 61,
 FONT_BEGIN, 0, 4,
 FONT_END, 4, 4,
 FONT_BEGIN, 4, 6,
 FONT_END, 0, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 62,
 FONT_BEGIN, 1, 7,
 FONT_NEXT, 3, 5,
 FONT_END, 1, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 63,
 FONT_BEGIN, 1, 2,
 FONT_END, 2, 2,
 FONT_BEGIN, 2, 4,
 FONT_NEXT, 2, 5,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 1, 8,
 FONT_END, 0, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 64,
 FONT_BEGIN, 3, 2,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 2, 4,
 FONT_NEXT, 2, 6,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 65,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_END, 4, 2,
 FONT_BEGIN, 0, 5,
 FONT_END, 4, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 66,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 3, 5,
 FONT_END, 0, 5,
 FONT_BEGIN, 3, 5,
 FONT_NEXT, 4, 6,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 0, 8,
 FONT_END, 0, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 67,
 FONT_BEGIN, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_END, 4, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 68,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_END, 0, 8,
 FONT_BEGIN, 1, 8,
 FONT_END, 1, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 69,
 FONT_BEGIN, 4, 2,
 FONT_NEXT, 0, 2,
 FONT_NEXT, 0, 8,
 FONT_END, 4, 8,
 FONT_BEGIN, 3, 5,
 FONT_END, 0, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 70,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 8,
 FONT_END, 4, 8,
 FONT_BEGIN, 3, 5,
 FONT_END, 0, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 71,
 FONT_BEGIN, 3, 4,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 3, 8,
 FONT_END, 4, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 72,
 FONT_BEGIN, 0, 2,
 FONT_END, 0, 8,
 FONT_BEGIN, 0, 5,
 FONT_END, 4, 5,
 FONT_BEGIN, 4, 8,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 73,
 FONT_BEGIN, 1, 2,
 FONT_END, 3, 2,
 FONT_BEGIN, 2, 2,
 FONT_END, 2, 8,
 FONT_BEGIN, 1, 8,
 FONT_END, 3, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 74,
 FONT_BEGIN, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_END, 4, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 75,
 FONT_BEGIN, 0, 2,
 FONT_END, 0, 8,
 FONT_BEGIN, 4, 8,
 FONT_NEXT, 1, 5,
 FONT_END, 4, 2,
 FONT_ADVANCE, 8, 0
 },
 {
 76,
 FONT_BEGIN, 0, 8,
 FONT_NEXT, 0, 2,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 77,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 8,
 FONT_NEXT, 2, 6,
 FONT_NEXT, 4, 8,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 78,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 8,
 FONT_NEXT, 4, 2,
 FONT_END, 4, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 79,
 FONT_BEGIN, 0, 3,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 1, 2,
 FONT_END, 0, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 80,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 4, 6,
 FONT_NEXT, 3, 5,
 FONT_END, 0, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 81,
 FONT_BEGIN, 0, 3,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 2, 2,
 FONT_NEXT, 1, 2,
 FONT_END, 0, 3,
 FONT_BEGIN, 2, 5,
 FONT_END, 4, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 82,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 8,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 4, 7,
 FONT_NEXT, 4, 6,
 FONT_NEXT, 3, 5,
 FONT_END, 0, 5,
 FONT_BEGIN, 1, 5,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 83,
 FONT_BEGIN, 4, 7,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 0, 7,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 1, 5,
 FONT_NEXT, 3, 5,
 FONT_NEXT, 4, 4,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 1, 2,
 FONT_END, 0, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 84,
 FONT_BEGIN, 0, 8,
 FONT_END, 4, 8,
 FONT_BEGIN, 2, 8,
 FONT_END, 2, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 85,
 FONT_BEGIN, 0, 3,
 FONT_END, 0, 8,
 FONT_BEGIN, 4, 8,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 1, 2,
 FONT_END, 0, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 86,
 FONT_BEGIN, 0, 8,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 2, 2,
 FONT_NEXT, 4, 5,
 FONT_END, 4, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 87,
 FONT_BEGIN, 0, 2,
 FONT_END, 0, 8,
 FONT_BEGIN, 4, 8,
 FONT_NEXT, 4, 2,
 FONT_NEXT, 2, 4,
 FONT_END, 0, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 88,
 FONT_BEGIN, 0, 2,
 FONT_END, 4, 8,
 FONT_BEGIN, 0, 8,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 89,
 FONT_BEGIN, 2, 2,
 FONT_NEXT, 2, 5,
 FONT_END, 0, 8,
 FONT_BEGIN, 4, 8,
 FONT_END, 2, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 90,
 FONT_BEGIN, 0, 8,
 FONT_NEXT, 4, 8,
 FONT_NEXT, 0, 2,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 91,
 FONT_BEGIN, 3, 1,
 FONT_NEXT, 1, 1,
 FONT_NEXT, 1, 9,
 FONT_END, 3, 9,
 FONT_ADVANCE, 6, 0
 },
 {
 92,
 FONT_BEGIN, 0, 7,
 FONT_END, 4, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 93,
 FONT_BEGIN, 1, 9,
 FONT_NEXT, 3, 9,
 FONT_NEXT, 3, 1,
 FONT_END, 1, 1,
 FONT_ADVANCE, 6, 0
 },
 {
 94,
 FONT_BEGIN, 2, 2,
 FONT_END, 2, 8,
 FONT_BEGIN, 0, 6,
 FONT_NEXT, 2, 8,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 95,
 FONT_BEGIN, 2, 3,
 FONT_NEXT, 0, 5,
 FONT_END, 2, 7,
 FONT_BEGIN, 0, 5,
 FONT_END, 4, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 96,
 FONT_BEGIN, 3, 8,
 FONT_NEXT, 2, 8,
 FONT_NEXT, 2, 9,
 FONT_NEXT, 3, 9,
 FONT_NEXT, 3, 8,
 FONT_END, 4, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 97,
 FONT_BEGIN, 1, 6,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_NEXT, 4, 2,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 4,
 FONT_END, 4, 4,
 FONT_ADVANCE, 6, 0
 },
 {
 98,
 FONT_BEGIN, 0, 8,
 FONT_NEXT, 0, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 4, 5,
 FONT_NEXT, 3, 6,
 FONT_END, 0, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 99,
 FONT_BEGIN, 4, 5,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 1, 6,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 100,
 FONT_BEGIN, 4, 8,
 FONT_NEXT, 4, 2,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 1, 6,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 101,
 FONT_BEGIN, 3, 2,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 1, 6,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_NEXT, 3, 4,
 FONT_END, 0, 4,
 FONT_ADVANCE, 6, 0
 },
 {
 102,
 FONT_BEGIN, 1, 2,
 FONT_NEXT, 1, 7,
 FONT_NEXT, 2, 8,
 FONT_NEXT, 3, 8,
 FONT_END, 4, 7,
 FONT_BEGIN, 2, 5,
 FONT_END, 0, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 103,
 FONT_BEGIN, 1, 0,
 FONT_NEXT, 3, 0,
 FONT_NEXT, 4, 1,
 FONT_NEXT, 4, 5,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 1, 6,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 104,
 FONT_BEGIN, 0, 2,
 FONT_END, 0, 8,
 FONT_BEGIN, 0, 6,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 105,
 FONT_BEGIN, 2, 2,
 FONT_END, 2, 5,
 FONT_BEGIN, 2, 6,
 FONT_END, 2, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 106,
 FONT_BEGIN, 0, 1,
 FONT_NEXT, 1, 0,
 FONT_NEXT, 3, 0,
 FONT_NEXT, 4, 1,
 FONT_END, 4, 5,
 FONT_BEGIN, 4, 6,
 FONT_END, 4, 7,
 FONT_ADVANCE, 6, 0
 },
 {
 107,
 FONT_BEGIN, 0, 2,
 FONT_END, 0, 8,
 FONT_BEGIN, 4, 6,
 FONT_NEXT, 2, 4,
 FONT_END, 0, 4,
 FONT_BEGIN, 2, 4,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 108,
 FONT_BEGIN, 1, 2,
 FONT_END, 1, 8,
 FONT_ADVANCE, 6, 0
 },
 {
 109,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 1, 6,
 FONT_NEXT, 2, 5,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_END, 4, 2,
 FONT_BEGIN, 2, 2,
 FONT_END, 2, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 110,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 1, 5,
 FONT_NEXT, 2, 6,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 111,
 FONT_BEGIN, 0, 3,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 1, 6,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 1, 2,
 FONT_END, 0, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 112,
 FONT_BEGIN, 0, 0,
 FONT_NEXT, 0, 6,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 3, 2,
 FONT_END, 0, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 113,
 FONT_BEGIN, 4, 2,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 1, 6,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 4, 5,
 FONT_END, 4, 0,
 FONT_ADVANCE, 6, 0
 },
 {
 114,
 FONT_BEGIN, 0, 2,
 FONT_END, 0, 6,
 FONT_BEGIN, 0, 4,
 FONT_NEXT, 2, 6,
 FONT_NEXT, 3, 6,
 FONT_END, 4, 5,
 FONT_ADVANCE, 6, 0
 },
 {
 115,
 FONT_BEGIN, 0, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_NEXT, 3, 4,
 FONT_NEXT, 1, 4,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 1, 6,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 116,
 FONT_BEGIN, 0, 6,
 FONT_END, 4, 6,
 FONT_BEGIN, 2, 8,
 FONT_NEXT, 2, 3,
 FONT_NEXT, 3, 2,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 117,
 FONT_BEGIN, 0, 6,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 118,
 FONT_BEGIN, 0, 6,
 FONT_NEXT, 0, 4,
 FONT_NEXT, 2, 2,
 FONT_NEXT, 4, 4,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 119,
 FONT_BEGIN, 0, 6,
 FONT_NEXT, 0, 3,
 FONT_NEXT, 1, 2,
 FONT_NEXT, 2, 3,
 FONT_NEXT, 3, 2,
 FONT_NEXT, 4, 3,
 FONT_END, 4, 6,
 FONT_ADVANCE, 6, 0
 },
 {
 120,
 FONT_BEGIN, 0, 2,
 FONT_END, 4, 6,
 FONT_BEGIN, 0, 6,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 121,
 FONT_BEGIN, 0, 0,
 FONT_NEXT, 4, 4,
 FONT_END, 4, 6,
 FONT_BEGIN, 0, 6,
 FONT_NEXT, 0, 4,
 FONT_END, 2, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 122,
 FONT_BEGIN, 0, 6,
 FONT_NEXT, 4, 6,
 FONT_NEXT, 0, 2,
 FONT_END, 4, 2,
 FONT_ADVANCE, 6, 0
 },
 {
 123,
 FONT_BEGIN, 4, 9,
 FONT_NEXT, 3, 8,
 FONT_NEXT, 3, 6,
 FONT_NEXT, 2, 5,
 FONT_NEXT, 3, 4,
 FONT_NEXT, 3, 2,
 FONT_END, 4, 1,
 FONT_ADVANCE, 6, 0
 },
 {
 124,
 FONT_BEGIN, 2, 9,
 FONT_END, 2, 0,
 FONT_ADVANCE, 6, 0
 },
 {
 125,
 FONT_BEGIN, 2, 2,
 FONT_NEXT, 2, 3,
 FONT_NEXT, 0, 5,
 FONT_NEXT, 2, 7,
 FONT_END, 2, 8,
 FONT_BEGIN, 2, 7,
 FONT_NEXT, 4, 5,
 FONT_END, 2, 3,
 FONT_ADVANCE, 6, 0
 },
 {
 126,
 FONT_BEGIN, 0, 9,
 FONT_NEXT, 1, 8,
 FONT_NEXT, 1, 6,
 FONT_NEXT, 2, 5,
 FONT_NEXT, 1, 4,
 FONT_NEXT, 1, 2,
 FONT_END, 0, 1,
 FONT_ADVANCE, 6, 0
 },
 {
 END_OF_LIST
 }
};


vfrLineText::vfrLineText(const std::string& nm, const Bool ssm)
  : vfrLetters(nm, ssm) {
  reset();
}

vfrLineText::~vfrLineText() {
}


void
vfrLineText::reset() {
  setFontScale(1.0f);
}

GLfloat
vfrLineText::getTextWidth() const {
  register float tw = 0.f;
  register float maxw = 0.f;
  register int textLen = _textBuf.size();
  for ( register int i = 0; i < textLen; i++ ) {
    register GLint mode;
    if ( _textBuf[i] == '\n' ) {
      if ( tw > maxw ) maxw = tw;
      tw = 0.f;
      continue;
    }
    for ( register int j = 1; mode = lineFont[_textBuf[i]-32][j]; j += 3 )
      if ( mode == FONT_ADVANCE ) {
	tw += (float)lineFont[_textBuf[i]-32][j+1]*LINE_STROKE_SCALE;
	tw += _fontSize[0] * _spaceRate;
      }
  } // end of for(i)
  if ( tw > maxw ) maxw = tw;
  return maxw;
}

GLfloat
vfrLineText::getTextHeight() const {
  return _fontSize[1];
}

GLfloat
vfrLineText::drawLetter(const char c) const {
  if ( c < 0x20 || c > 0x7e )
    return 0.f;
  register int i = c - 0x20;
  register GLint mode;
  register GLfloat retv = 0.f;
  for ( register int j = 1; mode = lineFont[i][j]; j += 3 ) {
    if ( mode == FONT_BEGIN ) {
      glBegin(GL_LINE_STRIP);
      glVertex2f((float)lineFont[i][j+1]*LINE_STROKE_SCALE,
               (float)lineFont[i][j+2]*LINE_STROKE_SCALE);
    } else if ( mode == FONT_NEXT ) {
      glVertex2f((float)lineFont[i][j+1]*LINE_STROKE_SCALE,
               (float)lineFont[i][j+2]*LINE_STROKE_SCALE);
    } else if ( mode == FONT_END ) {
      glVertex2f((float)lineFont[i][j+1]*LINE_STROKE_SCALE,
               (float)lineFont[i][j+2]*LINE_STROKE_SCALE);
      glEnd();
    } else if ( mode == FONT_ADVANCE ) {
      retv = (float)lineFont[i][j+1]*LINE_STROKE_SCALE
	+ (_fontSize[0]*_spaceRate);
      glTranslatef(retv, (float)lineFont[i][j+2]*LINE_STROKE_SCALE, 0.f);
      break;
    }
  }
  return retv;
}

// EOF
