class key:
    def __init__(self, ind_num: int = None,
     key_1: int = None, key_2: int = None,
      key_3: int = None):
        self.ind_num = ind_num
        self.key_1 = key_1
        self.key_2 = key_2
        self.key_3 = key_3

    def __str__(self) -> str:
        return f"{self.ind_num} { self.key_1} {self.key_2} {self.key_3}"