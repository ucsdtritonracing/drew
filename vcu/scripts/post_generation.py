from logger import get_logger
from convert_main import convert_main_to_cpp
from cleanup_main import cleanup_main

logger = get_logger()


def main():
    logger.info("[Post-generation Script] Started...")
    cleanup_main()      # Remove defaultTask from main.c code
    convert_main_to_cpp()
    logger.info("[Post-generation Script] Finished!")


if __name__ == "__main__":
    main()