#pragma once
#include <vector>
class IAnimation
{	
	public:
		static const int BLACK = 0;
		static const int WHITE = 122;
		static const int LIGHT_GREY = 123;
		static const int DARK_GREY = 124;
		static const int BLUE = 125;
		static const int GREEN = 126;
		static const int RED = 127;

		std::vector<std::vector<int>> midi_pad_map
		{
			{92, 93, 94, 95, 96, 97, 98, 99},
			{84, 85, 86, 87, 88, 89, 90, 91},
			{76, 77, 78, 79, 80, 81, 82, 83},
			{68, 69, 70, 71, 72, 73, 74, 75},
			{60, 61, 62, 63, 64, 65, 66, 67},
			{52, 53, 54, 55, 56, 57, 58, 59},
			{44, 45, 46, 47, 48, 49, 50, 51},
			{36, 37, 38, 39, 40, 41, 42, 43}
		};

		~IAnimation() {
		
		};
		
		virtual void goToNextFrame() = 0;
		virtual void initializeMatrix() = 0;
		virtual String getName() = 0;

		std::vector<std::vector<int>> getMatrix() {
			return matrix;
		}

		void transformMatrix(std::vector<std::vector<int>> &newMatrix) {
			matrix = newMatrix;
		}
	
	protected:
		std::vector<std::vector<int>> matrix
		{
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0}
		};
};

