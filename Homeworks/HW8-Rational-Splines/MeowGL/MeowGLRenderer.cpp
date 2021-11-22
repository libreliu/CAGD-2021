#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "MeowGLRenderer.h"
#include <iostream>

using namespace MeowGL;

static const std::string vShaderCode =
#include "MeowGLShader.vs"
"";

static const std::string fShaderCode =
#include "MeowGLShader.fs"
"";

void Renderer::Init(int target_w, int target_h) {
  finalRT.target_h = target_h;
  finalRT.target_w = target_w;

  if (!gl3wInit()) {
    throw std::exception("failed to initialize OpenGL");
  }

  if (!gl3wIsSupported(3, 3)) {
    throw std::exception("OpenGL 3.3 not supported\n");
  }

  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
    glGetString(GL_SHADING_LANGUAGE_VERSION));

  // Set up framebuffer object & bind as active
  glGenFramebuffers(1, &finalRT.fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, finalRT.fbo);

  glGenTextures(1, &finalRT.colorTex);
  glBindTexture(GL_TEXTURE_2D, finalRT.colorTex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, target_w, target_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Bind color texture to active framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, finalRT.colorTex, 0);  

  // Create depth & stencil render buffer
  glGenRenderbuffers(1, &finalRT.rbo);
  glBindRenderbuffer(GL_RENDERBUFFER, finalRT.rbo); 
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, target_w, target_h);  
  glBindRenderbuffer(GL_RENDERBUFFER, 0);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, finalRT.rbo);

  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    throw std::exception("Framebuffer is not complete!");
  }

  // Set up shader program to use
  InitShader();

}

void Renderer::InitShader() {
  GLuint vertex, fragment;
  const char *shaderSources[] = {
    vShaderCode.c_str(),
    fShaderCode.c_str()
  };

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &shaderSources[0], NULL);
  glCompileShader(vertex);
  CheckShaderCompileErrors(vertex, "VERTEX");

  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &shaderSources[1], NULL);
  glCompileShader(fragment);
  CheckShaderCompileErrors(fragment, "FRAGMENT");

  // shader Program
  shaders.progID = glCreateProgram();
  glAttachShader(shaders.progID, vertex);
  glAttachShader(shaders.progID, fragment);
  glLinkProgram(shaders.progID);
  CheckShaderCompileErrors(shaders.progID, "PROGRAM");

  // delete the shaders as they're linked into our program now and no longer necessery
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Renderer::CheckShaderCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM")
  {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
          glGetShaderInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
  }
  else
  {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success)
      {
          glGetProgramInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
      }
  }
}

void Renderer::AddTriMesh(const std::vector<Vec3f> &positions, const std::vector<Vec3i> indices) {
  MeshDesc mesh;

  // create buffers/arrays
  glGenVertexArrays(1, &mesh.VAO);
  glGenBuffers(1, &mesh.VBO);
  glGenBuffers(1, &mesh.EBO);
  glBindVertexArray(mesh.VAO);

  // load data into vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec3f), &positions[0], GL_STATIC_DRAW);  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  // Bind the default vertex attribute array object to current vertex arrays object

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vec3f), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  meshes.push_back(mesh);
}

void Renderer::SetCameraProperties(float fovy, float zNear, float zFar) {
  camera.farZ = zFar;
  camera.nearZ = zNear;
  camera.fovy = fovy;
}

// this is actually column major order here
// GLSL is column major too so no problem
mat4f Renderer::GetProjectionMatrix(float fovy, float aspect, float zNear, float zFar) {
  assert(abs(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);
  float const tanHalfFovy = tan(fovy / 2.0f);

  mat4f Result;
  Result(0,0) = 1.0f / (aspect * tanHalfFovy);
  Result(1,1) = 1.0f / (tanHalfFovy);
  Result(2,2) = - (zFar + zNear) / (zFar - zNear);
  Result(2,3) = - 1.0f;
  Result(3,2) = - (2.0f * zFar * zNear) / (zFar - zNear);
  return Result;
}

void Renderer::Draw() {
  // Bind to Framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, finalRT.fbo);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(shaders.progID);

  mat4f projMtx = GetProjectionMatrix(
    camera.fovy,
    finalRT.target_w / finalRT.target_h,
    camera.nearZ,
    camera.farZ
  );
  glUniformMatrix4fv(glGetUniformLocation(shaders.progID, "ProjMtx"), 1, GL_FALSE, &projMtx(0,0));

  for (auto &mesh: meshes) {
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.elementCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  // Unbind from framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::Cleanup() {
  // Clean all VAO, buffers, and so on
  
}