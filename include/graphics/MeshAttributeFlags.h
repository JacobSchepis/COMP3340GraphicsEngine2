#pragma once

#include <type_traits>  // Needed for std::underlying_type

enum class MeshAttributeFlags : unsigned int {
    None = 0,
    Position = 1 << 0,  // 0001
    Normal = 1 << 1,  // 0010
    Color = 1 << 2,  // 0100
    TexCoord = 1 << 3,  // 1000
};

// Enable bitwise operations for the enum class
inline MeshAttributeFlags operator|(MeshAttributeFlags lhs, MeshAttributeFlags rhs) {
    return static_cast<MeshAttributeFlags>(
        static_cast<std::underlying_type<MeshAttributeFlags>::type>(lhs) |
        static_cast<std::underlying_type<MeshAttributeFlags>::type>(rhs)
        );
}

inline MeshAttributeFlags operator&(MeshAttributeFlags lhs, MeshAttributeFlags rhs) {
    return static_cast<MeshAttributeFlags>(
        static_cast<std::underlying_type<MeshAttributeFlags>::type>(lhs) &
        static_cast<std::underlying_type<MeshAttributeFlags>::type>(rhs)
        );
}

// Check if a specific flag is set
inline bool hasFlag(MeshAttributeFlags flags, MeshAttributeFlags flag) {
    return (static_cast<std::underlying_type<MeshAttributeFlags>::type>(flags) &
        static_cast<std::underlying_type<MeshAttributeFlags>::type>(flag)) != 0;
}
