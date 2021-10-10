#include <node.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

std::string Node::GetName()
{
  return mName;
}
void Node::SetName(std::string name)
{
  mName = name;
}

void Node::AddChild(std::shared_ptr<Node> child)
{
  mChildren.push_back(child);
}

// std::vector<Node>& Node::GetChildren()
// {
//   return mChildren;
// }

std::shared_ptr<Node> Node::GetParent()
{
  std::shared_ptr<Node> p = mParent.lock();
  return p;
}
void Node::SetParent(std::weak_ptr<Node> parent)
{
  mParent = parent;
}

glm::mat4 Node::GetLocalTransform()
{
  return mLocalTransform;
}
void Node::SetLocalTransform(glm::mat4 localTransform)
{
  mLocalTransform = localTransform;
  mDirtyGlobalTransform = true;
}

glm::mat4 Node::GetGlobalTransform()
{
  std::shared_ptr<Node> p = mParent.lock();

  if (mDirtyGlobalTransform)
  {
    mGlobalTransform = p == nullptr ? mLocalTransform : p->GetGlobalTransform() * mLocalTransform;
    mDirtyGlobalTransform = false;
  }

  return mGlobalTransform;
}

void Node::Scale(glm::vec3 delta)
{
  mScale.x *= delta.x;
  mScale.y *= delta.y;
  mScale.z *= delta.z;
}
void Node::Rotate(glm::vec3 delta)
{
  glm::quat q = glm::quat(delta);

  glm::mat4 temp = glm::toMat4(q) * mRotation;
  mRotation = temp;
}
void Node::Translate(glm::vec3 delta)
{
  mTranslation.x += delta.x;
  mTranslation.y += delta.y;
  mTranslation.z += delta.z;
}
