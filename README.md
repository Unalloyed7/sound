# Sound Processor

Sound Processor — консольное C++ приложение для цифровой обработки WAV-файлов. Программа читает входной звуковой файл, применяет к нему последовательность фильтров и сохраняет результат в новый WAV-файл.

## Поддерживаемый WAV-формат

Приложение работает с WAV-файлами следующего формата:

* PCM;
* 1 канал;
* 44100 Гц;
* 16 бит на отсчёт;
* отсчёты типа `int16_t`.

## Пример запуска

```bash
sound_processor -i input.wav -o output.wav -f ampl 0.8 -f timestretch 2
```

Также можно создать сигнал без входного файла:

```bash
sound_processor -o output.wav -f generator sin 440 2000
```

## Поддерживаемые фильтры

* `ampl factor` — изменение амплитуды сигнала;
* `normalize [peak]` — нормализация громкости;
* `silence unit start end` — вставка участка тишины;
* `timestretch factor` — изменение длительности сигнала;
* `lowpass window_size` — сглаживание сигнала;
* `generator sin frequency_hz duration_ms` — генерация синусоиды;
* `generator am amplitude carrier_hz modulation_hz depth duration_ms` — генерация AM-сигнала;
* `generator fm amplitude carrier_hz modulation_hz deviation_hz duration_ms` — генерация FM-сигнала.

## Архитектура

Проект разделён на независимые компоненты:

* `ArgsParser` — разбирает аргументы командной строки;
* `Waveform` — хранит звуковой сигнал в памяти;
* `WavFile` — читает и записывает WAV-файлы;
* `IFilter` — общий интерфейс фильтров;
* `Pipeline` — применяет фильтры последовательно;
* `FilterProducers` — создают фильтры из текстовых параметров;
* `CmdLineArgs2PipelineConverter` — преобразует результат парсинга в pipeline;
* `Application` — связывает все компоненты в единый сценарий работы.

## Общий принцип работы

1. Программа разбирает аргументы командной строки.
2. Из списка фильтров создаётся pipeline.
3. WAV-файл читается в объект `Waveform`, либо создаётся пустой сигнал.
4. Pipeline последовательно применяет фильтры.
5. Результат записывается в выходной WAV-файл.
