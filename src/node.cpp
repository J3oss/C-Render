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
  SetDirty();
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
  // mScale.x *= delta.x;
  // mScale.y *= delta.y;
  // mScale.z *= delta.z;
}
void Node::Rotate(glm::vec3 delta)
{
  glm::quat q = glm::quat(delta);
  mLocalTransform = mLocalTransform * glm::toMat4(q);
  SetDirty();
}
void Node::Translate(glm::vec3 delta)
{
  glm::mat4 t(1.0);
  t[3] = glm::vec4(delta, 1.0);
  mLocalTransform = mLocalTransform * t;
  SetDirty();
}

void Node::SetDirty()
{
  mDirtyGlobalTransform = true;

  for (size_t childIndedx = 0; childIndedx < mChildren.size(); childIndedx++)
    mChildren[childIndedx]->SetDirty();
}

glm::vec3 Node::GetPosition() {
  return glm::vec3(GetGlobalTransform()[3]);
}
