#pragma once

#include <ext/glm/glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Node
{

public:
  std::string mName;

  std::shared_ptr<Node> mParent;
  std::vector< std::shared_ptr<Node> > mChildren;

  Node(std::shared_ptr<Node> parent, const char* name, glm::mat4 localTransform);

  glm::mat4 GetLocalTransform();
  glm::mat4 GetGlobalTransform();

  void AddChild(std::shared_ptr<Node> child);

private:
  glm::mat4 mLocalTransform;
  glm::mat4 mGlobalTransform;

  bool mDirtyTransform;
};
