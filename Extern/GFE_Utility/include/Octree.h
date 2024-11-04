#ifndef Octree_H
#define Octree_H

#include <vector>
#include <bitset>
#include "Math/GFE_Vector.h"
#include <iostream>

using Vec3 = GFE::Vec3D;


template <typename Ele, std::size_t N>
struct OctreeNode;

template <typename TreeElement, std::size_t N>
class internal_tree;

template <typename T>
constexpr std::size_t recommendsize() {
	// recommend size 1024 bytes or so
    constexpr std::size_t size = 1024;
	if constexpr (sizeof (T) < size / 2)
		return size / sizeof (T) - 1;
	return 1;
}

template <
	typename TreeElement,
	typename ElementCentroid,
	typename ElementBox,
	typename ElementEqual = std::equal_to<TreeElement>,
	std::size_t __BucketSize = recommendsize<TreeElement>()
> class Octree;

//! each octree leaf node stores max N point
template <typename Ele, std::size_t N>
struct OctreeNode {
	Ele data[N];
	std::bitset<N> valid;
};


template <typename TreeElement, std::size_t N>
class internal_tree {
public:
	using Ele = TreeElement;
	using dataNode = OctreeNode<TreeElement, N>;


	internal_tree(const Vec3& origin, const Vec3& halfDimension)
		: origin(origin), halfDimension(halfDimension), data(nullptr) {
			for (int i = 0; i < 8; ++i)
				children[i] = nullptr;
		}

	internal_tree(const internal_tree&) = delete;
	//Octree(const Octree& copy)
	//	: origin(copy.origin), halfDimension(copy.halfDimension), data(copy.data) {
	//
	//	}

	~internal_tree() {
		for(int i=0; i<8; ++i)
			delete children[i];
        delete data;
	}

	bool isLeafNode() const {
		for (int i = 0; i < 8; ++i) {
			if (children[i])
				return false;
		}
		return true;
	}

	Vec3 origin;         	//! The physical center of this node
	Vec3 halfDimension;  	//! Half the width/height/depth of this node

	// The tree has up to eight children and can additionally store
	// a point, though in many applications only, the leaves will store data.
	internal_tree* children[8];	//! Pointers to child octants
	dataNode* data;	//! Data point to be stored at a node

	/*
		Children follow a predictable pattern to make accesses simple.
		Here, - means less than 'origin' in that dimension, + means greater than.
		child:	0 1 2 3 4 5 6 7
		x:      - - - - + + + +
		y:      - - + + - - + +
		z:      - + - + - + - +
	 */

};

template <typename Ele, std::size_t N>
static void try_newChild(internal_tree<Ele, N>* internal, int index) {

	if (internal->children[index] != nullptr)
		return;
	auto&& half = internal->halfDimension * 0.5;
	// Compute new bounding box for this child
	Vec3 newOrigin = internal->origin;
	newOrigin.x += half.x * (index&4 ? 1 : -1);
	newOrigin.y += half.y * (index&2 ? 1 : -1);
	newOrigin.z += half.z * (index&1 ? 1 : -1);
	internal->children[index] = new internal_tree<Ele, N>(newOrigin, half);
}

template <typename Ele, std::size_t N>
static int getOctant(internal_tree<Ele, N>* internal, const Vec3& point) {
	int oct = 0;
	if (point.x >= internal->origin.x) oct |= 4;
	if (point.y >= internal->origin.y) oct |= 2;
	if (point.z >= internal->origin.z) oct |= 1;
	return oct;
}

template <typename Ele, std::size_t N, typename Func>
static std::pair<OctreeNode<Ele, N>*, std::size_t> find(internal_tree<Ele, N>* internal, const Ele& element, const Vec3& pos, const Func& equal = {}) {

	if (internal == nullptr)
		return {nullptr, -1};

	if (internal->isLeafNode()) {
		if (internal->data == nullptr)
			return {nullptr, -1};
		else {
			for (auto i = 0; i < N; ++i) {
				if (internal->data->valid.test(i)) {
					if (equal(internal->data->data[i], element))
						return {internal->data, i};
				}
			}
			return {nullptr, -1};
		}
	}
	else {
		return find(internal->children[getOctant(internal, pos)], element, pos, equal);
	}
}

