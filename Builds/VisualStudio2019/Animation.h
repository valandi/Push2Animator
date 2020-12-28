#include <vector>
#include <list>
#include <iostream>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>

using namespace std;

#pragma once
class Animation
{
	public:
        static const int BLACK = 0;
        static const int WHITE = 122;
        static const int LIGHT_GREY = 123;
        static const int DARK_GREY = 124;
        static const int BLUE = 125;
        static const int GREEN = 126;
        static const int RED = 127;

        Animation() {

        };

        ~Animation() {

        };

        Animation(string name, std::vector<std::vector<int>> initialMatrix) {
            this.animationName = name;
            this.matrix = initialMatrix;
        };

        /**
          Get's the initial 8x8 matrix (the first frame of the animation 
        */
		virtual std::vector<std::vector<int>> getInitialMatrix() = 0;
        
        /**
          Get's the next frame of the sequence. Typically, some sort of operation will be done on the current 
        */
        std::vector<std::vector<int>> getNextFrame() {
            getInitialMatrix();
        };

        std::vector<std::vector<int>> getMatrix() {
            return matrix;
        };

        string getAnimationName() { return animationName; }


    protected:
        std::vector<std::vector<int>> matrix
        {
            {-1, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY}, 
            {LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY},
            {LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY},
            {LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY},
            {LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY},
            {LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY},
            {LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY},
            {LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY, LIGHT_GREY}
        };

        string animationName = "AnimationName";
};