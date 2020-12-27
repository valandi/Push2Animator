#pragma once
#include "Animation.h" 
using namespace std;
class StarsAnimation :
    public Animation
{
    public:
        
        StarsAnimation() {
            matrix = getInitialMatrix();
            animationName = "Stars Animation";
        }

        std::vector<std::vector<int>> getInitialMatrix() {
            return
            {
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE},
                {BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE}
            };
        }
        
        std::vector<std::vector<int>> getNextFrame() {
            for (int star_count = 0; star_count < 10; star_count++) {
                matrix[0 + (rand() % static_cast<int>(7 - 0 + 1))][0 + (rand() % static_cast<int>(7 - 0 + 1))] = GREEN;
            }
            return matrix;
        }
        
};

