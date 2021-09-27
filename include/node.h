#pragma once

#include <ext/glm/glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Node
{

public:
  std::vector< std::shared_ptr<Node> > mChildren;

  Node() {}

  void SetName(const char* name);
  void SetParent(std::shared_ptr<Node> parent);
  void SetLocalTransform(glm::mat4 localTransform);

  std::string GetName();
  std::shared_ptr<Node> GetParent();
  glm::mat4 GetLocalTransform();
  glm::mat4 GetGlobalTransform();

  void AddChild(std::shared_ptr<Node> child);
private:
  std::string mName;

  std::shared_ptr<Node> mParent;

  glm::mat4 mLocalTransform;

  glm::mat4 mGlobalTransform;
  bool mDirtyGlobalTransform = true;
};
