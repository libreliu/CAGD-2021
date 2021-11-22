#include "MeowGLMath.h"
#include <GL/gl3w.h>
#include <glfw/glfw3.h>

namespace MeowGL {

// For OpenGL 3+ usage
class Renderer {
private:
  struct {
    GLuint fbo;
    GLuint colorTex;
    GLuint rbo;
    int target_w;
    int target_h;
  } finalRT;

  struct {
    GLuint progID;
  } shaders;

  struct {
    float nearZ;
    float farZ;
    float fovy;
  } camera;

  struct MeshDesc {
    GLuint VAO; // Vertex Arrays Object
    GLuint VBO; // Vertex buffer Object
    GLuint EBO; // Element buffer Object

    GLsizei elementCount;
  };
  std::vector<MeshDesc> meshes;

  void InitShader();
  void CheckShaderCompileErrors(GLuint shader, std::string type);
  mat4f GetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);

public:
  void Init(int target_w, int target_h);
  void AddTriMesh(const std::vector<Vec3f> &positions, const std::vector<Vec3i> indices);
  void SetCameraProperties(float fovy, float zNear, float zFar);
  void Draw();
  inline GLuint GetRenderTextureID() {
    return finalRT.colorTex;
  }
  void Cleanup();
};

}