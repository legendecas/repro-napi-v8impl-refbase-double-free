const addon = require('./build/Release/test.node');
global.gc_tick = () => {
  console.log('force gc');
  global.gc();
};

{
  for (let i = 0; i < 1000; i++) {
    const obj = new addon.MyObject();
  }
}
