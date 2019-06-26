//
// Created by kurono267 on 2019-06-26.
//

#ifndef MANGO_SCENENODE_HPP
#define MANGO_SCENENODE_HPP

#include "SceneTransform.hpp"
#include <vector>

namespace mango::scene {

class SceneNode : public SceneTransform, public std::enable_shared_from_this<SceneNode> {
	typedef std::shared_ptr<SceneNode> ptr;
	public:
		SceneNode();
		~SceneNode();

		void addChild(const ptr& child);
		std::vector<ptr>& getChilds();

		ptr getParent();
	protected:
		std::vector<ptr> _childs;
		mutable ptr _parent;
};

}


#endif //MANGO_SCENENODE_HPP
