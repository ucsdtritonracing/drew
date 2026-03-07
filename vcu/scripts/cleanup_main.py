import os
import re
from logger import get_logger
from pathlib import Path

logger = get_logger()

BASE_DIR = Path(__file__).resolve().parent
SRC_DIR = BASE_DIR.parent / "Core" / "Src"
MAIN_C = SRC_DIR / "main.c"

def remove_default_task_code(text: str):
    # Remove the definitions block
    text, count = re.subn(
        r'/\*\s*Definitions for defaultTask\s*\*/.*?};\s*\n?',
        '',
        text,
        flags=re.DOTALL
    )
    if count > 0:
        logger.info('Removed defaultTask definition')

    # Remove the forward declaration
    text, count = re.subn(
        r'^\s*void\s+StartDefaultTask\s*\(\s*void\s*\*\s*argument\s*\)\s*;\s*$',
        '',
        text,
        flags=re.MULTILINE
    )
    if count > 0:
        logger.info('Removed StartDefaultTask forward declaration')

    # Remove the task creation line
    text, count = re.subn(
        r'^\s*/\*\s*creation of defaultTask\s*\*/\s*\n\s*defaultTaskHandle\s*=\s*osThreadNew\s*\(\s*StartDefaultTask\s*,\s*NULL\s*,\s*&defaultTask_attributes\s*\)\s*;\s*$',
        '',
        text,
        flags=re.MULTILINE
    )
    if count > 0:
        logger.info('Removed defaultTask creation')

    # Remove the function implementation
    text, count = re.subn(
        r'/\*\s*USER CODE BEGIN Header_StartDefaultTask\s*\*/.*?\n\}',
        '',
        text,
        flags=re.DOTALL
    )
    if count > 0:
        logger.info('Removed StartDefaultTask implementation')

    return text


def cleanup_main():
    if not MAIN_C.exists():
        logger.error(f"File not found: {MAIN_C}")
        return
    
    text = MAIN_C.read_text()
    cleaned_text = remove_default_task_code(text)
    if cleaned_text:
        logger.info("Successfully removed defaultTask code from main")

    MAIN_C.write_text(cleaned_text)
    logger.info(f"Wrote cleaned code to {MAIN_C}")
