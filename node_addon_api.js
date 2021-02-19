const addon = require('./build/Release/test_node_addon_api.node');
// attach those data on global for simplicity.
{ global['it'] = new addon.MyObject(); }
global.cleanup = () => {
  // force gc after addon node_env has been teardown
  delete global.it;
  global.gc();
}
