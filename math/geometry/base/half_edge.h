#ifndef MOZI_MATH_GEOMETRY_BASE_HALF_EDGE_MESH_H
#define MOZI_MATH_GEOMETRY_BASE_HALF_EDGE_MESH_H

namespace mozi {
namespace math {

template <typename FloatType, typename IndexType>
struct HalfEdge
{
    IndexType end_vertex_;
    IndexType opp_;
    IndexType face_;
    IndexType next_;
};

} // namespace math
} // namespace mozi

#endif
