import re
import matplotlib.pyplot as plt
import os

def parse_info_file(filename):
    """Парсит файл info.txt и возвращает список словарей с данными."""
    data = []
    if not os.path.exists(filename):
        print(f"Файл {filename} не найден!")
        return data

    with open(filename, 'r', encoding='utf-8') as f:
        content = f.read()

    # Разбиваем на блоки по разделителю
    blocks = content.strip().split("----------------------------------------")
    
    for block in blocks:
        if not block.strip():
            continue
        
        entry = {}
        # Парсинг размера матриц (например, 200x200)
        size_match = re.search(r"Размер матриц:\s*(\d+)x(\d+)", block)
        if size_match:
            entry['size'] = int(size_match.group(1))
        
        # Парсинг конфигурации блока (например, 8x8)
        config_match = re.search(r"Конфигурация блока:\s*(\d+)x(\d+)", block)
        if config_match:
            entry['block_config'] = f"{config_match.group(1)}x{config_match.group(2)}"
            
        # Парсинг времени выполнения
        time_match = re.search(r"Время выполнения:\s*(\d+)\s*мс", block)
        if time_match:
            entry['time_ms'] = int(time_match.group(1))
            
        # Парсинг объема работы (опционально, для оси X, если захочешь)
        ops_match = re.search(r"Объём работы:\s*(\d+)", block)
        if ops_match:
            entry['operations'] = int(ops_match.group(1))

        if 'size' in entry and 'time_ms' in entry and 'block_config' in entry:
            data.append(entry)
            
    return data

def plot_results(data):
    if not data:
        print("Нет данных для построения графиков.")
        return

    # Создаем папку для графиков, если нет
    os.makedirs("graphs", exist_ok=True)

    # --- График 1: Зависимость времени от размера матрицы для разных конфигураций блоков ---
    plt.figure(figsize=(12, 6))
    
    # Получаем уникальные конфигурации блоков
    block_configs = sorted(list(set(item['block_config'] for item in data)))
    
    for config in block_configs:
        # Фильтруем данные для текущей конфигурации
        subset = [item for item in data if item['block_config'] == config]
        # Сортируем по размеру матрицы
        subset.sort(key=lambda x: x['size'])
        
        sizes = [item['size'] for item in subset]
        times = [item['time_ms'] for item in subset]
        
        plt.plot(sizes, times, marker='o', label=f'Блок {config}')

    plt.title('Зависимость времени выполнения CUDA умножения матриц от размера\n(разные конфигурации блоков)', fontsize=14)
    plt.xlabel('Размер матрицы N (N x N)', fontsize=12)
    plt.ylabel('Время выполнения (мс)', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend(title="Конфигурация блока")
    plt.xticks(sorted(list(set(item['size'] for item in data))))
    
    # Сохраняем первый график
    plt.tight_layout()
    plt.savefig('graphs/time_vs_size.png', dpi=300)
    plt.show()
    print("График 'time_vs_size.png' сохранен в папку 'graphs'.")

    # --- График 2: Сравнение производительности разных блоков при фиксированных размерах ---
    # Выбираем несколько ключевых размеров для сравнения
    key_sizes = [800, 1200, 1600, 2000]
    
    plt.figure(figsize=(12, 6))
    
    x_pos = range(len(block_configs))
    width = 0.25
    
    for i, size in enumerate(key_sizes):
        times_for_size = []
        for config in block_configs:
            # Ищем время для данного размера и конфигурации
            time_val = 0
            for item in data:
                if item['size'] == size and item['block_config'] == config:
                    time_val = item['time_ms']
                    break
            times_for_size.append(time_val)
        
        # Смещаем столбцы для каждого размера
        offset = width * (i - 1.5) # Центрируем группу столбцов
        plt.bar([p + offset for p in x_pos], times_for_size, width, label=f'N={size}')

    plt.title('Сравнение времени выполнения для разных конфигураций блоков\nпри различных размерах матриц', fontsize=14)
    plt.xlabel('Конфигурация блока', fontsize=12)
    plt.ylabel('Время выполнения (мс)', fontsize=12)
    plt.xticks(x_pos, block_configs)
    plt.grid(True, axis='y', linestyle='--', alpha=0.7)
    plt.legend(title="Размер матрицы")
    
    # Сохраняем второй график
    plt.tight_layout()
    plt.savefig('graphs/blocks_comparison.png', dpi=300)
    plt.show()
    print("График 'blocks_comparison.png' сохранен в папку 'graphs'.")

if __name__ == "__main__":
    # Укажи путь к файлу с результатами
    info_file = "info.txt" 
    data = parse_info_file(info_file)
    plot_results(data)