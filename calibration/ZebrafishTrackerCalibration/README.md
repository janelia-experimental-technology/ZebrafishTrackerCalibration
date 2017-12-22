# ZebrafishTrackerCalibration

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Example Usage

```shell
cd ~/git/ZebrafishTrackerConfig/calibration/ZebrafishTrackerCalibration
./bin/ZebrafishTracker ../ZebrafishTrackerConfig
```

## Installation

### Setup Jetson

<https://github.com/janelia-experimental-technology/nvidia_jetson_setup>

# Checkout Git Repositories

```shell
mkdir ~/git
cd ~/git
git clone https://github.com/janelia-experimental-technology/ZebrafishTrackerConfig.git
```

### Compile

```shell
cd ~/git/ZebrafishTrackerConfig/calibration/
mkdir build
cd build
cmake ..
make
```
