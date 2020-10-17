#pragma once

#include "Triangle.h"
#include "Vector3.h"
#include <list>
#include <stack>
#include "assert.h"

// first front layer
// 		   |z
// 		   |
// 	3	   |	2
// 		   |
// -------------->y
// 		   |
// 		   |
// 	4	   |	1
// 		   |

// second back layer
// 		   |z
// 		   |
// 	7	   |	6
// 		   |
// -------------->y
// 		   |
// 		   |
// 	8	   |	5
// 		   |

namespace mfn
{

	bool isBetween(float value, float left, float right, float eps)
	{
		return (value - eps > left && value < right + eps);
	}

	template <typename T>
	class OctoTree;

	template <typename T>
	class OctoNode
	{
		OctoNode<T> *children_[8];
		std::list<Triangle<T>> data_;
		Vector3<T> origin_;
		T length_;

	public:
		OctoNode();

		friend class OctoTree<T>;

		~OctoNode();
	};

	template <typename T>
	class OctoTree
	{
		const float eps = 1E-07;
		const float minlength = 1E-04;
		OctoNode<T> *head_;
		const std::list<Triangle<T>> &triangles_;
		T length_;
		void findLength();
		int rec_collision(OctoNode<T> *&node, const Triangle<T> &triangle);

	public:
		OctoTree(const std::list<Triangle<T>> &triangles);
		void generateTree(OctoNode<T> *&node);
		void collision(OctoNode<T> *&node);
		OctoNode<T> *getHead()
		{
			return head_;
		}
		long k;

		void print(OctoNode<T> *&node);
		void destroyTree(OctoNode<T> *&node);
		~OctoTree();
	};

	template <typename T>
	inline OctoNode<T>::OctoNode() : length_(0)
	{
		for (int i = 0; i < 8; i++)
		{
			children_[i] = nullptr;
		}
	}

	template <typename T>
	inline OctoNode<T>::~OctoNode()
	{
		for (int i = 0; i < 8; i++)
		{
			if (children_[i] != nullptr)
			{
				delete children_[i];
			}
		}
	}

	template <typename T>
	inline OctoTree<T>::~OctoTree()
	{
		if (head_ != nullptr)
			delete head_;
	}

	template <typename T>
	inline OctoTree<T>::OctoTree(const std::list<Triangle<T>> &triangles) : triangles_(triangles),
																			length_(0),
																			k(0)
	{
		findLength();
		//std::cout << "Length: " << length_ << std::endl;
		head_ = new OctoNode<T>();
		head_->data_ = triangles_;
		head_->length_ = length_;
	}

