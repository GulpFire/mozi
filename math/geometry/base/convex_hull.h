#ifndef MOZI_MATH_GEOMETRY_CONVEX_HULL_H
#define MOZI_MATH_GEOMETRY_CONVEX_HULL_H

#include "vector3.h"

#include <memory>
#include <vector>

namespace mozi {
namespace math {

template <typename T>
class VertexDataSource
{
    public:
        VertexDataSource(const Vector3<T>* ptr, size_t count)
            : ptr_(ptr)
            , size_(size)
        {}

        VertexDataSource(const std::vector<Vector3<T>>& vec)
            : ptr_(&vec[0])
            , size_(vec.size())
        {}

        VertexDataSource()
            : ptr_(nullptr)
            , size_(0)
        {}

        inline size_t size() const { return size_; }
        
        inline const Vector3<T>& operator[](size_t index) const
        {
            return ptr_[index];
        }

        inline const Vector3<T>* begin() const
        {
            return ptr_;
        }

        inline const Vector3<T>* end() const
        {
            return ptr_ + size_;
        }

    private:
        const Vector3<T>* ptr_;
        size_t size_;
};

class ConvexHull
{
    public:
        // copy construct
        ConvexHull(const ConvexHull& other)
        {
            indices_ = other.indices_;
            if (other.optimized_vertex_buf_)
            {
                optimized_vertex_buf_.reset(
                        new std::vector<Vector3<T>>(*other.optimized_vertex_buf_));
                vertices_ = VertexDataSource<T>(*optimized_vertex_buf_);
            }
            else
            {
                vertices_ = other.vertices_;
            }
        }

        // assignment construct
        inline ConvexHull& operator=(const ConvexHull& other)
        {
            if (&other == this)
            {
                return *this;
            }
            
            indices_ = other.indices_;
            
            if (other.optimized_vertex_buf_)
            {
                optimized_vertex_buf_.reset(
                        new std::vector<Vector3<T>>(*other.optimized_vertex_buf_));
                vertices_ = VertexDataSource<T>(*optimized_vertex_buf_);
            }
            else
            {
                vertices_ = other.vertices_;
            }

            return *this;
        }

        // move construct
        ConvexHull(ConvexHull&& other)
        {
            indices_ = std::move(other.indices_);
            if (other.optimized_vertex_buf_)
            {
                optimized_vertex_buf = std::move(other.optimized_vertex_buf_);
                other.vertices_ = VertexDataSource<T>();
                vertices_ = VertexDataSource<T>(*optimized_vertex_buf_);
            }
            else
            {
                vertices_ = other.vertices_;
            }
        }

        // forward construct
        inline ConvexHull& operator=(ConvexHull&& other)
        {
            if (&other == this)
            {
                return *this;
            }

            indices_ = std::move(other.indices_);

            if (other.optimized_vertex_buf_)
            {
                optimized_vertex_buf_ = std::move(other.optimized_vertex_buf_);
                other.vertices_ = VertexDataSource<T>();
                vertices_ = VertexDataSource<T>(*optimized_vertex_buf_);
            }
            else
            {
                vertices_ = other.vertices_;
            }
            return *this;
        }

        inline std::vector<size_t>& getIndexBuffer()
        {
            return indices_;
        }

        inline const std::vector<size_t>& getIndexBuffer() const 
        {
            return indices_;
        }

        inline VertexDataSource<T>& getVertexBuffer()
        {
            return vertices_;
        }

        inline const VertexDataSource<T>& getVertexBuffer() const
        {
            return vertices_;
        }

    private:
        std::unique_ptr<std::vector<Vector3>> optimized_vertex_buf_;
        VertexDataSource<T> vertices_;
        std::vector<size_t> indices_;
};
}
} // namespace mozi

#endif
