/*
  ==============================================================================

    ExternalMidi.h
    Created: 15 Apr 2023 4:14:34pm
    Author:  Luke Coles

    Class to handle external MIDI input from DAW
    REFERENCE:
        tpoole, "MidiLoggerPluginDemo.h", 
        <https://github.com/juce-framework/JUCE/blob/master/examples/Plugins/MidiLoggerPluginDemo.h>
        [accessed 21/04/2023]

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include <iterator>
class MidiQueue
{
public:
    void push(const juce::MidiBuffer& buffer)
    {
        for (const auto metadata : buffer) {
            fifo.write(1).forEach([&](int dest) {messages[(size_t)dest] = metadata.getMessage(); });
        }
    }
    template<typename OutputIt>
    void pop(OutputIt out)
    {
        fifo.read(fifo.getNumReady()).forEach([&](int source) {*out++ = messages[(size_t)source]; });
    }

private:
    static constexpr auto queueSize = 1 << 14;
    juce::AbstractFifo fifo{ queueSize };
    std::vector<juce::MidiMessage> messages = std::vector<juce::MidiMessage>(queueSize);

};

class MidiListModel
{
public:
    template<typename It>
    void addMessages(It begin, It end)
    {
        if (begin == end) { return; }
        const auto numNewMessages = (int)std::distance(begin, end);
        const auto numToAdd = juce::jmin(numToStore, numNewMessages);
        const auto numToRemove = juce::jmax(0, (int)messages.size() + numToAdd - numToStore);
        messages.erase(messages.begin(), std::next(messages.begin(), numToRemove));
        messages.insert(messages.end(), std::prev(end, numToAdd), end);

        newMessages.clear();
        newMessages.insert(newMessages.end(), std::prev(end, numToAdd), end);

        if (onChange != nullptr) { onChange(); }
    }
    void clear()
    {
        messages.clear();
        newMessages.clear();
        if (onChange != nullptr) { onChange(); }
    }
    const juce::MidiMessage& operator[](size_t ind) const 
    {
        return messages[ind];
    }
    size_t size() const 
    {
        return messages.size();
    }
    std::function<void()> onChange;
    std::vector<juce::MidiMessage> messages;
    std::vector<juce::MidiMessage> newMessages;
private:

    static constexpr auto numToStore = 8;
};