#include <chrono>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "algorithms.h"

enum TEST_MODE
{
    STD_MINMAX = 0,
    CUSTOM_MINMAX,
    EARLY_RETURN,
    OPT_CLIP_DET,
    STD_CLAMP,
    STD_CLAMP_DIR_ACCESS,
    CUSTOM_CLAMP,
    STD_FILL,
    MEMSET_FILL,
    CUSTOM_FILL,
    MAX_MODES  
};

constexpr int ITERATIONS   = 10000;
constexpr int SUB_ITERATIONS =  25;

void print_results(const std::vector<int>& results)
{
    std::cout << "std::minmax_element:\t\t"      << results[STD_MINMAX] << "\tns" << std::endl;
    std::cout << "Custom minmax_element:\t\t"    << results[CUSTOM_MINMAX] << "\tns" << std::endl;
    std::cout << "Early return detection:\t\t"   << results[EARLY_RETURN] << "\tns" << std::endl;
    std::cout << "Optimised clip detect:\t\t"    << results[OPT_CLIP_DET] << "\tns" << std::endl;
    std::cout << "std::clamp:\t\t\t"             << results[STD_CLAMP] << "\tns" << std::endl;
    std::cout << "std::clamp(direct_access):\t"  << results[STD_CLAMP_DIR_ACCESS] << "\tns" << std::endl;
    std::cout << "Custom clamp:\t\t\t"           << results[CUSTOM_CLAMP] << "\tns" << std::endl;
    std::cout << "std::fill:\t\t\t"              << results[STD_FILL] << "\tns" << std::endl;
    std::cout << "memset fill:\t\t\t"                 << results[MEMSET_FILL] << "\tns" << std::endl;
    std::cout << "Custom fill:\t\t\t"            << results[CUSTOM_FILL] << "\tns" << std::endl;
}

float float_rand(float min, float max)
{
    return (((std::rand()) * (max - min)) / static_cast<float>(RAND_MAX)) + min;
}

/* Fill the buffer with random values, linearily distributed between
 * -peak_value and peak_value */
void fill_buffer(AudioBuffer& buffer, float peak_value)
{
    for (auto& sample : buffer)
    {
        sample = float_rand(-peak_value, peak_value);
    }
}

void run_test(float clip_ratio)
{
    float fill_value = 0;
    std::vector<int> results;
    std::array<AudioBuffer, SUB_ITERATIONS> buffers;
    volatile int count = 0;
    for (int mode = STD_MINMAX; mode < MAX_MODES; ++mode)
    {
        auto timing = std::chrono::nanoseconds(0); 
        for (int i = 0; i < ITERATIONS; ++i)
        {
            for (auto& buffer : buffers)
            {
                fill_buffer(buffer, 1.0f/(1.0f - clip_ratio));
            }
            /* Since we are dealing with very small time scales, the overhead of getting
             * the current time is not negligible, hence we average over SUB_ITERATIONS calls */
            auto start_time = std::chrono::high_resolution_clock::now();
            for (auto& buffer : buffers)
            {
                switch (mode)
                {
                    case STD_MINMAX:
                    {
                        count += clip_detect_std_alg(buffer);
                        break;
                    }
                    case EARLY_RETURN:
                    {
                        count += clip_detect_early_return(buffer);
                        break;
                    }
                    case CUSTOM_MINMAX:
                    {
                        count += clip_detection_custom_minmax(buffer);
                        break;
                    }
                    case OPT_CLIP_DET:
                    {
                        count += clip_detect_optimised(buffer);
                        break;
                    }
                    case STD_CLAMP:
                    {
                        clamp_alg(buffer);
                        break;
                    }
                    case STD_CLAMP_DIR_ACCESS:
                    {
                        clamp_alg_direct_access(buffer);
                        break;
                    }
                    case CUSTOM_CLAMP:
                    {
                        custom_clamp(buffer);
                        break;
                    }
                    case STD_FILL:
                    {
                        alg_fill(buffer, fill_value);
                        break;
                    }
                    case MEMSET_FILL:
                    {
                        memset_fill(buffer, fill_value);
                        break;
                    }
                    case CUSTOM_FILL:
                    {
                        custom_fill(buffer, fill_value);
                        break;

                    }
                }
            }
            auto stop_time = std::chrono::high_resolution_clock::now();
            timing += std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time);          
        }
        results.push_back(timing.count() / (ITERATIONS * SUB_ITERATIONS));
    }
    print_results(results);
}

int main()
{
    std::cout << "0% clipped samples:\n-------------------------" << std::endl;
    run_test(0.0);
    std::cout << "\n1% clipped samples:\n-------------------------" << std::endl;
    run_test(0.01);
    std::cout << "\n50% clipped samples:\n-------------------------" << std::endl;
    run_test(0.5);
    return 0;
}