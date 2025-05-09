# GainPlugin
NAME HAS TO CHANGE AS WELL

A JUCE-based VST plugin that integrates the [Neural Amp Modeler (NAM)](https://github.com/sdatkinson/neural-amp-modeler) to deliver high-quality, neural-network-based amp simulation in a flexible and extensible audio plugin framework.

## 🎯 Project Overview

This project aims to bring the powerful capabilities of the Neural Amp Modeler into the JUCE ecosystem. By embedding NAM into a JUCE VST plugin, we enable real-time neural amplifier emulation within any modern DAW that supports VST3.

The current implementation focuses on establishing a clean architecture, with an emphasis on future scalability, DSP abstraction, and performance optimization.

---

## 🔧 Features

* ✅ Real-time guitar amp emulation using Neural Amp Modeler
* ✅ JUCE-based VST3 plugin
* ✅ Modular and extensible architecture
* ✅ Initial refactoring for better scalability and maintenance

---

## 📌 TODOs / Roadmap

* [ ] Implement multithreading for smoother, non-blocking parameter updates (especially for the NAM mappers)
* [ ] Refactor DSP framework to define a common interface across implementations
* [ ] Transition from per-sample processing to vectorized (SIMD) processing for performance gains

  * Consider integrating Intel IPP or similar vector libraries for optimized audio processing

---

## 🧰 Built With

* [JUCE](https://juce.com/) – C++ framework for developing audio applications and plugins
* [Neural Amp Modeler](https://github.com/sdatkinson/neural-amp-modeler) – Open-source neural network-based amp simulator

---

## 📦 Installation

Clone the repository and open the project with **Projucer** or directly in your preferred IDE if using CMake-based workflow.
Ensure you have the necessary dependencies and paths configured for both JUCE and Neural Amp Modeler.

```bash
https://github.com/MariusBoucard/GainPlugin.git
cd gainPlugin
# Open in Projucer or build with CMake
```

---

## 🧪 Status

The amp emulation is functional, and the plugin is ready for testing and feedback. Major refactoring work has been completed to support cleaner code separation and future features.

---

## 🤝 Contributing

Contributions are welcome! Please fork the repo and submit a pull request or open an issue for suggestions and bugs.
