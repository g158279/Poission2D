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

        PreProcess preProcess;
        try
        {
            preProcess.readFromJson(argv[1]);
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return 3; // TASK_JSON_PARSE_FAILED
        }

        std::unique_ptr<Mesh> pMesh;
        try
        {
            std::string meshShape = preProcess.getDef().shape;
            std::transform(meshShape.begin(), meshShape.end(), meshShape.begin(), ::tolower);
            if (meshShape == "triangular" || meshShape == "tri" || meshShape == "t")
            {
                pMesh = std::make_unique<TriangularMesh>(preProcess.getDef());
            }
            else if (meshShape == "rectangular" || meshShape == "rect" || meshShape == "r")
            {
                pMesh = std::make_unique<RectangularMesh>(preProcess.getDef());
            }
            else
            {
                std::cerr << "Wrong mesh name." << std::endl;
                return 4;
            }
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << std::endl;
            return 4; // MESH_CONSTRUCTION_FAILED
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
