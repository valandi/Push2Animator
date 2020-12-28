#pragma once
#include "IAnimation.h"
class HeartAnimation : public IAnimation
{
    public:
        HeartAnimation() {
            initializeMatrix();
        }

	    ~HeartAnimation() { 

        }

        String getName() {
            return "Heart";
        }

        void goToNextFrame() {
            DBG(" Heart going to next frame. ");
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    matrix[row][col] = (matrix[row][col] + 1) % 128;
                }
            }
	    }

	    void initializeMatrix() {
            DBG("Initializing the heart");
		    transformMatrix(
                std::vector<std::vector<int>>
                {
                    {DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY},
                    {DARK_GREY, BLUE, DARK_GREY, BLUE, BLUE, DARK_GREY, BLUE, DARK_GREY},
                    {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
                    {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
                    {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
                    {DARK_GREY, BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE, DARK_GREY},
                    {DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY},
                    {DARK_GREY, DARK_GREY, DARK_GREY, BLUE, BLUE, DARK_GREY, DARK_GREY, DARK_GREY}
                }
            );
	    }
};