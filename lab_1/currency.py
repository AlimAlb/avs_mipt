class currency:
    def __init__(self, name: str = None, letter_code: str = None,
     curr_code: int = None):
        self.name = name
        self.letter_code = letter_code
        self.curr_code = curr_code

    def __str__(self):
        return f"{self.name}, {self.letter_code}, {self.curr_code}"

    



