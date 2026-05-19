#!/bin/bash


if [ "$#" -lt 3 ]; then
    echo "Использование: $0 <суффикс> <длина> <символ> [каталог]"
    exit 1
fi

SUFFIX="$1"        
TARGET_LEN="$2"    
CHAR="$3"          
DIR="${4:-.}"    


if ! [[ "$TARGET_LEN" =~ ^[1-9][0-9]*$ ]]; then
    echo "Ошибка: длина должна быть натуральным числом."
    exit 1
fi

if [ "${#CHAR}" -ne 1 ]; then
    echo "Ошибка: символ дополнения должен быть ровно одним символом."
    exit 1
fi

if [ ! -d "$DIR" ]; then
    echo "Ошибка: каталог '$DIR' не существует."
    exit 1
fi


for filepath in "$DIR"/*"$SUFFIX"; do
    [ -e "$filepath" ] || continue      # проверка: существует ли файл
    [ -f "$filepath" ] || continue      # проверка: является ли обычным файлом

    filename="$(basename "$filepath")"  # отрезать директорию
    name="${filename%$SUFFIX}"          # отрезать суффикс
    current_len="${#name}"                 

    if [ "$current_len" -ge "$TARGET_LEN" ]; then     # если больше или равно длине, то ничего не менять
        echo "Длина уже $current_len: $filename"
        continue
    fi

    add=$(( TARGET_LEN - current_len ))

    padding="$(printf "%${add}s" | tr ' ' "$CHAR")"   # создать пробелы и заменить их на нужный символ

    new_name="${name}${padding}${SUFFIX}"
    new_path="$(dirname "$filepath")/$new_name"

    if [ -e "$new_path" ]; then
        echo "Такой файл уже существует: $new_name"
        continue
    fi

    mv -- "$filepath" "$new_path"
    echo "Переименовано: $filename → $new_name"
done

# & 'C:\Program Files\Git\bin\bash.exe' ./9_var.sh .txt 10 '@' ./labi