from dataclasses import dataclass
from abc import ABC, abstractmethod
from typing import TypeVar
from enum import Enum, auto

DisplayRef = TypeVar('display reference') # opaque data/pointer
DisplayCon = TypeVar('open display connection') # opaque data/pointer

class DDC_Interface(ABC):
    @dataclass
    class Monitor:
        display_idx: int
        display_ref: DisplayRef
        model: str
        manufacturer: str
        vcp_ver: str

        def __str__(self):
            return f'{self.display_idx}: [{self.manufacturer}] {self.model}'

    @dataclass
    class VCP_result:
        value: int
        max: int

    class VCP(Enum):
        BRIGHTNESS = 0x10

    @abstractmethod
    def get_monitors() -> list[Monitor]:
        pass

    @abstractmethod
    def open_monitor(mon: Monitor) -> DisplayCon:
        # context manager for with statement
        pass

    @abstractmethod
    def read_vcp(con: DisplayCon, code: int) -> VCP_result:
        pass

    @abstractmethod
    def write_vcp(con: DisplayCon, code: int, value: int):
        pass