#ifndef SIGNEDIT_SIGNBLOCK_H
#define SIGNEDIT_SIGNBLOCK_H

#include <stl.h>
#include <stl/string>

#ifndef SIGNEDIT_BLOCK_H
	class BlockLegacy;
#endif
#ifndef SIGNEDIT_BLOCKACTOR_H
	class BlockActor;
#endif

class SignBlockActor : public BlockActor {
public:
	enum SignType : int {};

	stl::string& getMessage();
	void setType(SignBlockActor::SignType);
};

class SignBlock : public BlockLegacy {
public:
	SignBlockActor::SignType getSignType() const;
};

#endif
