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



#include "skmesh.h"

#include <iostream>

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

void SkinnedMesh::VertexBoneData::AddBoneData(uint BoneID, float Weight)
    {
    for (uint i = 0; i < NUM_BONES_PER_VEREX; i++) {
        if (Weights[i] == 0.0) {
            IDs[i] = BoneID;
            Weights[i] = Weight;
            return;
            }
        }

    // should never get here - more bones than we have space for
    assert(0);
    }

SkinnedMesh::SkinnedMesh()
    {
    m_VAO = 0;
    for (int i = 0; i < NUM_VBs; i++)
        m_Buffers[i] = 0;
    m_NumBones = 0;
    m_pScene = NULL;
    }


SkinnedMesh::~SkinnedMesh()
    {
    Clear();
    }


void SkinnedMesh::Clear()
    {
    m_Textures.clear();
        

    if (m_Buffers[0] != 0) {
        glDeleteBuffers(NUM_VBs, m_Buffers);
        }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
        }
    }



bool SkinnedMesh::LoadMesh(const string& Filename)
    {
    // Release the previously loaded mesh (if it exists)
    //Clear();

    // Create the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    // Create the buffers for the vertices attributes
    glGenBuffers(NUM_VBs, m_Buffers);

    bool Ret = false;

    m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals );
  /*  m_Importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    m_pScene = m_Importer.ReadFile(Filename.c_str(), aiProcess_JoinIdenticalVertices |		// join identical vertices/ optimize indexing
                            aiProcess_ValidateDataStructure |		// perform a full validation of the loader's output
                            aiProcess_ImproveCacheLocality |		// improve the cache locality of the output vertices
                            aiProcess_RemoveRedundantMaterials |	// remove redundant materials
                            aiProcess_GenUVCoords |					// convert spherical, cylindrical, box and planar mapping to proper UVs
                            aiProcess_TransformUVCoords |			// pre-process UV transformations (scaling, translation ...)
                            //aiProcess_FindInstances |				// search for instanced meshes and remove them by references to one master
                            aiProcess_LimitBoneWeights |			// limit bone weights to 4 per vertex
                            aiProcess_OptimizeMeshes |				// join small meshes, if possible;
                            //aiProcess_PreTransformVertices |
                            aiProcess_GenSmoothNormals |			// generate smooth normal vectors if not existing
                            aiProcess_SplitLargeMeshes |			// split large, unrenderable meshes into sub-meshes
                            aiProcess_Triangulate |					// triangulate polygons with more than 3 edges
                            aiProcess_ConvertToLeftHanded |			// convert everything to D3D left handed space
                            aiProcess_SortByPType);*/

    if (m_pScene) {
        m_GlobalInverseTransform = mat4_cast(m_pScene->mRootNode->mTransformation);
        m_GlobalInverseTransform = inverse(m_GlobalInverseTransform);
        Ret = InitFromScene(m_pScene, Filename);
        }
    else {
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), m_Importer.GetErrorString());
        }

    // Make sure the VAO is not changed from the outside
    glBindVertexArray(0);

    return Ret;
    }


bool SkinnedMesh::InitFromScene(const aiScene* pScene, const string& Filename)
    {
    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    vector<vec3> Positions;
    vector<vec3> Normals;
    vector<vec2> TexCoords;
    vector<VertexBoneData> Bones;
    vector<uint> Indices;

    uint NumVertices = 0;
    uint NumIndices = 0;

    // Count the number of vertices and indices
    for (uint i = 0; i < m_Entries.size(); i++) {
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;

        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices += m_Entries[i].NumIndices;
        }

    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
    Bones.resize(NumVertices);
    Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
    for (uint i = 0; i < m_Entries.size(); i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        InitMesh(i, paiMesh, Positions, Normals, TexCoords, Bones, Indices);
        }

    if (!InitMaterials(pScene, Filename)) {
        return false;
        }

    // Generate and populate the buffers with vertex attributes and the indices
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
    return true;
    //return GLCheckError();
    }


