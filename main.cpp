#include <iostream>
#include <raylib.h>

using namespace std;

Color Light_Blue = Color{52, 152, 219, 255}; //light blue
Color Dark_Blue = Color{33, 97, 140, 255}; //dark blue
Color Blue = Color{133, 193, 233, 255}; //blue
Color White = Color{255, 255, 255, 255}; //white

int player_score = 0;
int cpu_score = 0;


class Ball{
    public:
        float x, y;
        int speed_x, speed_y;
        int radius;

    Ball(float x, float y, int radius, int speed_x, int speed_y){
        this->x = x;
        this->y = y;
        this->radius = radius;
        this->speed_x = speed_x;
        this->speed_y = speed_y;
    }

    void Draw(){
        DrawCircle(x, y, radius, White);
    }

    void Update(){
        x += speed_x;
        y += speed_y;

        if(y + radius >= GetScreenHeight() || y - radius <= 0){
            speed_y *= -1;
        }
        else if(x + radius >= GetScreenWidth()){//player scores
            player_score ++;
            ResetBall();
        } 
        else if(x - radius <= 0){//cpu scores
            cpu_score ++;
            ResetBall();
        }
    }

    void ResetBall(){
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0,1)];
        speed_y *= speed_choices[GetRandomValue(0,1)];

    }

};

class Paddle{

    protected:

    void LimitMovement(){

         if(y <= 0){
            y = 0;
        }
        else if(y + height >= GetScreenHeight()){
            y = GetScreenHeight() - height;
        }
    }

    public:
        float x, y;
        float width, height;
        int speed_y;

    Paddle(float x, float y, float width, float height, int speed_y){
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
        this->speed_y = speed_y;
    }

    void Draw(){
        DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
    }

    void Update(){
        if(IsKeyDown(KEY_UP)){
            y -= speed_y;
        }
        else if(IsKeyDown(KEY_DOWN)){
            y += speed_y;
        }

        LimitMovement();
    }

    
};

class CpuPaddle: public Paddle{

    public:
    //contructor
    CpuPaddle(float x, float y, float width, float height, int speed_y): Paddle(x, y, width, height, speed_y){

    }


    void Update(int ball_y){
        if(y + height/2 > ball_y){
            y -= speed_y;
        }
        else if(y + height/2 < ball_y){
            y += speed_y;
        }

        LimitMovement();
    }   

};

int main () {

    cout << "Starting game!";
    const int SCREEN_WIDTH  = 1280;
    const int SCREEN_HEIGHT = 800;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My Pong Game");
    SetTargetFPS(60);

    //object initi
    Ball ball(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 20, 7, 7);
    Paddle player(10, (SCREEN_HEIGHT/2) - 60, 25, 120, 6);
    CpuPaddle cpu(SCREEN_WIDTH - 35, (SCREEN_HEIGHT/2) - 60, 25, 120, 6);

    cout << cpu.x;

    while(WindowShouldClose() == false){
        BeginDrawing();
        
        //Update postions
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        //checking for collision
        if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{player.x, player.y, player.width, player.height})){
            ball.speed_x *= -1;
        }
        else if(CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})){
            ball.speed_x *= -1;
        }


        //Drawing 
        ClearBackground(Dark_Blue);
        DrawRectangle(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, Light_Blue);
        DrawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 150, Blue);
        DrawLine(SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT, WHITE);
        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), SCREEN_WIDTH/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * SCREEN_WIDTH/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}