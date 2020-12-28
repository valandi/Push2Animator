/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2020 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             MidiManager
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Handles incoming and outcoming midi messages.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2019, linux_make, androidstudio, xcode_iphone

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        MidiManager

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>

#include "AnimationManager.h"
#include "HeartAnimation.h"
#include "DiagnoalAnimation.h"

//==============================================================================
struct MidiDeviceListEntry : ReferenceCountedObject
{
    MidiDeviceListEntry (MidiDeviceInfo info) : deviceInfo (info) {}

    MidiDeviceInfo deviceInfo;
    std::unique_ptr<MidiInput> inDevice;
    std::unique_ptr<MidiOutput> outDevice;

    using Ptr = ReferenceCountedObjectPtr<MidiDeviceListEntry>;
};


//==============================================================================
class MidiManager : public Component,
    private Timer,
    private MidiInputCallback,
    private AsyncUpdater, public juce::Slider::Listener
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
        {DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY},
        {DARK_GREY, BLUE, DARK_GREY, BLUE, BLUE, DARK_GREY, BLUE, DARK_GREY},
        {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
        {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
        {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
        {DARK_GREY, BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE, DARK_GREY},
        {DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY},
        {DARK_GREY, DARK_GREY, DARK_GREY, BLUE, BLUE, DARK_GREY, DARK_GREY, DARK_GREY}
    };


    std::vector<std::vector<int>> diagonal
    {
        {0, 1, 2, 3, 4, 5, 6, 7},
        {DARK_GREY, BLUE, DARK_GREY, BLUE, BLUE, DARK_GREY, BLUE, DARK_GREY},
        {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
        {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
        {BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE},
        {DARK_GREY, BLUE, DARK_GREY, DARK_GREY, DARK_GREY, DARK_GREY, BLUE, DARK_GREY},
        {DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY, BLUE, DARK_GREY, DARK_GREY},
        {DARK_GREY, DARK_GREY, DARK_GREY, BLUE, BLUE, DARK_GREY, DARK_GREY, DARK_GREY}
    };

    std::vector<std::vector<int>> stars
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

    //==============================================================================
    MidiManager()
        : midiKeyboard(keyboardState, MidiKeyboardComponent::horizontalKeyboard),
        midiOutputSelector(new MidiDeviceListBox("Midi Output Selector", *this, false)),
        animationManager(new HeartAnimation())
    {

        addAndMakeVisible(textLabel);
        textLabel.setFont(textFont);

        //TODO: Design an animation template file and have animationMap load from available Animations available
        animationMap[1] = new HeartAnimation();
        animationMap[2] = new DiagonalAnimation();

        addAndMakeVisible(rotarySlider);
        rotarySlider.setRange(1, 10);
        rotarySlider.addListener(this);


        addAndMakeVisible(animationDropDown);
        animationDropDown.addItem("Heart", 1);
        animationDropDown.addItem("Diagonal", 2);
        animationDropDown.setSelectedId(1);


        addLabelAndSetStyle(midiOutputLabel);
        addLabelAndSetStyle(chooseAnimationLabel);

        midiKeyboard.setName("MIDI Keyboard");
        addAndMakeVisible(midiKeyboard);

        addAndMakeVisible(startButton);
        addAndMakeVisible(stopButton);

        startButton.onClick = [this]
        {
            std::thread(
                [this] {
                    if (!animationManager.isAnimating()) {
                        animationManager.setAnimation(animationMap[animationDropDown.getSelectedId()]);
                        animationManager.startAnimating();
                        animate();
                    }
                }).detach();
        };

        stopButton.onClick = [this]
        {
            std::thread(
                [this] {
                    animationManager.stopAnimating();
                    clearColor();
                }
            ).detach();
        };

        addAndMakeVisible(makeBlinksButton);
        makeBlinksButton.onClick = [this]
        {
            std::thread(
                [this] {
                    makeBlinks();
                }
            ).detach();
        };

        addAndMakeVisible(midiOutputSelector.get());

        initializeMidiMap();

        setSize(732, 520);
        startTimer(500);
    }

    ~MidiManager() override
    {
        clearColor();
        stopTimer();

        midiOutputs.clear();
        midiOutputSelector.reset();
    }

    void clearColor()
    {
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                sendToOutputs(juce::MidiMessage(144, midi_pad_map[row][col], BLACK));
            }
        }

        for (int address = 3; address < 120; address++) {
            sendToOutputs(juce::MidiMessage(176, address, BLACK));
        }

    }

    void sliderValueChanged(juce::Slider* slider) override {
        if (slider == &rotarySlider)
            animationManager.setSpeedFactor(slider->getValue());
    }

    void animate() {
        
        Array<juce::MidiMessage> messages = animationManager.getMidiMessages();
        
        while (animationManager.isAnimating()) {
            messages = animationManager.getMidiMessages();
            for (int i = 0; i < 64; i++) {
                sendToOutputs(messages[i]);
            }
            animationManager.animate();
        }
    }

    void makeBlinks() { 
        for (int counter = 0; counter < 100; counter++) {
            for (int address = 3; address < 120; address++) {
                sendToOutputs(juce::MidiMessage(176, address, 127));
            }

            Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 300);

            for (int address = 3; address < 120; address++) {
                sendToOutputs(juce::MidiMessage(176, address, BLACK));
            }

            Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 300);
        }
    }

    void drawStars() {

        for (int star_count = 0; star_count < 10; star_count++) {
            stars[0 + (rand() % static_cast<int>(7 - 0 + 1))][0 + (rand() % static_cast<int>(7 - 0 + 1))] = GREEN;
        }

        for (int counter = 0; counter < 127; counter++) {
            for (int row = 0; row < 8; row++) {
                for (int col = 0; col < 8; col++) {
                    if (col < 4) {
                        int b = stars[row][col];
                        stars[row][col] = stars[row][7 - col];
                        stars[row][7 - col] = b;
                    }
                    sendToOutputs(juce::MidiMessage(144, midi_pad_map[row][col], stars[row][col]));
                }
            }
            Time::waitForMillisecondCounter(Time::getMillisecondCounter() + 500);
        }
    }

    //==============================================================================
    void timerCallback() override
    {
        updateDeviceList (true);
        updateDeviceList (false);
    }

    void initializeMidiMap()
    {
        int pad_value = 92;
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                midi_pad_map[row][col] = pad_value + col;
            }
            pad_value -= 8;
        }
    }

    void paint (Graphics&) override {}

    void resized() override
    {
        auto margin = 10;

        auto area = getLocalBounds().reduced(10);
        auto row = area.removeFromTop(100);

        rotarySlider.setBounds(row.removeFromLeft(100).reduced(5));

        midiOutputLabel.setBounds (
            (getWidth() / 2) + margin, 
            margin,
            (getWidth() / 2) - (2 * margin), 
            24
        );

        chooseAnimationLabel.setBounds(
            margin,
            (getHeight() / 2) - (margin + 24),
            (getWidth() / 2) - (2 * margin), 
            24
        );

        midiOutputSelector->setBounds(
            getWidth() / 2 + margin, 
            (2 * margin) + 24,
            (getWidth() / 2 - margin),
            (getHeight() / 2) - ((4 * margin) + 24 + 24)
        );

        animationDropDown.setBounds(
            margin,
            (getHeight() / 2) + (margin), 
            (getWidth() / 2) - (2 * margin),
            24
        );

        startButton.setBounds(
            (getWidth() / 2) + margin,
            (getHeight() / 2)  - (24 + margin),
            (getWidth() / 2) - (2 * margin),
            (getHeight() / 4)
        );

        stopButton.setBounds(
            (getWidth() / 2) + margin,
            (getHeight() / 2) + (getHeight() / 4) - (2 * margin), 
            (getWidth() / 2) - (2 * margin), 
            (getHeight() / 4)
        );

        auto y = (getHeight() / 2) + ((2 * 24) + (3 * margin) + 64);
    }

    void openDevice (bool isInput, int index)
    {
        jassert (midiOutputs[index]->outDevice.get() == nullptr);
        midiOutputs[index]->outDevice = MidiOutput::openDevice (midiOutputs[index]->deviceInfo.identifier);

        if (midiOutputs[index]->outDevice.get() == nullptr)
        {
            DBG ("MidiManager::openDevice: open output device for index = " << index << " failed!");
        }
        
    }

    void closeDevice (bool isInput, int index)
    {
        jassert (midiOutputs[index]->outDevice.get() != nullptr);
        midiOutputs[index]->outDevice.reset();
    }

    ReferenceCountedObjectPtr<MidiDeviceListEntry> getMidiDevice(int index, bool isInput) const noexcept
    {
        return midiOutputs[index];
    }

    int getNumMidiOutputs() const noexcept
    {
        return midiOutputs.size();
    }