void SkinnedMesh::InitMesh(uint MeshIndex,
                           const aiMesh* paiMesh,
                           vector<vec3>& Positions,
                           vector<vec3>& Normals,
                           vector<vec2>& TexCoords,
                           vector<VertexBoneData>& Bones,
                           vector<uint>& Indices)
    {
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

    // Populate the vertex attribute vectors
    for (uint i = 0; i < paiMesh->mNumVertices; i++) {
        const aiVector3D* pPos = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

        Positions.push_back(vec3(pPos->x, pPos->y, pPos->z));
        Normals.push_back(vec3(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(vec2(pTexCoord->x, pTexCoord->y));
        }

    LoadBones(MeshIndex, paiMesh, Bones);

    // Populate the index buffer
    for (uint i = 0; i < paiMesh->mNumFaces; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
        }
    }


void SkinnedMesh::LoadBones(uint MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones)
    {
    for (uint i = 0; i < pMesh->mNumBones; i++) {
        uint BoneIndex = 0;
        string BoneName(pMesh->mBones[i]->mName.data);

        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = mat4_cast(pMesh->mBones[i]->mOffsetMatrix);
            m_BoneMapping[BoneName] = BoneIndex;
            }
        else {
            BoneIndex = m_BoneMapping[BoneName];
            }

        for (uint j = 0; j < pMesh->mBones[i]->mNumWeights; j++) {
            uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight = pMesh->mBones[i]->mWeights[j].mWeight;
            Bones[VertexID].AddBoneData(BoneIndex, Weight);
            }
        }
    }


bool SkinnedMesh::InitMaterials(const aiScene* pScene, const string& Filename)
    {
    // Extract the directory part from the file name
    string::size_type SlashIndex = Filename.find_last_of("/");
    string Dir;

    if (SlashIndex == string::npos) {
        Dir = ".";
        }
    else if (SlashIndex == 0) {
        Dir = "/";
        }
    else {
        Dir = Filename.substr(0, SlashIndex);
        }

    bool Ret = true;

    // Initialize the materials
    for (uint i = 0; i < pScene->mNumMaterials; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
                {
                string p(Path.data);
                if (p.substr(0, 2) == ".\\")
                    {
                    p = p.substr(2, p.size() - 2);
                    }
                string FullPath = Dir + "/" + p;
                m_Textures[i] = SmartTexture::createTexture(FullPath);
                }
            }
        }

    return Ret;
    }


void SkinnedMesh::Render(GLuint shaderProgram)
{
    glBindVertexArray(m_VAO);

    for (uint i = 0; i < m_Entries.size(); i++) {
        const uint MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());

        if (m_Textures[MaterialIndex]) {
            //GLuint location = glGetUniformLocation(shaderProgram, "tex");
            //m_Textures[MaterialIndex]->bind(location,0);
            m_Textures[MaterialIndex]->bind(shaderProgram,0);
		}

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(uint) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);
	}

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
}


uint SkinnedMesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
    {
    for (uint i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
            }
        }

    assert(0);

    return 0;
    }


uint SkinnedMesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
    {
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
            }
        }

    assert(0);

    return 0;
    }


uint SkinnedMesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
    {
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        if (AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
            }
        }

    assert(0);

    return 0;
    }


void SkinnedMesh::CalcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
    {
    if (pNodeAnim->mNumPositionKeys == 1) {
        Out = pNodeAnim->mPositionKeys[0].mValue;
        return;
        }

    uint PositionIndex = FindPosition(AnimationTime, pNodeAnim);
    uint NextPositionIndex = (PositionIndex + 1);
    assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);
    float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
    const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
    }


