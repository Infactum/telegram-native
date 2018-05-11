# TDLib bindings for 1C:Enterprise

[![Build status](https://ci.appveyor.com/api/projects/status/2ium20h8q5moqkj8/branch/master?svg=true)](https://ci.appveyor.com/project/Infactum/telegram-native/branch/master) [![CircleCI](https://circleci.com/gh/Infactum/telegram-native/tree/master.svg?style=shield)](https://circleci.com/gh/Infactum/telegram-native/tree/master)

Cross platform native API [add-in](https://1c-dn.com/library/add_in_creation_technology/) bindings for [TDLib](https://github.com/tdlib/td) (Telegram Database library) that allows to create full featured telegram clients within 1C:Enterprise platform.

## How to use

```bsl
AttachAddIn("<path>", "Telegram", AddInType.Native);
ComponentObject = New("AddIn.Telegram.TelegramNative");
```

## Latest builds
[Windows x86](https://ci.appveyor.com/api/projects/Infactum/telegram-native/artifacts/build/telegram_native.dll?job=Platform%3A%20x86)

[Windows x64](https://ci.appveyor.com/api/projects/Infactum/telegram-native/artifacts/build/telegram_native.dll?job=Platform%3A%20x64)

[Linux x64](https://circleci.com/api/v1.1/project/github/Infactum/telegram-native/latest/artifacts?circle-token=145d0d1437eca8a2efa454742fab4875ad3ab734)
