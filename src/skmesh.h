#pragma once
/*

    Copyright 2011 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OGLDEV_SKINNED_MESH_H
#define	OGLDEV_SKINNED_MESH_H

#include <map>
#include <vector>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "SmartTexture.h"


using namespace std;
using namespace glm;
typedef unsigned int uint;
static inline glm::vec3 vec3_cast(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 vec2_cast(const aiVector3D& v) { return glm::vec2(v.x, v.y); }
static inline glm::quat quat_cast(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }
static inline glm::mat4 mat4_cast(const aiMatrix3x3& m) { return glm::transpose(glm::make_mat3(&m.a1)); }

class SkinnedMesh
    {
    public:
        int currentAnimation = 0;
        SkinnedMesh();

        ~SkinnedMesh();

        bool LoadMesh(const string& Filename);

        void Render(GLuint shaderProgram);

        uint NumBones() const
            {
            return m_NumBones;
            }

        void BoneTransform(float TimeInSeconds, vector<mat4>& Transforms);
        void setBoneTransformations(GLuint shaderProgram, GLfloat currentTime);
    private:
#define NUM_BONES_PER_VEREX 4

        struct BoneInfo
            {
            mat4 BoneOffset;
            mat4 FinalTransformation;

            BoneInfo()
                {
                BoneOffset = mat4(0);
                FinalTransformation = mat4(0);           
                }
            };

        struct VertexBoneData
            {
            uint IDs[NUM_BONES_PER_VEREX];
            float Weights[NUM_BONES_PER_VEREX];

            VertexBoneData()
                {
                Reset();
                };

            void Reset()
                {
                for (int i = 0; i < NUM_BONES_PER_VEREX; i++)
                    {
                    IDs[i] = 0;
                    Weights[i] = 0;
                    }
                }

            void AddBoneData(uint BoneID, float Weight);
            };

        void CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        void CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
        uint FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
        const aiNodeAnim* FindNodeAnim(const aiAnimation* pAnimation, const string NodeName);
        void ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const mat4& ParentTransform);
        bool InitFromScene(const aiScene* pScene, const string& Filename);
        void InitMesh(uint MeshIndex,
                      const aiMesh* paiMesh,
                      vector<vec3>& Positions,
                      vector<vec3>& Normals,
                      vector<vec2>& TexCoords,
                      vector<VertexBoneData>& Bones,
                      vector<unsigned int>& Indices);
        void LoadBones(uint MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
        bool InitMaterials(const aiScene* pScene, const string& Filename);
        void Clear();

#define INVALID_MATERIAL 0xFFFFFFFF

        enum VB_TYPES {
            INDEX_BUFFER,
            POS_VB,
            NORMAL_VB,
            TEXCOORD_VB,
            BONE_VB,
            NUM_VBs
            };

        GLuint m_VAO;
        GLuint m_Buffers[NUM_VBs];

        struct MeshEntry {
            MeshEntry()
                {
                NumIndices = 0;
                BaseVertex = 0;
                BaseIndex = 0;
                MaterialIndex = INVALID_MATERIAL;
                }

            unsigned int NumIndices;
            unsigned int BaseVertex;
            unsigned int BaseIndex;
            unsigned int MaterialIndex;
            };

        vector<MeshEntry> m_Entries;
       std::vector<std::shared_ptr<SmartTexture>> m_Textures;

        map<string, uint> m_BoneMapping; // maps a bone name to its index
        uint m_NumBones;
        vector<BoneInfo> m_BoneInfo;
        mat4 m_GlobalInverseTransform;

        const aiScene* m_pScene;
        Assimp::Importer m_Importer;
    };

#endif	/* OGLDEV_SKINNED_MESH_H */

