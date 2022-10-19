#ifndef MOZI_MATH_QUICK_HULL_H
#define MOZI_MATH_QUICK_HULL_H

namespace mozi {
namespace math {

struct DiagnosticsData
{
    size_t failed_horizon_edges;
    
    DiagnosticsData()
        : failed_horizon_edges(0)
    {}
};

template <typename T>
inline T defaultEps()
{
    return T{1} / T{10000000};
}

template <typename T>
class QuickHull
{
    private:
        using vec3 = Vector3<T>;
        T epsilon_;
        T epsilon_squared_;
        T scale_;
        bool planar_;
        std::vector<vec3> planar_point_cloud_temp_;
        VertexDataSource<T> vertex_data_;
        MeshBuilder<T> mesh_;
        std::array<size_t, 6> extreme_values_;
        DiagnosticsData diagnostics_;


};
 
} // namespace math
} // namespace mozi

#endif
