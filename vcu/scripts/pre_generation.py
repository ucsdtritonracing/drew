from logger import get_logger
from convert_main import convert_main_to_c

logger = get_logger()


def main():
    logger.info("[Pre-generation Script] Started...")
    convert_main_to_c()
    logger.info("[Pre-generation Script] Finished!")


if __name__ == "__main__":
    main()