template <typename Ele, std::size_t N, typename Func, typename ... Args>
static void insert(internal_tree<Ele, N>* internal, const Func& func, Args ...para) {

	if (internal->isLeafNode()) {
		if (internal->data == nullptr) {
			internal->data = new typename internal_tree<Ele, N>::dataNode;
			internal->data->data[0] = Ele(std::forward<Args&&>(para)...);
			internal->data->valid.set(0);
		}
		else {
			auto& data = internal->data;
			if (!data->valid.all()) {
				for (auto i = 0; i < N; ++i) {
					if (!data->valid.test(i)) {
						data->data[i] = Ele(std::forward<Args&&>(para)...);
						data->valid.set(i);
						break;
					}
				}
			}
			else {	// split this node
				for (int i = 0; i < N; ++i) {
					int index = getOctant(internal, func(data->data[i]));
					try_newChild(internal, index);
					insert(internal->children[index], func, std::move(data->data[i]));
				}
				auto ele = Ele(std::forward<Args&&>(para)...);
				int index = getOctant(internal, func(ele));
				try_newChild(internal, index);
				insert(internal->children[index], func, std::move(ele));

                delete data;
            	data = nullptr;
			}
		}
	}
	else {
		auto ele = Ele(std::forward<Args&&>(para)...);
		int index = getOctant(internal, func(ele));
		try_newChild(internal, index);
		insert(internal->children[index], func, std::move(ele));
	}

}

template <typename Ele, std::size_t N, typename Func>
static void getInside(internal_tree<Ele, N>* internal, const Vec3& bmin, const Vec3& bmax, const Func& func, std::vector<Ele>& result) {
	// If we're at a leaf node, just see if the current data point is inside
	// the query bounding box
	if (internal->isLeafNode()) {
		auto data = internal->data;
		if (data != nullptr) {
			for (int i = 0; i < N; ++i) {
				if (data->valid.test(i)) {
					auto&& [pmin, pmax] = func(data->data[i]);
					if (pmax.x > bmax.x || pmax.y > bmax.y || pmax.z > bmax.z)
                       	continue;
					if (pmin.x < bmin.x || pmin.y < bmin.y || pmin.z < bmin.z)
                       	continue;
					result.push_back(data->data[i]);
				}
			}
		}
	}
	else {
		// We're at an interior node of the tree. We will check to see if
		// the query bounding box lies outside the octants of this node.
		auto children = internal->children;
		for(int i = 0; i < 8; ++i) {

			if (children[i] == nullptr)
                continue;
			// Compute the min/max corners of this child octant
			Vec3 cmax = children[i]->origin + children[i]->halfDimension;
			Vec3 cmin = children[i]->origin - children[i]->halfDimension;

			// If the query rectangle is outside the child's bounding box,
			// then continue
			if (cmax.x < bmin.x || cmax.y < bmin.y || cmax.z < bmin.z)
				continue;
			if (cmin.x > bmax.x || cmin.y > bmax.y || cmin.z > bmax.z)
				continue;

			// At this point, we've determined that this child is intersecting
			// the query bounding box
			getInside(children[i], bmin, bmax, func, result);
		}
	}
}

template <typename Ele, std::size_t N, typename Func>
static void getIntersect(internal_tree<Ele, N>* internal, const Vec3& bmin, const Vec3& bmax, const Func& func, std::vector<Ele>& result) {
	if (internal->isLeafNode()) {
		auto data = internal->data;
		if (data != nullptr) {
			for (int i = 0; i < N; ++i) {
				if (data->valid.test(i)) {
					auto&& [pmin, pmax] = func(data->data[i]);
					if (pmin.x >= bmax.x || pmin.y >= bmax.y || pmin.z >= bmax.z)
                      	continue;
					if (pmax.x <= bmin.x || pmax.y <= bmin.y || pmax.z <= bmin.z)
                       	continue;
					result.push_back(data->data[i]);
				}
			}
		}
	}
	else {
		auto children = internal->children;
		for(int i = 0; i < 8; ++i) {

			if (children[i] == nullptr)
                continue;
			// Compute the min/max corners of this child octant
			Vec3 cmax = children[i]->origin + children[i]->halfDimension;
			Vec3 cmin = children[i]->origin - children[i]->halfDimension;

			// If the query rectangle is outside the child's bounding box,
			// then continue
			if (cmax.x < bmin.x || cmax.y < bmin.y || cmax.z < bmin.z)
				continue;
			if (cmin.x > bmax.x || cmin.y > bmax.y || cmin.z > bmax.z)
				continue;

			getIntersect(children[i], bmin, bmax, func, result);
		}
	}
}

