from client import client
from currency import currency 
from key import key
import os
import pandas as pd

def cls():
    os.system('cls' if os.name=='nt' else 'clear')


def show_balances(total_balance, val):
    print('============Контрольные суммы до шифрования============')
    for key in total_balance.curr_start.keys():
        print(f"{find_curr_name(int(key), val)} -- {total_balance.curr_start[key]}")

    print('============Контрольные суммы после шифрования============')
    for key in total_balance.curr_cipher.keys():
        print(f"{find_curr_name(key, val)} -- {total_balance.curr_cipher[key]}")
    

def find_curr_name(code, val):
    for item in val:
        if item.curr_code == code:
            return item.letter_code

def currencu_referense(arr_cur, path):
    df = pd.read_csv(path)
    for i in range(len(df)):
        if check_curr(int(list(df.iloc[i])[2]), arr_cur):
            continue
        arr_cur.append(currency(list(df.iloc[i])[0],list(df.iloc[i])[1], int(list(df.iloc[i])[2])))
    #cls()
                
def check_curr(code, val):
    for item in val:
        if item.curr_code == code:
            return True
    return False

def input_curr(id_number, val):
    inp = input('Введите код валюты счета --> ')
    while not(check_curr(int(inp), val)):
        inp = input(f'Некорректная валюта! Введите код валюты счета --> ')
    return inp




def customer_base(path, bank: list, val: list, total_balance: list):

    df = pd.read_csv(path)
    for i in range(len(df)):
        lst = list(df.iloc[i])
        bank.append(client(
            lst[0],
            lst[1],
            lst[2],
            lst[3]
         ))
        if lst[3] in total_balance.curr_start.keys():
            total_balance.curr_start[lst[3]] += int(lst[2])
        else:
            total_balance.curr_start[lst[3]] = int(lst[2])



def find_last2_digs(num):
    digs = []
    s = str(num)
    i = len(str(num))-1
    while i > 0:
        if s[i] != '0' and len(digs) < 2:
            digs.append(int(s[i]))
        i -= 1
    return digs


def find_nth_dig(num, n):
    s = str(num)
    i = n-1
    while i < len(s):
        if s[i] != '0':
            return int(s[i])
        i += 1


def find_third_dig(num):
    s = str(num)
    i = 0
    counter = 0
    while i < len(s):
        if s[i] != '0':
            counter +=1
            if counter == 3:
                return int(s[i])
        i += 1


def find_2_nums(num):
    digs_1 = []
    digs_2 = []
    s = str(num)
    i = len(s)-1
    while i > 0:
        if s[i] != '0' and len(digs_1) <= 2:
            digs_1.append(s[i])
        i -= 1
    i = 0
    while i < len(s):
        if s[i] != '0' and len(digs_2) <= 2:
            digs_2.append(s[i])
        i += 1
    return int(''.join(digs_1[::-1])), int(''.join(digs_2))
    

def resheto(n):
    lst = [False if(i== 0 or i == 1) else True for i in range(10)]


    counter = 0
    i = 0
    while True:
        if lst[i]:
            for j in range(i+1, len(lst)):
                if j % i == 0:
                    lst[j] = False
        i += 1

        if i == len(lst):
            counter = sum(lst)
            if counter >= n:
                break
            _ = [lst.append(True) for i in range(10)]
            i = 0

    return [i for i in range(len(lst)) if lst[i]][n-1]


def acc_cif(id_num, acc):
    return find_nth_dig(acc, 2)*resheto(sum(find_last2_digs(id_num)))
    

def account_encryption(n, bank, bank_cipher, keys):
    _ = [keys.append(key(bank[i].indentificator_numder)) for i in range(n)]
    _ = [bank_cipher.append(client(bank[i].indentificator_numder)) for i in range(n)]
    for i in range(n):
        key_1 = acc_cif(int(bank[i].indentificator_numder), int(bank[i].accounty))
        acc_cifed = int(bank[i].accounty) + key_1
        bank_cipher[i].accounty = acc_cifed
        keys[i].key_1 = key_1

def gcd(a, b):
    if(b == 0):
        return abs(a)
    else:
        return gcd(b, a % b)


def amm_cif(id_num):
    n1, n2 = find_2_nums(id_num)
    return find_third_dig(id_num) * gcd(n1,n2)
    

