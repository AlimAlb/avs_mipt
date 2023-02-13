class client:
    def __init__(self: int, indentificator_numder:int = None,
     accounty:int = None, 
    amount:int = None,
     code:int = None):
        self.indentificator_numder = indentificator_numder
        self.accounty = accounty
        self.amount = amount
        self.code = code

    def __str__(self) -> str:
        return f"{self.indentificator_numder}, {self.accounty}, {self.amount}, {self.code}"
        
        