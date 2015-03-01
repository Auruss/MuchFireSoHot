#pragma once

#include <cstdio>
#include <typeinfo>
#include <vector>
#include <stddef.h>

#define LIVELOG_REFL_SCALAR 0
#define LIVELOG_REFL_ARRAY 1

namespace Common {
    namespace LiveLog {

        class ReflObject {

        public:
            struct MemberMeta {
                int offset;
                int size;
                const char* type_name;
                const char* second_type_name;
                const char* name;
                int mem;
            };

            struct GroupMeta {
                int offset;
                const char* name;
                ReflObject* obj;
            };

        public:
            /**
            * Initializes the Reflection object
            */
            template <class T>
            void init();

            /**
            * Adds an member to the object
            *
            * @param name Name of the member (the name you use to access the member)
            * @param as specs. the memory layout of the type (ex. vector would be an ARRAY)
            */
            template <class T>
            void addMember(const char* name, int offset, int as = LIVELOG_REFL_SCALAR);

            template <class T, class I>
            void addVectorMember(const char* name, int offset);

            /**
            * Adds an other reflection
            * Results in nested structs!
            *
            * @param refl Reflection implementation of the nested struct
            */
            void addOtherReflection(const char* name, int offset, ReflObject* refl);

            /**
            * Returns the size in bytes of the object
            * !Should be used as assertion to check whether the reflection equals the true object!
            */
            std::size_t sizeOf();


        public:
            const char* Name;
            std::vector<MemberMeta> Meta;
            std::vector<GroupMeta> Groups;

        };


        // IMPLEMENTATION --------------------------------------------------
        template <class T>
        void ReflObject::init() {
            Name = typeid(T).name();
        }

        template <class T>
        void ReflObject::addMember(const char* name, int offset, int as) {
            MemberMeta meta;
            meta.name = name;
            meta.mem = as;
            meta.type_name = typeid(T).name();
            meta.second_type_name = NULL;
            meta.size = sizeof(T);
            meta.offset = offset;
            Meta.push_back(meta);
        }

        template <class T, class I>
        void ReflObject::addVectorMember(const char* name, int offset) {
            MemberMeta meta;
            meta.name = name;
            meta.mem = LIVELOG_REFL_ARRAY;
            meta.type_name = typeid(T).name();
            meta.second_type_name = typeid(I).name();
            meta.size = sizeof(T);
            meta.offset = offset;
            Meta.push_back(meta);
        }
    }
}
