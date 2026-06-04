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


if __name__ == "__main__":
    # 示例: 读取当前目录下的 data.json
    result = read_json("data.json")
    if result is not None:
        print(json.dumps(result, indent=2, ensure_ascii=False))
