#include "MainComponent.h"
#include "ContextMIDIComponent.h"

//==============================================================================
MainComponent::MainComponent() 
{
    setSize(1024, 800);
    addAndMakeVisible(contextMIDIComponent);
}


MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    juce::Rectangle<int> boundsRectangle(0, 0, this->getWidth()-200, this->getHeight()-200);
    boundsRectangle.setCentre(juce::roundToInt(this->getWidth()/2), juce::roundToInt(this->getHeight()/2));
    
    contextMIDIComponent.setBounds(boundsRectangle);
    contextMIDIComponent.setAlwaysOnTop(true);

}
