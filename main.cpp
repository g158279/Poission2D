#include "Poisson2DLib/RunPoisson2D.h"

using namespace CPPPOISSON;

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        const std::filesystem::path jsonPath{ argv[1] };
        if (!exists(jsonPath))
        {
            return 2; // TASK_JSON_NOT_EXIST
        }

        const std::filesystem::path exePath(argv[0]);

        auto pTask = std::make_unique<RunPoisson2D>(argc, argv);

        try
        {
            bool readJsonSucceeds = pTask->readFromJson(argv[1]);
            if (!readJsonSucceeds)
            {
                std::cout << "ERROR: Loading .json file failed!" << std::endl;
                return 4; // TASK_JSON_PARSE_FAILED
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return 4; // TASK_JSON_PARSE_FAILED
        }

        bool simulationSucceeds = pTask->simulate();

        if (!simulationSucceeds)
        {
            std::cout << "ERROR: Simulation failed!" << std::endl;
            return 7; // SIMULATION_FAILED
        }

        std::cout << "Simulation finished successfully!" << std::endl;
        return 0; // SIMULATION_SUCCEEDED
    }
    else
    {
        std::cout << "Usage: " << argv[0] << std::endl;
        return 1; // WRONG_ARGC
    }
}