	template <typename T>
	inline void OctoTree<T>::generateTree(OctoNode<T> *&node)
	{
		assert(node);

		int belong[3] = {-1, -1, -1};
		int count = 0;
		int tr_count = 0;
		int zone = 0;

		//std::cout << "The size of data is " << node->data_.size() << std::endl;

		if (node->length_ < minlength - eps)
			return;

		// Find triangles that belong to some certain zone
		for (auto triangle = node->data_.begin(); triangle != node->data_.end(); ++triangle)
		{
			count = 0;
			for (auto point : triangle->points_)
			{
				// Check if the point belongs to the boundary
				if (isBetween(point.x_, node->origin_.x_ - eps, node->origin_.x_ + eps, 0.f) ||
					isBetween(point.y_, node->origin_.y_ - eps, node->origin_.y_ + eps, 0.f) ||
					isBetween(point.z_, node->origin_.z_ - eps, node->origin_.z_ + eps, 0.f))
					break;

				if (!isBetween(point.y_, node->origin_.y_, node->origin_.y_ + node->length_, eps))
				{
					// Not 1, 2, 5, 6; only 3, 4, 7, 8
					if (!isBetween(point.x_, node->origin_.x_, node->origin_.x_ + node->length_, eps))
					{
						// Not 3, 4; only 7, 8
						if (!isBetween(point.z_, node->origin_.z_, node->origin_.z_ + node->length_, eps))
						{
							// It's 8
							belong[count] = 7;
						}
						else if (!isBetween(point.z_, node->origin_.z_ - node->length_, node->origin_.z_, eps))
						{
							// It's 7
							belong[count] = 6;
						}
					}
					else if (!isBetween(point.x_, node->origin_.x_ - node->length_, node->origin_.x_, eps))
					{
						// Not 7, 8; only 3, 4
						if (!isBetween(point.z_, node->origin_.z_, node->origin_.z_ + node->length_, eps))
						{
							// It's 4
							belong[count] = 3;
						}
						else if (!isBetween(point.z_, node->origin_.z_ - node->length_, node->origin_.z_, eps))
						{
							// It's 3
							belong[count] = 2;
						}
					}
				}
				else if (!isBetween(point.y_, node->origin_.y_ - node->length_, node->origin_.y_, eps))
				{
					// Not 3, 4, 7, 8; only 1, 2, 5, 6
					if (!isBetween(point.x_, node->origin_.x_, node->origin_.x_ + node->length_, eps))
					{
						// Not 1, 2; only 5, 6
						if (!isBetween(point.z_, node->origin_.z_, node->origin_.z_ + node->length_, eps))
						{
							// It's 5
							belong[count] = 4;
						}
						else if (!isBetween(point.z_, node->origin_.z_ - node->length_, node->origin_.z_, eps))
						{
							// It's 6
							belong[count] = 5;
						}
					}
					else if ((!isBetween(point.x_, node->origin_.x_ - node->length_, node->origin_.x_, eps)))
					{
						// Not 5, 6; only 1, 2
						if (!isBetween(point.z_, node->origin_.z_, node->origin_.z_ + node->length_, eps))
						{
							// It's 1
							belong[count] = 0;
						}
						else if (!isBetween(point.z_, node->origin_.z_ - node->length_, node->origin_.z_, eps))
						{
							// It's 2
							belong[count] = 1;
						}
					}
				}

				count++;
			}

			if ((belong[0] == belong[1]) && (belong[1] == belong[2]) && (belong[0] != -1))
			{
				tr_count++;
				zone = belong[0];

				if (node->children_[zone] == nullptr)
				{
					// If there is no this child yet
					node->children_[zone] = new OctoNode<T>;
					// Set its origin and length
					node->children_[zone]->length_ = node->length_ * 0.5;
					if ((zone == 0) || (zone == 1) || (zone == 5) || (zone == 4))
						node->children_[zone]->origin_.y_ = node->origin_.y_ + node->children_[zone]->length_;
					else
						node->children_[zone]->origin_.y_ = node->origin_.y_ - node->children_[zone]->length_;
					if ((zone == 0) || (zone == 1) || (zone == 2) || (zone == 3))
						node->children_[zone]->origin_.x_ = node->origin_.x_ + node->children_[zone]->length_;
					else
						node->children_[zone]->origin_.x_ = node->origin_.x_ - node->children_[zone]->length_;
					if ((zone == 1) || (zone == 2) || (zone == 6) || (zone == 5))
						node->children_[zone]->origin_.z_ = node->origin_.z_ + node->children_[zone]->length_;
					else
						node->children_[zone]->origin_.z_ = node->origin_.z_ - node->children_[zone]->length_;
				}

				// Push this triangle into its data
				node->children_[zone]->data_.push_back(*triangle);

				// Erase from the original array
				triangle = std::prev(triangle);
				node->data_.erase(std::next(triangle));
			}

			for (int i = 0; i < 3; i++)
				belong[i] = -1;
		}

		if (tr_count == 0 || tr_count == 1 || tr_count == 2)
			return;

		// Find triangles that belong to several zones
		for (auto triangle = node->data_.begin(); triangle != node->data_.end(); ++triangle)
		{
			int belong[8] = {1};

			T xmax = triangle->points_[0].x_;
			T xmin = xmax;
			T ymin = triangle->points_[0].y_;
			T ymax = ymin;
			T zmax = triangle->points_[0].z_;
			T zmin = zmax;

			// Find the max and min of each coordinate
			for (auto point : triangle->points_)
			{
				if (point.x_ - eps > xmax)
					xmax = point.x_;
				if (point.x_ < xmin - eps)
					xmin = point.x_;
				if (point.y_ - eps > ymax)
					ymax = point.y_;
				if (point.y_ < ymin - eps)
					ymin = point.y_;
				if (point.z_ - eps > zmax)
					zmax = point.z_;
				if (point.z_ < zmin - eps)
					zmin = point.z_;
			}

			if ((xmax - node->origin_.x_) * (node->origin_.x_ - xmin) > -eps)
			{
				// Same zone
				if (xmax > -eps)
				{
					belong[4] = 0;
					belong[5] = 0;
					belong[6] = 0;
					belong[7] = 0;
				}
				else
				{
					belong[0] = 0;
					belong[1] = 0;
					belong[2] = 0;
					belong[3] = 0;
				}
			}

			if ((ymax - node->origin_.y_) * (node->origin_.y_ - ymin) > -eps)
			{
				// Same zone
				if (ymax > -eps)
				{
					belong[2] = 0;
					belong[3] = 0;
					belong[6] = 0;
					belong[7] = 0;
				}
				else
				{
					belong[0] = 0;
					belong[1] = 0;
					belong[4] = 0;
					belong[5] = 0;
				}
			}

			if ((zmax - node->origin_.z_) * (node->origin_.z_ - zmin) > -eps)
			{
				// Same zone
				if (zmax > -eps)
				{
					belong[0] = 0;
					belong[3] = 0;
					belong[4] = 0;
					belong[7] = 0;
				}
				else
				{
					belong[1] = 0;
					belong[2] = 0;
					belong[5] = 0;
					belong[6] = 0;
				}
			}

			// Copy this triangle into all zones he belongs to
			for (int i = 0; i < 8; i++)
			{
				if (belong[i] == 1)
				{
					// Belongs to this zone
					zone = belong[i];
					if (node->children_[zone] == nullptr)
					{
						// If there is no this child yet
						node->children_[zone] = new OctoNode<T>;
						// Set its origin and length
						node->children_[zone]->length_ = node->length_ * 0.5;
						if ((zone == 0) || (zone == 1) || (zone == 5) || (zone == 4))
							node->children_[zone]->origin_.y_ = node->origin_.y_ + node->children_[zone]->length_;
						else
							node->children_[zone]->origin_.y_ = node->origin_.y_ - node->children_[zone]->length_;
						if ((zone == 0) || (zone == 1) || (zone == 2) || (zone == 3))
							node->children_[zone]->origin_.x_ = node->origin_.x_ + node->children_[zone]->length_;
						else
							node->children_[zone]->origin_.x_ = node->origin_.x_ - node->children_[zone]->length_;
						if ((zone == 1) || (zone == 2) || (zone == 6) || (zone == 5))
							node->children_[zone]->origin_.z_ = node->origin_.z_ + node->children_[zone]->length_;
						else
							node->children_[zone]->origin_.z_ = node->origin_.z_ - node->children_[zone]->length_;
					}

					// Push this triangle into its data
					node->children_[zone]->data_.push_back(*triangle);
				}
			}

			// Erase from the original array
			triangle = std::prev(triangle);
			node->data_.erase(std::next(triangle));

			for (int i = 0; i < 8; i++)
			{
				belong[i] = 1;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			if (node->children_[i] != nullptr)
			{
				generateTree(node->children_[i]);
			}
		}
	}

	template <typename T>
	inline void OctoTree<T>::findLength()
	{
		length_ = std::abs(triangles_.begin()->points_[0].x_);

		for (auto triangle : triangles_)
		{
			for (auto point : triangle.points_)
			{
				if (length_ < std::abs(point.x_) - eps)
					length_ = std::abs(point.x_);
				if (length_ < std::abs(point.y_) - eps)
					length_ = std::abs(point.y_);
				if (length_ < std::abs(point.z_) - eps)
					length_ = std::abs(point.z_);
			}
		}
	}

	template <typename T>
	inline int OctoTree<T>::rec_collision(OctoNode<T> *&node, const Triangle<T> &triangle)
	{
		assert(node);

		for (int i = 0; i < 8; i++)
		{
			if (node->children_[i] != nullptr)
			{
				if (rec_collision(node->children_[i], triangle) == 1)
					return 1;
			}
		}

		for (auto it1 = node->data_.begin(); it1 != node->data_.end(); ++it1)
		{
			k++;
			if (it1->is_collided(triangle))
			{
				std::cout << triangle.number << " " << it1->number << " ";
				return 1;
			}
		}

		return -1;
	}

	template <typename T>
	inline void OctoTree<T>::collision(OctoNode<T> *&node)
	{
		assert(node);

		for (auto it1 = node->data_.begin(); it1 != node->data_.end(); ++it1)
		{
			for (auto it2 = node->data_.begin(); it2 != node->data_.end(); ++it2)
			{
				if (it1 != it2)
				{
					k++;
					if (it1->is_collided(*it2))
					{
						std::cout << it1->number << " ";
						break;
					}
				}
			}

			for (int i = 0; i < 8; i++)
			{
				if (node->children_[i] != nullptr)
					if (rec_collision(node->children_[i], *it1) == 1)
						break;
			}
		}

		for (int i = 0; i < 8; i++)
		{
			if (node->children_[i] != nullptr)
				collision(node->children_[i]);
		}
	}

	template <typename T>
	inline void OctoTree<T>::print(OctoNode<T> *&node)
	{
		std::cout << "=====" << std::endl;
		std::cout << "Size: " << node->data_.size() << std::endl;
		std::cout << "Length: " << node->length_ << std::endl;
		std::cout << "Origin: x=" << node->origin_.x_ << ", y=" << node->origin_.y_ << ", z=" << node->origin_.z_ << std::endl;
		std::cout << "Data: " << std::endl;
		for (auto triangle : node->data_)
		{
			std::cout << triangle.number << "{";
			for (auto point : triangle.points_)
			{
				std::cout << "(" << point.x_ << ";" << point.y_ << ";" << point.z_ << "), ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "=====" << std::endl;
		for (int i = 0; i < 8; i++)
		{
			if (node->children_[i] != nullptr)
				print(node->children_[i]);
		}
	}
}