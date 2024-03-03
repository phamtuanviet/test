/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL.h>
#include <stdio.h>

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Kích thước cửa sổ
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// Kích thước ô vuông
const int GRID_SIZE = 20;
const int GRID_WIDTH = WINDOW_WIDTH / GRID_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / GRID_SIZE;

// Cấu trúc của một ô trên màn hình
struct Cell {
    int x, y;
};

// Hướng di chuyển
enum class Direction { UP, DOWN, LEFT, RIGHT };

int main(int argc, char* argv[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Snake Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          WINDOW_WIDTH,
                                          WINDOW_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Khởi tạo random
    std::srand(std::time(nullptr));

    // Khởi tạo con rắn
    std::vector<Cell> snake = {{GRID_WIDTH / 2, GRID_HEIGHT / 2}};
    Direction direction = Direction::RIGHT;

    // Khởi tạo mồi
    Cell food = {std::rand() % GRID_WIDTH, std::rand() % GRID_HEIGHT};

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        direction = Direction::UP;
                        break;
                    case SDLK_DOWN:
                        direction = Direction::DOWN;
                        break;
                    case SDLK_LEFT:
                        direction = Direction::LEFT;
                        break;
                    case SDLK_RIGHT:
                        direction = Direction::RIGHT;
                        break;
                }
            }
        }

        // Di chuyển con rắn
        Cell next = snake.front();
        switch (direction) {
            case Direction::UP:
                next.y--;
                break;
            case Direction::DOWN:
                next.y++;
                break;
            case Direction::LEFT:
                next.x--;
                break;
            case Direction::RIGHT:
                next.x++;
                break;
        }

        // Kiểm tra va chạm với tường hoặc cơ thể
        if (next.x < 0 || next.x >= GRID_WIDTH || next.y < 0 || next.y >= GRID_HEIGHT) {
            quit = true; // Game over
        } else {
            snake.insert(snake.begin(), next);
            if (next.x == food.x && next.y == food.y) {
                // Ăn mồi, tạo mồi mới
                food = {std::rand() % GRID_WIDTH, std::rand() % GRID_HEIGHT};
            } else {
                // Di chuyển và xoá ô cuối của con rắn
                snake.pop_back();
            }
        }

        // Xóa màn hình
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Vẽ con rắn
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        for (const auto& cell : snake) {
            SDL_Rect rect = {cell.x * GRID_SIZE, cell.y * GRID_SIZE, GRID_SIZE, GRID_SIZE};
            SDL_RenderFillRect(renderer, &rect);
        }

        // Vẽ mồi
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
        SDL_Rect rect = {food.x * GRID_SIZE, food.y * GRID_SIZE, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);

        // Cập nhật màn hình
        SDL_RenderPresent(renderer);

        // Ngưng 0.1 giây để giảm tốc độ của trò chơi
        SDL_Delay(100);
    }

    // Giải phóng bộ nhớ và thoát
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
