#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <string>
#include <optional>
#include "instrument.hpp"
#include "tuning.hpp"
#include "pipeline.hpp"

EMSCRIPTEN_KEEPALIVE
extern "C" const char *process_data(
    const char *filename,
    const char *string1,
    const char *string2,
    int in_num_frets,
    int out_num_frets)
{
    static std::string result;

    try
    {
        std::string tab_data = std::string(filename);
        std::optional<Tuning> input_tuning = std::nullopt;
        std::optional<Tuning> output_tuning = std::nullopt;

        auto str1 = std::string(string1);
        if (!str1.empty())
        {
            input_tuning = Tuning::create_tuning(str1);
            if (!input_tuning)
            {
                result = std::string("Could not create input tuning. Please supply correct notes.");
                return result.c_str();
            }

        }

        auto str2 = std::string(string2);
        if (!str2.empty())
        {
            output_tuning = Tuning::create_tuning(str2);
            if (!output_tuning)
            {
                result = "Could not create output tuning. Please supply correct notes.";
                return result.c_str();
            }
        }

        StringInstrument output_instrument{out_num_frets, output_tuning.value()};
        StringInstrument input_instrument{in_num_frets, input_tuning.value()};
        TabConverterPipeline pipeline(input_instrument,output_instrument,Transformer::TransformationOptions{});

        auto proc = pipeline.processTab(tab_data);
        if (proc.isSuccess())
        {
            result = proc.getValue();
            return result.c_str();
        }
        else{

            result = proc.getError();
            return result.c_str();
        }
    }
    catch (const std::exception &e)
    {
        result = std::string("Error: ") + e.what();
        return result.c_str();
    }
}
