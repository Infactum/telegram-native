# TDLib bindings for 1C:Enterprise

[![Build status](https://ci.appveyor.com/api/projects/status/2ium20h8q5moqkj8/branch/master?svg=true)](https://ci.appveyor.com/project/Infactum/telegram-native/branch/master)

Cross platform native API [add-in](https://1c-dn.com/library/add_in_creation_technology/) bindings for [TDLib](https://github.com/tdlib/td) (Telegram Database library) that allows to create full featured telegram clients within 1C:Enterprise platform.

## How to use

```bsl
AttachAddIn("<path>", "Telegram", AddInType.Native);
ComponentObject = New("AddIn.Telegram.TelegramNative");
```

## Latest builds
[Windows x86](https://ci.appveyor.com/api/projects/Infactum/telegram-native/artifacts/telegram_native_x86.dll?job=Image%3A%20Visual%20Studio%202017%3B%20Environment%3A%20TARGET_PLATFORM%3Dx86)

[Windows x64](https://ci.appveyor.com/api/projects/Infactum/telegram-native/artifacts/telegram_native_x64.dll?job=Image%3A%20Visual%20Studio%202017%3B%20Environment%3A%20TARGET_PLATFORM%3Dx64)

[Linux x64](https://ci.appveyor.com/api/projects/Infactum/telegram-native/artifacts/telegram_native_x64.so?job=Image%3A%20Ubuntu%3B%20Environment%3A%20TARGET_PLATFORM%3Dx64)
