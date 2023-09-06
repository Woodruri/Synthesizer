#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>

const int SAMPLE_RATE = 44100;
const int NUM_CHANNELS = 2;
const int SAMPLES_PER_FRAME = 512;
const double FREQUENCY = 220.0;
const double AMPLITUDE = 32767.0;

void AudioCallback(void* userdata, Uint8* stream, int len) {
    Sint16* audioStream = reinterpret_cast<Sint16*>(stream);
    double angularFrequency = 2.0 * M_PI * FREQUENCY / SAMPLE_RATE;
    static int t = 0;

    for (int i = 0; i < len / sizeof(Sint16); i += NUM_CHANNELS) {
        double sample = AMPLITUDE * sin(angularFrequency * t++);
        audioStream[i] = static_cast<Sint16>(sample);
        audioStream[i + 1] = static_cast<Sint16>(sample);
    }
}

int main() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL initialization error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_AudioSpec audioSpec;
    audioSpec.freq = SAMPLE_RATE;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = NUM_CHANNELS;
    audioSpec.samples = SAMPLES_PER_FRAME;
    audioSpec.callback = AudioCallback;
    audioSpec.userdata = nullptr;

    SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, 0);
    if (audioDevice == 0) {
        std::cerr << "Audio device opening error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_PauseAudioDevice(audioDevice, 0);

    SDL_Event event;
    bool quit = false;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }
    }

    SDL_CloseAudioDevice(audioDevice);
    SDL_Quit();
    return 0;
}
