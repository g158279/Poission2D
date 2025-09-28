#include "Poisson2DLib/preprocess.h"
#include "Poisson2DLib/mesh.h"
//#include "Poisson2DLib/RunPoisson2D.h"

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

        try
        {
            PreProcess preProcess;
            preProcess.readFromJson(argv[1]);
            Mesh mesh(preProcess.getDef());
            //RunPoisson2D task(argc, argv);
            //bool simulationSucceeds = task.simulate();
        }
        catch (const std::invalid_argument& e)
        {
            std::cout << e.what() << std::endl;
            return 3; // WRONG_ARGUMENTS
        }
        catch (const std::runtime_error& e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "ERROR: Simulation failed!" << std::endl;
            return 4; // SIMULATION_FAILED
        }


        //RunPoisson2D task(argc, argv);
        //bool simulationSucceeds = task.simulate();

        //if (!simulationSucceeds)
        //{
        //    std::cout << "ERROR: Simulation failed!" << std::endl;
        //    return 7; // SIMULATION_FAILED
        //}

        std::cout << "Simulation finished successfully!" << std::endl;
        return 0; // SIMULATION_SUCCEEDED
    }
    else
    {
        std::cout << "Usage: " << argv[0] << std::endl;
        return 1; // WRONG_ARGC
    }
}
