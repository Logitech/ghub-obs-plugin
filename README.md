# Logitech G OBS Studio Plugin

This is a plugin for [OBS Studio](https://obsproject.com/) exposing actions to [G HUB](https://www.logitechg.com/en-us/innovation/g-hub.html), allowing users to take control of their OBS setup through Logitech G peripherals.

## File structure

* `obs_plugin` directory contains the source code for the plugin.
* `obs_plugin/libs` directory contains precompiled libraries of [OBS Studio](https://github.com/obsproject/obs-studio).
* `other` directory contains libraries and other dependencies for the plugin.
* `other/obs-studio` directory contains source code for an API-only version of [OBS Studio](https://github.com/obsproject/obs-studio).

## Libraries and Other Dependencies
* [Asio C++ Library](https://github.com/chriskohlhoff/asio)
* [JSON for Modern C++](https://github.com/nlohmann/json)
* [OBS Studio](https://github.com/obsproject/obs-studio)
* [WebSocket++](https://github.com/zaphoyd/websocketpp)
