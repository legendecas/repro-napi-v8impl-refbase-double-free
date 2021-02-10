all:
	node -v
	node-gyp rebuild
	node index.js
