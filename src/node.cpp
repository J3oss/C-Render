#include <node.h>

void Node::SetName(const char* name)
{
  mName = std::string(name);
}

void Node::SetParent(std::shared_ptr<Node> parent)
{
  mParent = parent;
}

void Node::SetLocalTransform(glm::mat4 localTransform)
{
  mLocalTransform = localTransform;
  mDirtyGlobalTransform = true;
}

std::string Node::GetName()
{
  return mName;
}

std::shared_ptr<Node> Node::GetParent()
{
  return mParent;
}

glm::mat4 Node::GetLocalTransform()
{
  return mLocalTransform;
}

glm::mat4 Node::GetGlobalTransform()
{
  if (mDirtyGlobalTransform)
  {
    mGlobalTransform = mParent == nullptr ? mLocalTransform : mParent->GetGlobalTransform() * mLocalTransform;
    mDirtyGlobalTransform = false;
  }

  return mGlobalTransform;
}

void Node::AddChild(std::shared_ptr<Node> child)
{
  mChildren.push_back(child);
}
