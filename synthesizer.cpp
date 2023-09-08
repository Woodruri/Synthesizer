#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>


//constants
const int SAMPLE_RATE = 44100;
const int NUM_CHANNELS = 2;
const int SAMPLES_PER_FRAME = 1024;
const double AMPLITUDE = 32767.0;

//will change, defaults to A
int frequency = 440.0;

//prototypes
void AudioCallback(void* userdata, Uint8* stream, int len);

int main(int argc, char* argv[]) {
    
    if (SDL_Init(SDL_INIT_AUDIO) < 0){
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Main program loop
    bool quit = false;
    while (!quit) {
        // Display a prompt and get user input
        std::cout << "Enter frequency in Hz (0 to quit): ";
        std::cin >> frequency;

        if (frequency == 0) {
            // User entered 0 to quit
            quit = true;
        } else {
            // Update the audio specification with the new frequency
            SDL_AudioSpec audioSpec;
            audioSpec.freq = SAMPLE_RATE;
            audioSpec.format = AUDIO_S16SYS;
            audioSpec.channels = NUM_CHANNELS;
            audioSpec.samples = SAMPLES_PER_FRAME;
            audioSpec.callback = AudioCallback;
            audioSpec.userdata = nullptr;

            // Open the audio device with the updated audio specification
            SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, 0);
            if (audioDevice == 0) {
                std::cerr << "Failed to open audio device: " << SDL_GetError() << std::endl;
                break;
            }

            // Print the selected frequency
            std::cout << "Playing sine wave at " << frequency << " Hz. Press 0 to quit." << std::endl;

            // Unpause the audio device to start audio playback
            SDL_PauseAudioDevice(audioDevice, 0);

            // Wait for the user to press 0 to quit
            while (frequency != 0) {
                std::cin >> frequency;
            }

            // Close the audio device
            SDL_CloseAudioDevice(audioDevice);
        }
    }

    // Quit SDL
    SDL_Quit();

    return 0;
} 


void AudioCallback(void* userdata, Uint8* stream, int len) {
    Sint16* audioStream = reinterpret_cast<Sint16*>(stream);
    double angularFrequency = 2.0 * M_PI * frequency / SAMPLE_RATE;
    static int t = 0;

    for (int i = 0; i < len / sizeof(Sint16); i += NUM_CHANNELS) {
        double sample = AMPLITUDE * sin(angularFrequency * t++);
        audioStream[i] = static_cast<Sint16>(sample);
        audioStream[i + 1] = static_cast<Sint16>(sample);
    }
}