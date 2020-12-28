#pragma once
#include "IManager.h"
#include "IAnimation.h"
#include <vector>

class AnimationManager
{
	public:
		AnimationManager(IAnimation* newAnimation) {
			this->animation = newAnimation;
			this->shouldAnimate = false;
			this->speedFactor = 3;
		};

		void setAnimation(IAnimation* newAnimation) {
			this->animation = newAnimation;
		}

		void animate() {
			if (shouldAnimate) {
				animation->goToNextFrame();
				Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 100 * speedFactor);
			}

		};

		Array<juce::MidiMessage> getMidiMessages() {
			DBG("Getting MIDI messages...");
			Array<juce::MidiMessage> midiMessages = {};

			if (shouldAnimate) {
				for (int row = 0; row < 8; row++) {
					for (int col = 0; col < 8; col++) {
						midiMessages.add(
							juce::MidiMessage(144, 
								animation->midi_pad_map[row][col], 
								animation->getMatrix()[row][col]
							)
						);
					}
				}
			}
			else {
				for (int row = 0; row < 8; row++) {
					for (int col = 0; col < 8; col++) {
						midiMessages.add(
							juce::MidiMessage(144,
								animation->midi_pad_map[row][col],
								0
							)
						);
					}
				}
			}

			return midiMessages;
		};

		void startAnimating() {
			shouldAnimate = true;
		}

		void stopAnimating() { 
			shouldAnimate = false; 
		}

		void setSpeedFactor(int newSpeedFactor) {
			if (newSpeedFactor >= 1 && newSpeedFactor <= 10) {
				this->speedFactor = newSpeedFactor;
			}
		}

		bool isAnimating() { return shouldAnimate; }

	private:
		IAnimation* animation;
		bool shouldAnimate;
		int speedFactor;
};