//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_GEOMETRY_HPP
#define MANGO_GEOMETRY_HPP

#include "../unified/Mesh.hpp"
#include "Material.hpp"
#include "Skin.hpp"

namespace mango {

/// Geometry consist of mesh and material
class Renderable {
	public:
		/// Default constructor
		Renderable() = default;
		/// Construct Geometry with Mesh and Material
		/// @param mesh - Mesh
		/// @param material - Material
		Renderable(const spMesh& mesh, const spMaterial& material);

		/// Set render type
		/// @param render type
		void setRenderType(uint32_t renderType);

		/// Get render type
		/// @return render type
		uint32_t getRenderType();

		/// Set mesh
		/// @param mesh - Mesh
		void setMesh(const spMesh& mesh);
		/// Set material
		/// @param material - Material
		void setMaterial(const spMaterial& material);
		/// Set skin
		/// @param skin - Skin
		void setSkin(const spSkin& skin);

		/// Get mesh
		spMesh getMesh();
		/// Get material
		spMaterial getMaterial();
		/// Get skin
		spSkin getSkin();

		/// Construct shared pointer of Geometry
		/// @param mesh - Mesh
		/// @param material - Material
		static std::shared_ptr<Renderable> create(const spMesh& mesh = nullptr, const spMaterial& material = nullptr);
	protected:
		spMesh _mesh;
		spMaterial _material;
		spSkin _skin;
		uint32_t _renderType = 0;
};

typedef std::shared_ptr<Renderable> spRenderable;

}

#endif //MANGO_GEOMETRY_HPP
