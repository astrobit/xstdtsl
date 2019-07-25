#pragma once
#ifndef __XSTDTSL_ENUMS_HPP
#define __XSTDTSL_ENUMS_HPP

namespace xstdtsl
{
	namespace binary_trees
	{
		/// 
		/// enumeration to describe whether the iteration begins at the beginning or end of the tree
		///
		enum class start_point {
				beginning, ///< iteration will begin at the start of the tree
				root, ///< iteration will begin at the root of the tree
				end ///< iteration will begin at the end of the tree
				};
	}

	namespace maps
	{
		/// 
		/// enumeration to describe whether the iteration begins at the beginning or end of the tree
		///
		enum class start_point {
				beginning, ///< iteration will begin at the start of the map
				end ///< iteration will begin at the end of the map
				};
	}

	namespace lists
	{
		/// 
		/// enumeration to describe whether the iteration begins at the beginning or end of the list
		///
		enum class start_point {
				beginning, ///< iteration will begin at the start of the list
				end ///< iteration will begin at the end of the list
				};
	}
}
#endif //#ifndef __XSTDTSL_ENUMS_HPP
