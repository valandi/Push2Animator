#pragma once
#include "IAnimation.h"
class DiagonalAnimation : public IAnimation
{
    public:
        DiagonalAnimation() {
            initializeMatrix();
        }

        ~DiagonalAnimation() {

        }

        String getName() {
            return "Diagonal";
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
            DBG("Initializing the diagonal");

            transformMatrix(
                std::vector<std::vector<int>>
                {
                    {0, 1, 2, 3, 4, 5, 6, 7},
                    {1, 2, 3, 4, 5, 6, 7, 8},
                    {2, 3, 4, 5, 6, 7, 8, 9},
                    {3, 4, 5, 6, 7, 8, 9, 10},
                    {4, 5, 6, 7, 8, 9, 10, 11},
                    {5, 6, 7, 8, 9, 10, 11, 12},
                    {6, 7, 8, 9, 10, 11, 12, 13},
                    {7, 8, 9, 10, 11, 12, 13, 14}
                }
            );
        }
};