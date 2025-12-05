#include <gtest/gtest.h>
#include "mesh.h"
#include <vector>

using namespace CPPPOISSON;

TEST(RectangularMeshTest, ElementGeneration) {
    RectangularMesh mesh(2.0, 1.0, 4, 2);

    EXPECT_EQ(mesh.m_elements.size(), 3);

    std::vector<int> element1 = mesh.m_elements[0];
    EXPECT_EQ(element1.size(), 4);
    EXPECT_EQ(element1[0], 0);
    EXPECT_EQ(element1[1], 1);
    EXPECT_EQ(element1[2], 5);
    EXPECT_EQ(element1[3], 4);
}

TEST(TriangularMeshTest, ElementGeneration) {
    TriangularMesh mesh(2.0, 1.0, 4, 2);

    EXPECT_EQ(mesh.m_elements.size(), 6);

    std::vector<int> element1 = mesh.m_elements[0];
    EXPECT_EQ(element1.size(), 3);
    EXPECT_EQ(element1[0], 0);
    EXPECT_EQ(element1[1], 1);
    EXPECT_EQ(element1[2], 4);
}

TEST(MeshTest, MeshPointsInitialization) {
    RectangularMesh mesh(2.0, 1.0, 4, 2);

    EXPECT_EQ(mesh.m_points.size(), 8);

    EXPECT_DOUBLE_EQ(mesh.m_points[0][0], 0.0);
    EXPECT_DOUBLE_EQ(mesh.m_points[0][1], 0.0);

    EXPECT_DOUBLE_EQ(mesh.m_points[7][0], 2.0);  // x = 2
    EXPECT_DOUBLE_EQ(mesh.m_points[7][1], 1.0);  // y = 1
}

TEST(MeshTest, BoundaryNodes) {
    RectangularMesh mesh(2.0, 1.0, 4, 2);

    std::vector<int> leftNodes = mesh.getBoundaryNodes("left");
    EXPECT_EQ(leftNodes.size(), 2);

    EXPECT_EQ(leftNodes[0], 0);
    EXPECT_EQ(leftNodes[1], 4);

    std::vector<int> rightNodes = mesh.getBoundaryNodes("right");
    EXPECT_EQ(rightNodes.size(), 2);

    EXPECT_EQ(rightNodes[0], 3);
    EXPECT_EQ(rightNodes[1], 7);
}

TEST(MeshTest, WrongBoundaryName) {
    RectangularMesh mesh(2.0, 1.0, 4, 2);

    std::vector<int> wrongNodes = mesh.getBoundaryNodes("middle");

    EXPECT_EQ(wrongNodes.size(), 0);
}
