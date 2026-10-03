#include "pch.h"

// Define the dimensions of the window and the segments of the digits depending on the width of the window
#define WIDTH 1200
#define HEIGHT 900
#define SegmentLength WIDTH/13
#define SegmentWidth SegmentLength/3
#define Sepration SegmentWidth/4

//The colors for the ON and OFF segments of the digits
#define ON_COLOR RED
#define OFF_COLOR BLACK

//Function Prototypes
void DrawSegment(Vector2 CenterPosition, bool horizontal, Color color);
void DrawPart(Vector2 coordinate, int digit);
void DrawColon(Vector2 coordinate);
void DrawClock(Vector2 coordinate, time_t currentTime);

//Define the segments of the digits
// 0 -> 1st, 2nd, 3rd, 5th, 6th, 7th
// 1 -> 3rd, 6th
// 2 -> 1st, 3rd, 4th, 5th, 7th
// 3 -> 1st, 3rd, 4th, 6th, 7th
// 4 -> 2nd, 3rd, 4th, 6th
// 5 -> 1st, 2nd, 4th, 6th, 7th
// 6 -> 1st, 2nd, 4th, 5th, 6th, 7th
// 7 -> 1st, 3rd, 6th
// 8 -> 1st, 2nd, 3rd, 4th, 5th, 6th, 7th
// 9 -> 1st, 2nd, 3rd, 4th, 6th, 7th

int digits[10][7] = {
    {1, 1, 1, 0, 1, 1, 1}, //0
    {0, 0, 1, 0, 0, 1, 0}, //1
    {1, 0, 1, 1, 1, 0, 1}, //2
    {1, 0, 1, 1, 0, 1, 1}, //3
    {0, 1, 1, 1, 0, 1, 0}, //4
    {1, 1, 0, 1, 0, 1, 1}, //5
    {1, 1, 0, 1, 1, 1, 1}, //6
    {1, 0, 1, 0, 0, 1, 0}, //7
    {1, 1, 1, 1, 1, 1 ,1}, //8
    {1 ,1 ,1 ,1 ,0 ,1 ,1} //9
};