private:
    //==============================================================================
    struct MidiDeviceListBox : public ListBox,
                               private ListBoxModel
    {
        MidiDeviceListBox (const String& name,
                            MidiManager& contentComponent,
                           bool isInputDeviceList)
            : ListBox (name, this),
              parent (contentComponent)
        {
            setOutlineThickness (1);
            setMultipleSelectionEnabled (true);
            setClickingTogglesRowSelection (true);
        }

        //==============================================================================
        int getNumRows() override
        {
            return parent.getNumMidiOutputs();
        }

        void paintListBoxItem (int rowNumber, Graphics& g,
                               int width, int height, bool rowIsSelected) override
        {
            auto textColour = getLookAndFeel().findColour (ListBox::textColourId);

            if (rowIsSelected)
                g.fillAll (textColour.interpolatedWith (getLookAndFeel().findColour (ListBox::backgroundColourId), 0.5));


            g.setColour (textColour);
            g.setFont ((float) height * 0.7f);

            if (rowNumber < parent.getNumMidiOutputs())
                g.drawText (parent.getMidiDevice (rowNumber, false)->deviceInfo.name,
                            5, 0, width, height,
                            Justification::centredLeft, true);
        }

        //==============================================================================
        void selectedRowsChanged (int) override
        {
            auto newSelectedItems = getSelectedRows();
            if (newSelectedItems != lastSelectedItems)
            {
                for (auto i = 0; i < lastSelectedItems.size(); ++i)
                {
                    if (! newSelectedItems.contains (lastSelectedItems[i]))
                        parent.closeDevice (isInput, lastSelectedItems[i]);
                }

                for (auto i = 0; i < newSelectedItems.size(); ++i)
                {
                    if (! lastSelectedItems.contains (newSelectedItems[i]))
                        parent.openDevice (isInput, newSelectedItems[i]);
                }

                lastSelectedItems = newSelectedItems;
            }
        }

        //==============================================================================
        void syncSelectedItemsWithDeviceList (const ReferenceCountedArray<MidiDeviceListEntry>& midiDevices)
        {
            SparseSet<int> selectedRows;
            for (auto i = 0; i < midiDevices.size(); ++i)
                if (midiDevices[i]->inDevice.get() != nullptr || midiDevices[i]->outDevice.get() != nullptr)
                    selectedRows.addRange (Range<int> (i, i + 1));

            lastSelectedItems = selectedRows;
            updateContent();
            setSelectedRows (selectedRows, dontSendNotification);
        }

    private:
        //==============================================================================
        MidiManager& parent;
        bool isInput;
        SparseSet<int> lastSelectedItems;
    };

    //==============================================================================
    void handleIncomingMidiMessage (MidiInput* /*source*/, const juce::MidiMessage& message) override
    {
        // This is called on the MIDI thread
        const ScopedLock sl (midiMonitorLock);
        incomingMessages.add (message);
        triggerAsyncUpdate();
    }

    void handleAsyncUpdate() override
    {
        // This is called on the message loop
        Array<juce::MidiMessage> messages;

        {
            const ScopedLock sl (midiMonitorLock);
            messages.swapWith (incomingMessages);
        }

        String messageText;

        for (auto& m : messages)
            messageText << m.getDescription() << "\n";

        midiMonitor.insertTextAtCaret (messageText);
    }

    void sendToOutputs (const juce::MidiMessage& msg)
    {
        for (auto midiOutput : midiOutputs)
            if (midiOutput->outDevice.get() != nullptr)
                midiOutput->outDevice->sendMessageNow (msg);
    }

    //==============================================================================
    bool hasDeviceListChanged (const Array<MidiDeviceInfo>& availableDevices, bool isInputDevice)
    {
        ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = midiOutputs;

        if (availableDevices.size() != midiDevices.size())
            return true;

        for (auto i = 0; i < availableDevices.size(); ++i)
            if (availableDevices[i] != midiDevices[i]->deviceInfo)
                return true;

        return false;
    }

    ReferenceCountedObjectPtr<MidiDeviceListEntry> findDevice (MidiDeviceInfo device, bool isInputDevice) const
    {
        const ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = midiOutputs;

        for (auto& d : midiDevices)
            if (d->deviceInfo == device)
                return d;

        return nullptr;
    }

    void closeUnpluggedDevices (const Array<MidiDeviceInfo>& currentlyPluggedInDevices, bool isInputDevice)
    {
        ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = midiOutputs;

        for (auto i = midiDevices.size(); --i >= 0;)
        {
            auto& d = *midiDevices[i];

            if (! currentlyPluggedInDevices.contains (d.deviceInfo))
            {
                if (d.outDevice.get() != nullptr)
                    closeDevice (isInputDevice, i);

                midiDevices.remove (i);
            }
        }
    }

    void updateDeviceList (bool isInputDeviceList)
    {
        auto availableDevices = MidiOutput::getAvailableDevices();

        if (hasDeviceListChanged (availableDevices, isInputDeviceList))
        {

            ReferenceCountedArray<MidiDeviceListEntry>& midiDevices = midiOutputs;

            closeUnpluggedDevices (availableDevices, isInputDeviceList);

            ReferenceCountedArray<MidiDeviceListEntry> newDeviceList;

            // add all currently plugged-in devices to the device list
            for (auto& newDevice : availableDevices)
            {
                MidiDeviceListEntry::Ptr entry = findDevice (newDevice, isInputDeviceList);

                if (entry == nullptr)
                    entry = new MidiDeviceListEntry (newDevice);

                newDeviceList.add (entry);
            }

            // actually update the device list
            midiDevices = newDeviceList;

            // update the selection status of the combo-box
            if (auto* midiSelector = midiOutputSelector.get())
                midiSelector->syncSelectedItemsWithDeviceList (midiDevices);
        }
    }

    //==============================================================================
    void addLabelAndSetStyle (Label& label)
    {
        label.setFont (Font (15.00f, Font::plain));
        label.setJustificationType (Justification::centredLeft);
        label.setEditable (false, false, false);
        label.setColour (TextEditor::textColourId, Colours::black);
        label.setColour (TextEditor::backgroundColourId, Colour (0x00000000));

        addAndMakeVisible (label);
    }

    //==============================================================================
    Label midiOutputLabel   { "Midi Output Label", "MIDI Output:" };
    Label outgoingMidiLabel { "Outgoing Midi Label", "Play the keyboard to send MIDI messages..." };
    Label chooseAnimationLabel{ "Choose animation Label", "Animation:" };

    MidiKeyboardState keyboardState;
    MidiKeyboardComponent midiKeyboard;
    TextEditor midiMonitor  { "MIDI Monitor" };
    
    TextButton makeBlinksButton{ "Make it blink!!" };
    TextButton drawHeartButton{ "Draw a heart!" };
    TextButton drawDiagonalButton{ "Draw a diagonal!" };
    
    TextButton startButton{ "Start!" };
    TextButton stopButton{ "Stop!" };

    std::unique_ptr<MidiDeviceListBox> midiOutputSelector;
    ReferenceCountedArray<MidiDeviceListEntry> midiOutputs;

    CriticalSection midiMonitorLock;
    Array<juce::MidiMessage> incomingMessages;
    
    AnimationManager animationManager;

    juce::Label textLabel{ {}, "The quick brown fox jumps over the lazy dog." };
    juce::Font textFont{ 12.0f };
    juce::ComboBox animationDropDown;
    std::map<int, IAnimation*> animationMap;

    Slider rotarySlider{ Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox },
        verticalSlider{ Slider::LinearVertical, Slider::NoTextBox },
        barSlider{ Slider::LinearBar, Slider::NoTextBox },
        incDecSlider{ Slider::IncDecButtons, Slider::TextBoxBelow };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiManager)
};