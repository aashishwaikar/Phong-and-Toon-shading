#include <RenderObject.hpp>

#include <iostream>
#include <exception>

namespace Engine
{
    RenderObject::RenderObject()
    {
        _transform.SetPosition(glm::vec3(0.0f));
        _transform.SetOrientation(glm::mat4(1.0f));
        _transform.SetScale(glm::vec3(1.0f));
    }

    RenderObject::RenderObject(Mesh* mesh, Material* material) : _mesh(mesh), _material(material)
    {
        _transform.SetPosition(glm::vec3(0.0f));
        _transform.SetOrientation(glm::mat4(1.0f));
        _transform.SetScale(glm::vec3(1.0f));
    }

    void RenderObject::Render(Camera* cam)
    {
        GLuint programId = _material->GetProgram()->GetProgramId();
        glUseProgram(programId);
		
        // Send camera uniform data to glsl program
        glm::mat4 worldTransform = _transform.GetWorldTransform();

        glUniformMatrix4fv(glGetUniformLocation(programId, "worldTransform"), 1, GL_FALSE, (float*)&worldTransform);

        glm::mat4 cameraTransform = cam->GetTransform()->GetWorldTransform();
        glUniformMatrix4fv(glGetUniformLocation(programId, "cameraTransform"), 1, GL_FALSE, (float*)&cameraTransform);

        glm::mat4 projectionMatrix = cam->GetProjectionMatrix();
        glUniformMatrix4fv(glGetUniformLocation(programId, "projectionMatrix"), 1, GL_FALSE, (float*)&projectionMatrix);
		
        GLenum drawMode = _mesh->GetDrawMode();
        int numElements = _mesh->GetNumElements();

        if (drawMode == GL_FALSE)
        {
            std::cout << "Cannot render this object with undefined behaviour\n";
            throw std::exception();
        }

        if (numElements == 0)
        {
            std::cout << "Cannot render this object with zero elements\n";
            throw std::exception();
        }

        glBindVertexArray(_mesh->GetVao());

        if (_mesh->GetHasIndex())
        {
            glBindBuffer(GL_ARRAY_BUFFER, _mesh->GetVbo());
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh->GetIbo());
            glDrawElements(drawMode, numElements, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glBindBuffer(GL_ARRAY_BUFFER, _mesh->GetVbo());
            glDrawArrays(drawMode, 0, numElements);
        }

        glBindVertexArray(0);
    }
}