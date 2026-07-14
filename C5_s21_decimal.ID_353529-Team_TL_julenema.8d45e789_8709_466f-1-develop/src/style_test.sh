#!/bin/bash

# clang format
SOURCE_DIR="${1:-.}"           

process_files() {
    local dir="$1"
    
    # Проходим по всем элементам в текущей папке
    for item in "$dir"/*; do
        # Пропускаем, если нет файлов
        [ -e "$item" ] || continue
        
        if [ -d "$item" ]; then
            # Это папка - рекурсивно заходим в неё
            process_files "$item"
        elif [ -f "$item" ]; then
            # Проверяем, подходит ли файл под паттерн
            case "$item" in
                *.c|*.h)
                    echo "Найден файл: $item"
                    clang-format -i "$item"
                    clang-format -n "$item"
                    ;;
            esac
        fi
    done
}

process_files "$SOURCE_DIR"
echo "Готово!"
