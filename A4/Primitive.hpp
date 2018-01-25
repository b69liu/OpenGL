#pragma once

#include <glm/glm.hpp>

class Primitive {
public:
  virtual ~Primitive();
};

class Sphere : public Primitive {
public:
  virtual ~Sphere();
  int gettype(){return p_type;}
private:
  int p_type;                //Sphere 1, Cube 2, Nonhs 3, NonBox 4

};

class Cube : public Primitive {
public:
  virtual ~Cube();
  int gettype(){return p_type;}
private:
  int p_type;                //Sphere 1, Cube 2, Nonhs 3, NonBox 4

};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const glm::vec3& pos, double radius)
    : m_pos(pos), m_radius(radius),p_type(3)
  {
  }
  virtual ~NonhierSphere();
  glm::vec3 getpos(){
      return m_pos;
  }
  double getrad(){
      return m_radius;
  }
  int gettype(){return p_type;}

private:
  glm::vec3 m_pos;
  double m_radius;
  int p_type;                //Sphere 1, Cube 2, Nonhs 3, NonBox 4

};

class NonhierBox : public Primitive {
public:

  NonhierBox(const glm::vec3& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  int gettype(){return p_type;}

  
  virtual ~NonhierBox();

private:
  glm::vec3 m_pos;
  double m_size;
  int p_type;                //Sphere 1, Cube 2, Nonhs 3, NonBox 4

};
