# G'DOS

G'DOS is a ROM-native DOS made for use in the Blitz 68030 homebrew. Support is planned for ELF loading and relocation, filesystem and driver implementations, etc.

## State of operation

G'DOS is incredibly bare and basically nonexistant at this point. Working code exists for functions like ELF loading and relocation, but has not yet been integrated due to the lack of basic framework. It is developed alongside Blitz and is currently being actively worked on.

## Building

To make, just run this in the project root

```
make PLATFORM=<platform>
```

Where <platform> is the name of the desired platform (sourced from the dir names in ./src/arch).
BSVC is the main development platform. You can use ```make PLATFORM=BSVC run``` to make everything and immediately run it in the [BSVC Simulator](https://github.com/BSVC/bsvc), so long as you have it installed

## Contributing

G'DOS is a personal project at the moment, and will be until i have a basic framework in place.

## Authors

* **Carson Herrington / NotArtyom** - *All Code* - [Website](http://notartyoms-box.com)

