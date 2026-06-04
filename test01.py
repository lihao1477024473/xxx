import json
import os
from typing import Any, Dict, Optional


def read_json(file_path: str, encoding: str = "utf-8") -> Optional[Dict[str, Any]]:
    """
    读取 JSON 文件并返回解析后的数据。

    参数:
        file_path (str): JSON 文件的路径
        encoding (str): 文件编码，默认 "utf-8"

    返回:
        Optional[Dict[str, Any]]: 解析后的字典数据，失败返回 None
    """
    if not os.path.exists(file_path):
        print(f"文件不存在: {file_path}")
        return None

    try:
        with open(file_path, "r", encoding=encoding) as f:
            data = json.load(f)
        print(f"成功读取 JSON 文件: {file_path}")
        return data
    except json.JSONDecodeError as e:
        print(f"JSON 解析错误: {e}")
        return None
    except Exception as e:
        print(f"读取文件时发生错误: {e}")
        return None


def write_json(file_path: str, data: Dict[str, Any],
               encoding: str = "utf-8", indent: int = 2,
               ensure_ascii: bool = False) -> bool:
    """
    将数据写入 JSON 文件。

    参数:
        file_path (str): JSON 文件的保存路径
        data (Dict[str, Any]): 要写入的字典数据
        encoding (str): 文件编码，默认 "utf-8"
        indent (int): JSON 缩进空格数，默认 2
        ensure_ascii (bool): 是否转义非 ASCII 字符，默认 False

    返回:
        bool: 写入成功返回 True，失败返回 False
    """
    try:
        with open(file_path, "w", encoding=encoding) as f:
            json.dump(data, f, indent=indent, ensure_ascii=ensure_ascii)
        print(f"成功写入 JSON 文件: {file_path}")
        return True
    except TypeError as e:
        print(f"数据类型无法序列化: {e}")
        return False
    except Exception as e:
        print(f"写入文件时发生错误: {e}")
        return False


if __name__ == "__main__":
    # 示例: 读取当前目录下的 data.json
    result = read_json("data.json")
    if result is not None:
        print(json.dumps(result, indent=2, ensure_ascii=False))

    # 示例: 写入数据到 JSON 文件
    sample_data = {"name": "测试", "value": 123, "items": [1, 2, 3]}
    write_json("output.json", sample_data)
