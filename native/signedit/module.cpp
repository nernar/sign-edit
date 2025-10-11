#include <logger.h>
#include <mod.h>
#include <nativejs.h>
#include <symbol.h>
#include <innercore/global_context.h>
#include <innercore/vtable.h>

#include <stl.h>
#include <stl/memory>
#include <stl/string>

#include "includes/Actor.h"
#include "includes/Block.h"
#include "includes/BlockActor.h"
#include "includes/BlockSource.h"
#include "includes/ClientInstance.h"
#include "includes/SignBlock.h"

namespace SignEdit {
	stl::shared_ptr<UIScene> openedSignScreen;
	stl::string openedSignMessage;
	void pushSignScreen(LocalPlayer* player, SignBlockActor* sign) {
		ClientInstance* client = player->getClientInstance();
		if (client != nullptr) {
			VTABLE_FIND_OFFSET(ClientInstance_getCurrentSceneStack, _ZTV14ClientInstance, _ZNK14ClientInstance20getCurrentSceneStackEv);
			VTABLE_FIND_OFFSET(ClientInstance_getSceneFactory, _ZTV14ClientInstance, _ZNK14ClientInstance15getSceneFactoryEv);
			SceneStack* sceneStack = VTABLE_CALL<SceneStack*>(ClientInstance_getCurrentSceneStack, client);
			SceneFactory* sceneFactory = VTABLE_CALL<SceneFactory*>(ClientInstance_getSceneFactory, client);
			if (sceneStack != nullptr && sceneFactory != nullptr) {
				SignEdit::openedSignMessage = sign->getMessage();
				stl::shared_ptr<UIScene> scene = sceneFactory->createSignScreen(sign->getPosition());
				SignEdit::openedSignScreen = scene;
				sceneStack->pushScreen(scene, false);
			}
		}
	}
	void openSign(LocalPlayer* player, int x, int y, int z, int aboba = 0) {
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
	void updateTextbox() {
		if (SignEdit::openedSignScreen) {
			// native visual updating
			// SignEdit::openedSignScreen->setTextboxText(SignEdit::openedSignMessage);
			SignEdit::openedSignScreen.reset();
			// AppPlatform_android* platform = (AppPlatform_android*) GlobalContext::getAppPlatform();
			// edittext updating
			// platform->updateTextBoxText(SignEdit::openedSignMessage);
		}
	}
};

class SignEditModule : public Module {
public:
	SignEditModule(): Module("signedit") {}

	virtual void initialize() {
		DLHandleManager::initializeHandle("libminecraftpe.so", "mcpe");
		HookManager::addCallback(
			SYMBOL("mcpe", "_ZN20SignScreenController6onOpenEv"),
			LAMBDA((void* controller), {
				if (SignEdit::openedSignScreen) {
					// native visual updating
					SignEdit::openedSignScreen->setTextboxText(SignEdit::openedSignMessage);
					SignEdit::openedSignScreen.reset();
				}
			}, ),
			HookManager::CALL | HookManager::LISTENER
		);
	}
};

extern "C" {
	void Java_io_nernar_signedit_SignEdit_openSign(JNIEnv* env, jclass clazz, jint x, jint y, jint z) {
		SignEdit::openSign(GlobalContext::getLocalPlayer(), x, y, z);
	}
	void Java_io_nernar_signedit_SignEdit_updateTextbox(JNIEnv* env, jclass clazz) {
		// SignEdit::updateTextbox();
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
// JS_EXPORT(SignEdit, updateTextbox, "V()", (JNIEnv* env) {
	// SignEdit::updateTextbox();
	// return 0;
// });