//! 模板参数
//! TreeElement -> 数据类型
//! ElementCentroid 计算Element中心仿函数类型
//! ElementEqual 判断Element相等仿函数类型
//! ElementBox 计算Element bounding box仿函数类型
//! __BucketSize 每个数据节点存储数据个数
//!
//! 目前实现允许插入重复数据
//! bug: 如果插入数据重复个数 > __BucketSize, 暂未解决
template <typename TreeElement, typename ElementCentroid, typename ElementBox, typename ElementEqual, std::size_t __BucketSize>
class Octree {
public:

	//Octree() : centroid_func_(), equal_func_(), box_func_(), root_(new internal_tree<TreeElement, __BucketSize>()) {}
	Octree(const GFE::Vec3D& origin, const GFE::Vec3D& half_dim) :
		centroid_func_(),
		equal_func_(),
		box_func_(),
		root_(new internal_tree<TreeElement, __BucketSize>(origin, half_dim))
	{
		if (half_dim.x <= 0 || half_dim.y <= 0 || half_dim.z <= 0) {
			throw std::exception("Error octree dimension");
		}
	}

	Octree(const Octree&) = delete;
	Octree(Octree&& rhs) {
		centroid_func_ = rhs.centroid_func_;
		box_func_ = rhs.box_func_;
		equal_func_ = rhs.equal_func_;
		root_ = rhs.root_;
		rhs.root_ = nullptr;
	}
	Octree& operator= (const Octree&) = delete;
	Octree& operator= (Octree&& rhs) {
		centroid_func_ = rhs.centroid_func_;
		box_func_ = rhs.box_func_;
		equal_func_ = rhs.equal_func_;
		root_ = rhs.root_;
		rhs.root_ = nullptr;
		return *this;
	};
	~Octree() { delete root_; }


	inline void SetCentroidFunctor(const ElementCentroid& func) { centroid_func_ = func; }
	inline void SetEqualFunctor(const ElementEqual& func) { equal_func_ = func; }
	inline void SetBoxFunctor(const ElementBox& func) { box_func_ = func; }

	//! centroid结果作为参考点插入数据
	inline void Insert(const TreeElement& ele) {
		insert(root_, centroid_func_, ele);
	}

	//! 如果找到相同的元素，则移除
	inline void Remove(const TreeElement& ele) {
		auto [node, id] = find(root_, ele, centroid_func_(ele), equal_func_);
		if (node)
			node->valid.reset(id);
	}

	//! 如果找到相同的元素，则取代
	inline void Replace(const TreeElement& ele, const TreeElement& newEle) {
		auto [node, id] = find(root_, ele, centroid_func_(ele), equal_func_);
		if (node) {
			node->valid.reset(id);
			Insert(newEle);
		}
	}

	//!
	inline std::vector<TreeElement> GetInsideBox(const Vec3& bmin, const Vec3& bmax) const {
		std::vector<TreeElement> ret;
		getInside(root_, bmin, bmax, box_func_, ret);
		return ret;
	}

	inline std::vector<TreeElement> GetInsideBox(const Vec3& bmin, const Vec3& bmax, ElementBox func) const {
		std::vector<TreeElement> ret;
		getInside(root_, bmin, bmax, func, ret);
		return ret;
	}

    inline std::vector<TreeElement> GetIntersectWithBox(const Vec3& bmin, const Vec3& bmax) const {
		std::vector<TreeElement> ret;
		getIntersect(root_, bmin, bmax, box_func_, ret);
		return ret;
    }

    inline std::vector<TreeElement> GetIntersectWithBox(const Vec3& bmin, const Vec3& bmax, ElementBox func) const {
		std::vector<TreeElement> ret;
		getIntersect(root_, bmin, bmax, func, ret);
		return ret;
    }

	inline std::vector<TreeElement> GetAll() const {
		return GetInsideBox(root_->origin - root_->halfDimension, root_->origin + root_->halfDimension);
	}

	inline const GFE::Vec3D& GetOrigin() const {
		return root_->origin;
	}
	inline const GFE::Vec3D& GetDimension() const {
		return root_->halfDimension;
	}

private:

	ElementCentroid centroid_func_;
	ElementEqual equal_func_;
	ElementBox box_func_;
	internal_tree<TreeElement, __BucketSize>* root_;

};

#endif
