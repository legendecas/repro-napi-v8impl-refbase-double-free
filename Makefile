all:
	node -v
	node-gyp rebuild
	node --expose_gc index.j
