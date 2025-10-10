#ifndef SIGNEDIT_BLOCKSOURCE_H
#define SIGNEDIT_BLOCKSOURCE_H

#ifndef SIGNEDIT_BLOCK_H
	class Block;
#endif
#ifndef SIGNEDIT_BLOCKACTOR_H
	class BlockActor;
#endif

struct BlockPos {
	int x, y, z;
	BlockPos(float, float, float);
	BlockPos(double, double, double);
	BlockPos(int, int, int);
};

class BlockSource {
public:
	Block* getBlock(const BlockPos&) const;
	BlockActor* getBlockEntity(const BlockPos&);
};

#endif