// Main entry point of the program
int main(){

    // Initialize the window and set the target FPS
    InitWindow(WIDTH, HEIGHT, "Digital Clock");
    SetTargetFPS(60);

    while(!WindowShouldClose()){
        BeginDrawing();
            ClearBackground(BLACK);
            DrawClock((Vector2){(4*SegmentLength + 3*Sepration), HEIGHT/2}, time(NULL));
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

// Function to draw a single segment of the digit, either horizontal or vertical, at the specified center position and color
void DrawSegment(Vector2 CenterPosition, bool horizontal, Color color){
    Vector2 a,b,c,d,e,f;
    int count = 6;
    float HalfSegmentLength = SegmentLength/2;
    float HalfSegmentWidth = SegmentWidth/2;

    // Calculate the vertices of the segment based on its orientation (horizontal or vertical)
    if (horizontal)
    {
        a = (Vector2){CenterPosition.x - HalfSegmentLength, CenterPosition.y};
        b = (Vector2){CenterPosition.x - HalfSegmentLength + HalfSegmentWidth, CenterPosition.y + HalfSegmentWidth};
        c = (Vector2){b.x, CenterPosition.y - HalfSegmentWidth};
        d = (Vector2){CenterPosition.x + HalfSegmentLength - HalfSegmentWidth, CenterPosition.y + HalfSegmentWidth};
        e = (Vector2){d.x, CenterPosition.y - HalfSegmentWidth};
        f = (Vector2){CenterPosition.x + HalfSegmentLength, CenterPosition.y};
    }
    else
    {
        a = (Vector2){CenterPosition.x, CenterPosition.y - HalfSegmentLength};
        b = (Vector2){CenterPosition.x - HalfSegmentWidth, CenterPosition.y - HalfSegmentLength + HalfSegmentWidth};
        c = (Vector2){CenterPosition.x + HalfSegmentWidth, CenterPosition.y - HalfSegmentLength + HalfSegmentWidth};
        d = (Vector2){CenterPosition.x - HalfSegmentWidth, CenterPosition.y + HalfSegmentLength - HalfSegmentWidth};
        e = (Vector2){CenterPosition.x + HalfSegmentWidth, CenterPosition.y + HalfSegmentLength - HalfSegmentWidth};
        f = (Vector2){CenterPosition.x, CenterPosition.y + HalfSegmentLength};
    }

    Vector2 points[6] = {a, b, c, d, e, f};
    DrawTriangleStrip(points, count, color);
    
    return;

}

// Function to draw a single digit at the specified coordinate by drawing its segments based on the digit's segment configuration
void DrawPart(Vector2 coordinate, int digit){
    float HalfSegmentLength = SegmentLength/2 + Sepration;
    int *digitSegments = &digits[digit][0];
   
    //Upper 1st segment of the digit
    DrawSegment((Vector2){coordinate.x , coordinate.y - (SegmentLength + Sepration)}, true, digitSegments[0] ? ON_COLOR : OFF_COLOR);
    //Upper 2nd segment of the digit
    DrawSegment((Vector2){coordinate.x - HalfSegmentLength, coordinate.y - HalfSegmentLength}, false, digitSegments[1] ? ON_COLOR : OFF_COLOR);
    //Upper 3rd segment of the digit
    DrawSegment((Vector2){coordinate.x + HalfSegmentLength, coordinate.y - HalfSegmentLength}, false, digitSegments[2] ? ON_COLOR : OFF_COLOR);
    //Center 4th segment of the digit
    DrawSegment(coordinate, true, digitSegments[3] ? ON_COLOR : OFF_COLOR);
    //Lower 5th segment of the digit
    DrawSegment((Vector2){coordinate.x - HalfSegmentLength, coordinate.y + HalfSegmentLength}, false, digitSegments[4] ? ON_COLOR : OFF_COLOR);
    //Lower 6th segments of the digit
    DrawSegment((Vector2){coordinate.x + HalfSegmentLength, coordinate.y + HalfSegmentLength}, false, digitSegments[5] ? ON_COLOR : OFF_COLOR);
    //Lower 7th segment of the digit
    DrawSegment((Vector2){coordinate.x, coordinate.y + (SegmentLength + Sepration)}, true, digitSegments[6] ? ON_COLOR : OFF_COLOR);
    
    return;
}

// Function to draw the colon between the hours, minutes, and seconds by drawing two circles at the specified coordinate
void DrawColon(Vector2 coordinate){
    DrawCircle(coordinate.x, coordinate.y - SegmentLength/2, SegmentWidth/2, ON_COLOR);
    DrawCircle(coordinate.x, coordinate.y + SegmentLength/2, SegmentWidth/2, ON_COLOR);
    return;
}

// Function to draw the entire clock by extracting the hours, minutes, and seconds from the current time and drawing each part of the clock accordingly
void DrawClock(Vector2 coordinate, time_t currentTime){
    // Extract the hours, minutes, and seconds from the current time using localtime function
    struct tm *localTime = localtime(&currentTime);
    int hours = localTime->tm_hour;
    int minutes = localTime->tm_min;
    int seconds = localTime->tm_sec;

    //Draw Hours
    DrawPart((Vector2){coordinate.x - 3*SegmentLength, coordinate.y}, hours/10);
    DrawPart((Vector2){coordinate.x - SegmentLength, coordinate.y}, hours%10);

    //Draw Colon
    DrawColon(coordinate);

    //Draw Minutes
    DrawPart((Vector2){coordinate.x + SegmentLength, coordinate.y}, minutes/10);
    DrawPart((Vector2){coordinate.x + 3*SegmentLength, coordinate.y}, minutes%10);

    //Draw Colon
    DrawColon((Vector2){coordinate.x + 4*SegmentLength, coordinate.y});

    //Draw Seconds
    DrawPart((Vector2){coordinate.x + 5*SegmentLength, coordinate.y}, seconds/10);
    DrawPart((Vector2){coordinate.x + 7*SegmentLength, coordinate.y}, seconds%10);

    return;
}