all:
	node -v
	node-gyp rebuild
	node index.js
	node --expose_gc node_addon_api.js