void SkinnedMesh::CalcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
    {
    // we need at least two values to interpolate...
    if (pNodeAnim->mNumRotationKeys == 1) {
        Out = pNodeAnim->mRotationKeys[0].mValue;
        return;
        }

    uint RotationIndex = FindRotation(AnimationTime, pNodeAnim);
    uint NextRotationIndex = (RotationIndex + 1);
    assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);
    float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
    const aiQuaternion& EndRotationQ = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
    aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
    Out = Out.Normalize();
    }


void SkinnedMesh::CalcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
    {
    if (pNodeAnim->mNumScalingKeys == 1) {
        Out = pNodeAnim->mScalingKeys[0].mValue;
        return;
        }

    uint ScalingIndex = FindScaling(AnimationTime, pNodeAnim);
    uint NextScalingIndex = (ScalingIndex + 1);
    assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);
    float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
    float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
    const aiVector3D& End = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
    aiVector3D Delta = End - Start;
    Out = Start + Factor * Delta;
    }


void SkinnedMesh::ReadNodeHeirarchy(float AnimationTime, const aiNode* pNode, const glm::mat4& ParentTransform)
    {
    std::string NodeName(pNode->mName.data);
    const aiAnimation* pAnimation = m_pScene->mAnimations[currentAnimation];
    glm::mat4 NodeTransformation = mat4_cast(pNode->mTransformation);

    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);
    if (pNodeAnim) {
        // Interpolate scaling and generate scaling transformation matrix
        aiVector3D Scaling;
        CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
        glm::vec3 scale = glm::vec3(Scaling.x, Scaling.y, Scaling.z);
        glm::mat4 ScalingM = glm::scale(glm::mat4(1.0f), scale);

        // Interpolate rotation and generate rotation transformation matrix
        aiQuaternion RotationQ;
        CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
        glm::quat rotation = quat_cast(RotationQ);
        glm::mat4 RotationM = mat4(rotation);
        // Interpolate translation and generate translation transformation matrix
        aiVector3D Translation;
        CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
        glm::vec3 translation = glm::vec3(Translation.x, Translation.y, Translation.z);
        glm::mat4 TranslationM = glm::translate(glm::mat4(1.0f), translation);
        // Combine the above transformations
        NodeTransformation = TranslationM * RotationM * ScalingM;
        }

    // Combine with node Transformation with Parent Transformation
    glm::mat4 GlobalTransformation = ParentTransform * NodeTransformation;
    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) {
        unsigned int BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
        }
    for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
        ReadNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation);
        }
    }


void SkinnedMesh::BoneTransform(float timeInSeconds, std::vector<glm::mat4>& Transforms)
{
    glm::mat4 Identity = glm::mat4(1.0f);
    
    /* Calc animation duration */
    auto numPosKeys = m_pScene->mAnimations[currentAnimation]->mChannels[0]->mNumPositionKeys;
    auto animDuration = m_pScene->mAnimations[currentAnimation]->mChannels[0]->mPositionKeys[numPosKeys - 1].mTime;
    auto TicksPerSecond = (float)(m_pScene->mAnimations[currentAnimation]->mTicksPerSecond != 0 ? m_pScene->mAnimations[currentAnimation]->mTicksPerSecond : 25.0f);
    auto TimeInTicks = timeInSeconds * TicksPerSecond;
    auto AnimationTime = fmod(TimeInTicks, (float)animDuration);
    ReadNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity);
    Transforms.resize(m_NumBones);
    
    for (unsigned i = 0; i < m_NumBones; i++) {
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}


const aiNodeAnim* SkinnedMesh::FindNodeAnim(const aiAnimation* pAnimation, const string NodeName)
    {
    for (uint i = 0; i < pAnimation->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
            }
        }

    return NULL;
    }


void SkinnedMesh::setBoneTransformations(GLuint shaderProgram, GLfloat currentTime)
        {
        std::vector<glm::mat4> Transforms;
        BoneTransform((float)currentTime, Transforms);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "gBones"), (GLsizei)Transforms.size(), GL_FALSE, glm::value_ptr(Transforms[0]));
        }