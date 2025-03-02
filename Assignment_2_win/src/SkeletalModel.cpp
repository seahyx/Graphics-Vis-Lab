///=========================================================================================///
///
///                       Functions to be filled in for Assignment 2   
///
///           IMPORTANT: you ONLY need to work on functions with TODO in this section    
///    
///=========================================================================================///


#include "SkeletalModel.h"


///=========================================================================================///
///                                    Load .skel File    
///=========================================================================================///


// Load the skeleton from file here, create hierarchy of joints
//       (i.e., set values for m_rootJoint and m_joints)
void SkeletalModel::loadSkeleton( const char* filename )
{
    ifstream skelFile(filename);
    float tx, ty, tz;
    int parentId;

    while (skelFile >> tx >> ty >> tz >> parentId) {
        cout << tx << " " << ty << " " << tz << " " << parentId << endl;

        Joint* joint = new Joint;
        m_joints.push_back(joint);

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(tx, ty, tz));

        joint->transform = transform;
        if (parentId == -1)
        {
            m_rootJoint = joint;
            continue;
        }
        m_joints[parentId]->children.push_back(joint);
    }
}




///=========================================================================================///
///                         Compute transformations for Joints and Bones     
///=========================================================================================///

void SkeletalModel::computeTransforms()
{
    if( m_joints.size() == 0 )
        return;

    computeJointTransforms();

    computeBoneTransforms();
}

// Compute a transformation matrix for each joint (i.e., ball) of the skeleton
void SkeletalModel::computeJointTransforms( )
{
    jointMatList.clear();

    m_matrixStack.clear();

    computeJointTransforms(m_rootJoint, m_matrixStack);
}

// Recursive helper function to traverse the joint hierarchy for computing transformations of the joints
void SkeletalModel::computeJointTransforms(Joint* joint, MatrixStack matrixStack)
{
    matrixStack.push(joint->transform);

    // Transpose cuz the main function transposes the transformation matrices for some unknown reason
    jointMatList.push_back(glm::transpose(matrixStack.top()));
    
    for (unsigned int i = 0; i < joint->children.size(); i++)
        computeJointTransforms(joint->children[i], matrixStack);

    matrixStack.pop();
}


// Compute a transformation matrix for each bone (i.e., cylinder) between each pair of joints in the skeleton
void SkeletalModel::computeBoneTransforms( )
{
    boneMatList.clear();

    m_matrixStack.clear();

    computeBoneTransforms(m_rootJoint, m_matrixStack);
}

// Recursive helper function to traverse the joint hierarchy for computing transformations of the bones
void SkeletalModel::computeBoneTransforms(Joint* joint, MatrixStack matrixStack)
{
    matrixStack.push(joint->transform);

    for (unsigned int i = 0; i < joint->children.size(); i++)
    {
        Joint* childJoint = joint->children[i];

        // Get distance between current joint and child joint
        float dist = glm::length(glm::vec3(childJoint->transform[3]));

        // Translate by z by 0.5 units (height/2) so the cylider so the pivot is at origin
        glm::mat4 translatePivot = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.5f));

        // Scale the cylinder so it's not so phat, and its height by the dist
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, dist));

        //// Rotate the cylinder to face the child joint
        glm::vec3 from = glm::vec3(0, 0, 1.0f);
        glm::vec3 to = glm::normalize(glm::vec3(childJoint->transform[3]));
        glm::vec3 axis = glm::normalize(glm::cross(from, to));
        float angle = acos(glm::dot(from, to));
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), angle, axis);

        // First translate pivot, then scale, then rotate, then translate to joint position
        glm::mat4 transform = matrixStack.top() * rotate * scale * translatePivot;

        // Transpose cuz the main function transposes the transformation matrices for some unknown reason
        boneMatList.push_back(glm::transpose(transform));
        computeBoneTransforms(childJoint, matrixStack);
    }

    matrixStack.pop();
}




///=========================================================================================///
///                              Set Joint Angles for Transform     
///=========================================================================================///

// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int jointIndex, float angleX, float angleY, float angleZ)
{
    // Convert from degrees to radians
    angleX *= M_PI / 180.0f;
    angleY *= M_PI / 180.0f;
    angleZ *= M_PI / 180.0f;
    glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), angleX, glm::vec3(1.0f, 0, 0));
    glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), angleY, glm::vec3(0, 1.0f, 0));
    glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), angleZ, glm::vec3(1, 0, 1.0f));
    // Apply rotation
    m_joints[jointIndex]->transform = m_joints[jointIndex]->transform * rotZ * rotY * rotX;
    computeTransforms();
}







