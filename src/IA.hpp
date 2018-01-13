#ifndef IA_HPP
#  define IA_HPP

#  include <string>

class IIA
{
public:

  IIA() {}
  virtual ~IIA() {}
  virtual std::string nextMove() = 0;
};

#endif
