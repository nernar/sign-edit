#ifndef SIGNEDIT_BLOCKACTOR_H
#define SIGNEDIT_BLOCKACTOR_H

#ifndef SIGNEDIT_BLOCKSOURCE_H
	struct BlockPos;
#endif

enum BlockActorType : int {
	SIGN = 4
};

class BlockActor {
public:
	BlockPos& getPosition() const;
	bool isType(BlockActorType) const;
};

#endif
