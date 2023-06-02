## example

测试环境: macOS M2(Ventura)

安装依赖:

```shell
brew install freeglut
brew install --cask xquartz
export DISPLAY=:0
```

编译步骤:

```shell
meson setup build
meson compile -C build
```

需要启动xquartz服务. 否则会出现错误: freeglut failed to open display

参考资料:

https://github.com/matrixcascade/PainterEngine

https://www.zhihu.com/question/587721560/answer/3053891651

https://www.painterengine.com/main/instances/instance2023053101/index.html

## designer

测试环境: Arch Linux

安装依赖:

```shell
pacman -S emscripten
```

编译步骤:

```shell
cd designer/platform/webassembly
make

# cp designer/platform/webassembly/PainterEngine.* frontend/
# mv frontend/PainterEngine.html frontend/index.html
```

参考资料:

https://www.painterengine.com/main/instances/instance2023053101/project.zip


## fox-jump

编译步骤:

```shell
cd fox-jump/platform/webassembly
make -j`nproc`
```

参考资料:

https://www.painterengine.com/main/instances/instance2021070501/project.zip
