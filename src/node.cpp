#include <node.h>

Node::Node(std::shared_ptr<Node> parent, const char* name, glm::mat4 localTransform)
{
  mParent = parent;
  mName = std::string(name);
  mLocalTransform = localTransform;
  mDirtyTransform = true;
}

glm::mat4 Node::GetLocalTransform()
{
  return mLocalTransform;
}

glm::mat4 Node::GetGlobalTransform()
{
  if (mDirtyTransform)
  {
    mGlobalTransform = mParent == nullptr ? mLocalTransform : mParent->GetGlobalTransform() * mLocalTransform;
    mDirtyTransform = false;
  }

  return mGlobalTransform;
}

void Node::AddChild(std::shared_ptr<Node> child)
{
  mChildren.push_back(child);
}
