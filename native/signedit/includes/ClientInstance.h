#ifndef SIGNEDIT_CLIENTINSTANCE_H
#define SIGNEDIT_CLIENTINSTANCE_H

#include <stl.h>

#ifndef SIGNEDIT_BLOCKSOURCE_H
	struct BlockPos;
#endif

class ClientInstance {};
class AbstractScene {};

class UIScene : public AbstractScene {
public:
	void setTextboxText(const stl_string&);
};

class SceneFactory {
public:
	stl_shared_ptr<UIScene> createSignScreen(const BlockPos&);
};

class SceneStack {
public:
	void pushScreen(stl_shared_ptr<AbstractScene>, bool);
};

#endif
