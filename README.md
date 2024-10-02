# CBuild

## Prerequisites

Before you begin, ensure you have the following installed:

| Dependency | Version |
| :--- | :---|
| **GCC** | *lastes* |
| libjson-c-dev | *lastes* |

## Config Project

In you project directory you will see a **`project.json`** that is the CBuild project file, to configere CBuild:

```.json
{
  "targets": [
    {
      "name": "cbuild",
      "source_file": "main.cpp",
      "build_mode": "debug"
    }
  ]
}
```

## Building Project

```.sh
gcc -o CBuild.exe build.c
```

## Run Project

```.sh
./CBuild.exe <target>
```
