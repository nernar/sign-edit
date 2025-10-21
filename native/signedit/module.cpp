#include <logger.h>
#include <mod.h>
#include <nativejs.h>
#include <symbol.h>
#include <innercore/global_context.h>
#include <innercore/vtable.h>

#include <stl.h>

#include "includes/Actor.h"
#include "includes/Block.h"
#include "includes/BlockActor.h"
#include "includes/BlockSource.h"
#include "includes/ClientInstance.h"
#include "includes/SignBlock.h"

namespace SignEdit {
	stl_shared_ptr<UIScene> openedSignScreen;
	stl_string openedSignMessage;

	void pushSignScreen(LocalPlayer* player, SignBlockActor* sign) {
		ClientInstance* client = player->getClientInstance();
		if (client != nullptr) {
			VTABLE_FIND_OFFSET(ClientInstance_getCurrentSceneStack, _ZTV14ClientInstance, _ZNK14ClientInstance20getCurrentSceneStackEv);
			VTABLE_FIND_OFFSET(ClientInstance_getSceneFactory, _ZTV14ClientInstance, _ZNK14ClientInstance15getSceneFactoryEv);

			SceneStack* sceneStack = VTABLE_CALL<SceneStack*>(ClientInstance_getCurrentSceneStack, client);
			SceneFactory* sceneFactory = VTABLE_CALL<SceneFactory*>(ClientInstance_getSceneFactory, client);
			if (sceneStack != nullptr && sceneFactory != nullptr) {
				stl_shared_ptr<UIScene> scene = sceneFactory->createSignScreen(sign->getPosition());
				SignEdit::openedSignMessage = sign->getMessage();
				if (SignEdit::openedSignMessage.length() != 0) {
					SignEdit::openedSignScreen = scene;
				}
				sceneStack->pushScreen(scene, false);
			}
		}
	}

	void openSign(LocalPlayer* player, int x, int y, int z) {
		BlockSource* region = player->getRegion();
		if (region != nullptr) {
			BlockPos pos(x, y, z);
			BlockActor* blockActor = region->getBlockEntity(pos);
			if (blockActor != nullptr && blockActor->isType(BlockActorType::SIGN)) {
				SignBlock* signBlock = (SignBlock*) region->getBlock(pos)->getBlockLegacy();
				((SignBlockActor*) blockActor)->setType(signBlock->getSignType());
				SignEdit::pushSignScreen(player, (SignBlockActor*) blockActor);
			}
		}
	}
};

class SignEditModule : public Module {
public:
	SignEditModule(): Module("signedit") {}

	virtual void initialize() {
		DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
		HookManager::addCallback(
#if defined(ARM64) || defined(_M_ARM64) || defined(__aarch64__)
			SYMBOL("mcpe", "_ZN20SignScreenController6onOpenEv"),
#else
			SYMBOL("mcpe", "_ZN25MinecraftScreenController6onOpenEv"),
#endif
			LAMBDA((void* instance), {
				if (SignEdit::openedSignScreen) {
					SignEdit::openedSignScreen->setTextboxText(SignEdit::openedSignMessage);
					SignEdit::openedSignMessage.clear();
					SignEdit::openedSignScreen.reset();
				}
			}, ),
			HookManager::LISTENER | HookManager::CALL
		);
	}
};

extern "C" {
	void Java_io_nernar_signedit_SignEdit_openSign(JNIEnv* env, jclass clazz, jint x, jint y, jint z) {
		SignEdit::openSign(GlobalContext::getLocalPlayer(), x, y, z);
	}
}

MAIN {
	new SignEditModule();
}

// JS_MODULE_VERSION(SignEdit, 1);

// JS_EXPORT(SignEdit, openSign, "V(III)", (JNIEnv* env, jint x, jint y, jint z) {
	// SignEdit::openSign(GlobalContext::getLocalPlayer(), x, y, z);
	// return 0;
// });
