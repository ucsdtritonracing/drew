import logging
from pathlib import Path
from logger import get_logger

logger = get_logger()

BASE_DIR = Path(__file__).resolve().parent
SRC_DIR = BASE_DIR.parent / "Core" / "Src"
MAIN_CPP = SRC_DIR / "main.cpp"
MAIN_C = SRC_DIR / "main.c"

def convert_main_to_c():
    if not MAIN_CPP.exists():
        logger.error(f"Could not find {MAIN_CPP}")
        return
    
    MAIN_CPP.rename(MAIN_C)
    logger.info(f"Renaming {MAIN_CPP} to {MAIN_C}")

def convert_main_to_cpp():
    if not MAIN_C.exists():
        logger.error(f"Could not find {MAIN_C}")
        return
    
    MAIN_C.rename(MAIN_CPP)
    logger.info(f"Renamed {MAIN_C} to {MAIN_CPP}")
