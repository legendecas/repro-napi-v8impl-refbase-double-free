{
  "targets": [
    {
      "target_name": "test",
      "sources": [ "test.cc" ]
    },
    {
      "target_name": "test_node_addon_api",
      "sources": [ "node_addon_api.cc" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