def amount_encryption(n, bank, bank_cipher, keys):
    for i in range(n):
        key_2 = amm_cif(int(bank[i].indentificator_numder))
        amm_cifed = int(bank[i].amount) + key_2
        bank_cipher[i].amount = amm_cifed
        keys[i].key_2 = key_2

def last_three_sum(id_num, curr_code):
    m = int(str(curr_code)[0])
    n = sum(map(int, str(id_num)[-3:]))
    if n == 0:
        n = 27
        return n, m
    else:
        return (n,m) if n > m else (m,n)

def fac(n):
    if n == 1 or n == 0:
        return 1
    else:
        return fac(n - 1) * n


def combination(n, k):
    return int((fac(n) / (fac(k) * fac(n - k))))
    

def code_cif(id_num, curr_code):
    return combination(*last_three_sum(id_num, curr_code))
    

def code_encryption(n, bank, bank_cipher, keys):
    for i in range(n):
        key_3 = code_cif(bank[i].accounty, bank[i].code)
        code_cifed = int(bank[i].code) + int(key_3)
        bank_cipher[i].code = code_cifed
        keys[i].key_3 = key_3


def decryption(n, bank_cipher, desciper, keys, total_balance):
    for i in range(n):
        desciper.append(client(
            bank_cipher[i].indentificator_numder,
            int(bank_cipher[i].accounty) - keys[i].key_1,
            int(bank_cipher[i].amount) - keys[i].key_2,
            int(bank_cipher[i].code) - int(keys[i].key_3),
        ))
        if desciper[i].code in total_balance.curr_cipher.keys():
            total_balance.curr_cipher[desciper[i].code] += desciper[i].amount
        else:
            total_balance.curr_cipher[desciper[i].code] = desciper[i].amount




def flush_to_csv(bank, bank_cipher, decipher, val, keys, total_balance):
    client_data = {
        'id_num': [],
        'account': [],
        'amount': [],
        'curr_code': []
    }

    client_data_ciph = {
        'id_num': [],
        'account': [],
        'amount': [],
        'curr_code': []
    }

    client_data_decipher = {
        'id_num': [],
        'account': [],
        'amount': [],
        'curr_code': []
    }

    currencies = {
        'name': [],
        'letter_code': [],
        'code': []
    }

    keys_o = {
        'id_num': [],
        'key_1': [],
        'key_2': [],
        'key_3': []
    }

    totals = {
        'index':[],
        'pre_ciph': [],
        'post_ciph' :[]
    }


    for key in total_balance.curr_start.keys():
        totals['index'].append(find_curr_name(int(key), val))
        totals['pre_ciph'].append(total_balance.curr_start[key])
        totals['post_ciph'].append(total_balance.curr_cipher[int(key)])


    for i in range(len(bank)):
        client_data['id_num'].append(bank[i].indentificator_numder)
        client_data['account'].append(bank[i].accounty)
        client_data['amount'].append(bank[i].amount)
        client_data['curr_code'].append(bank[i].code)
    
    for i in range(len(bank_cipher)):
        client_data_ciph['id_num'].append(bank_cipher[i].indentificator_numder)
        client_data_ciph['account'].append(bank_cipher[i].accounty)
        client_data_ciph['amount'].append(bank_cipher[i].amount)
        client_data_ciph['curr_code'].append(bank_cipher[i].code)

    for i in range(len(decipher)):
        client_data_decipher['id_num'].append(decipher[i].indentificator_numder)
        client_data_decipher['account'].append(decipher[i].accounty)
        client_data_decipher['amount'].append(decipher[i].amount)
        client_data_decipher['curr_code'].append(decipher[i].code)

    for i in range(len(keys)):
        keys_o['id_num'].append(keys[i].ind_num)
        keys_o['key_1'].append(keys[i].key_1)
        keys_o['key_2'].append(keys[i].key_2)
        keys_o['key_3'].append(keys[i].key_3)
    

    for i in range(len(val)):
        currencies['name'].append(val[i].name)
        currencies['letter_code'].append(val[i].letter_code)
        currencies['code'].append(val[i].curr_code)

    
    

    pd.DataFrame(client_data).to_csv('data/client_data.csv')
    pd.DataFrame(client_data_ciph).to_csv('data/client_data_ciph.csv')
    pd.DataFrame(client_data_decipher).to_csv('data/client_data_decipher.csv')
    pd.DataFrame(keys_o).to_csv('data/keys.csv')
    pd.DataFrame(currencies).to_csv('data/vals.csv')
    pd.DataFrame(totals).to_csv('data/totals.csv')