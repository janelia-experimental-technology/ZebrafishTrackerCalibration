# ZebrafishTrackerCalibration

Authors:

    Peter Polidoro <polidorop@janelia.hhmi.org>

License:

    BSD

## Example Usage

```shell
cd ~/git/ZebrafishTrackerCalibration
./bin/ZebrafishTrackerCalibration
```

## Installation

### Setup Jetson

<https://github.com/janelia-experimental-technology/nvidia_jetson_setup>

# Checkout Git Repositories

```shell
mkdir ~/git
cd ~/git
git clone https://github.com/janelia-experimental-technology/ZebrafishTrackerCalibration.git
```

### Compile

```shell
cd ~/git/ZebrafishTrackerCalibration/
mkdir build
cd build
cmake ..
make
```

### Calculate Background Image

```shell
convert *.png -evaluate-sequence median background.png
```
