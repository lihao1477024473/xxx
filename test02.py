from typing import List, Optional
import os


class TextFileHandler:
    """
    简单的文本文件读写工具类。

    功能：
    - 读取整个文本 (`read_text`) 返回 str
    - 读取行列表 (`read_lines`) 返回 List[str]
    - 覆盖写入文本 (`write_text`)
    - 追加写入文本 (`append_text`)
    - 写入多行 (`write_lines`)

    构造参数：
    - `encoding` 文本编码（默认 utf-8）
    - `errors` 解码错误处理策略（默认 strict）
    """

    def __init__(self, encoding: str = "utf-8", errors: str = "strict") -> None:
        self.encoding = encoding
        self.errors = errors

    def exists(self, path: str) -> bool:
        return os.path.exists(path)

    def read_text(self, path: str) -> Optional[str]:
        if not self.exists(path):
            print(f"文件不存在: {path}")
            return None
        try:
            with open(path, "r", encoding=self.encoding, errors=self.errors) as f:
                return f.read()
        except Exception as e:
            print(f"读取文件失败: {e}")
            return None

    def read_lines(self, path: str) -> Optional[List[str]]:
        if not self.exists(path):
            print(f"文件不存在: {path}")
            return None
        try:
            with open(path, "r", encoding=self.encoding, errors=self.errors) as f:
                return f.readlines()
        except Exception as e:
            print(f"读取文件行失败: {e}")
            return None

    def write_text(self, path: str, content: str) -> bool:
        try:
            with open(path, "w", encoding=self.encoding, errors=self.errors) as f:
                f.write(content)
            return True
        except Exception as e:
            print(f"写入文件失败: {e}")
            return False

    def append_text(self, path: str, content: str) -> bool:
        try:
            with open(path, "a", encoding=self.encoding, errors=self.errors) as f:
                f.write(content)
            return True
        except Exception as e:
            print(f"追加写入失败: {e}")
            return False

    def write_lines(self, path: str, lines: List[str], newline: str = "\n") -> bool:
        try:
            with open(path, "w", encoding=self.encoding, errors=self.errors) as f:
                f.write(newline.join(lines))
            return True
        except Exception as e:
            print(f"写入多行失败: {e}")
            return False


if __name__ == "__main__":
    handler = TextFileHandler()
    sample_path = "sample.txt"

    # 写入示例
    ok = handler.write_text(sample_path, "第一行\n第二行\n")
    if ok:
        print("写入成功")

    # 追加示例
    handler.append_text(sample_path, "第三行\n")

    # 读取全部
    text = handler.read_text(sample_path)
    print("读取全部内容:\n", text)

    # 读取行
    lines = handler.read_lines(sample_path)
    print("读取行:\n", lines)
