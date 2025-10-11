#ifndef SIGNEDIT_CLIENTINSTANCE_H
#define SIGNEDIT_CLIENTINSTANCE_H

#include <stl.h>
#include <stl/memory>
#include <stl/string>

#ifndef SIGNEDIT_BLOCKSOURCE_H
	struct BlockPos;
#endif

class ClientInstance {};
class AbstractScene {};

class UIScene : public AbstractScene {
public:
	void setTextboxText(const stl::string&);
};

class SceneFactory {
public:
	stl::shared_ptr<UIScene> createSignScreen(const BlockPos&);
};

class SceneStack {
public:
	void pushScreen(stl::shared_ptr<AbstractScene>, bool);
};

#endif
