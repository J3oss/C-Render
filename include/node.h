#pragma once

#include <ext/glm/glm/glm.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <memory>

class Node
{
public:
  Node() {}

  std::vector< std::shared_ptr<Node> > mChildren;

  std::string GetName();
  void SetName(std::string name);

  void AddChild(std::shared_ptr<Node> child);

  std::shared_ptr<Node> GetParent();
  void SetParent(std::weak_ptr<Node> parent);

  glm::mat4 GetLocalTransform();
  void SetLocalTransform(glm::mat4 localTransform);

  glm::mat4 GetGlobalTransform();

  void Scale(glm::vec3 delta);
  void Rotate(glm::vec3 delta);
  void Translate(glm::vec3 delta);

private:
  std::string mName;

  std::weak_ptr<Node> mParent;

  glm::mat4 mLocalTransform;

  glm::mat4 mGlobalTransform;
  bool mDirtyGlobalTransform = true;

  void SetDirty();
};
