import logging
from pathlib import Path

def get_logger(name: str = "scripts", log_filename: str = "scripts.log"):
    log_dir = Path(__file__).resolve().parent
    log_file = log_dir / log_filename

    if not logging.getLogger().hasHandlers():
        logging.basicConfig(
            level=logging.INFO,
            format="[%(asctime)s] [%(levelname)s] %(message)s",
            datefmt="%Y-%m-%d %H:%M:%S",
            handlers=[
                logging.FileHandler(log_file, mode="a", encoding="utf-8"),
                logging.StreamHandler()
            ]
        )

    return logging.getLogger(name)