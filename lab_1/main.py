from utils import currencu_referense, customer_base, account_encryption, amount_encryption, code_encryption, decryption, cls, show_balances, flush_to_csv
from balance import balance
import sys

bank = []
bank_cipher = []
decipher = []
val = []
keys = []

total_balance = balance(dict(), dict())

def main():
    #cls()    
    currencu_referense(val, sys.argv[1])
    #n = int(input('Введите число клиентов банка:'))
    #cls()
    customer_base(sys.argv[2], bank, val, total_balance)
    n = len(bank)
    account_encryption(n, bank, bank_cipher, keys)
    amount_encryption(n, bank, bank_cipher, keys)
    code_encryption(n, bank, bank_cipher, keys)
    decryption(n, bank_cipher, decipher, keys, total_balance)
    show_balances(total_balance, val)
    flush_to_csv(bank, bank_cipher, decipher, val, keys, total_balance)

main()




