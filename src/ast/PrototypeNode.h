#pragma once

#include <string>
#include <vector>

#include "ASTNode.h"


class PrototypeNode : public ASTNode {
public:
  std::string name;
  std::vector<std::string> args;

  PrototypeNode(const std::string &name, const std::vector<std::string> &args)
    : name(name), args(args) {};

  template<class RenderSpec> typename RenderSpec::Result* render(Renderer<RenderSpec> *renderer) {
    return renderer->render(this);
  };
};
