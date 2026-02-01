import ctypes
from tkinter import *
from tkinter import filedialog, messagebox


class Client(ctypes.Structure):
    _fields_= [
        ("client_name", ctypes.c_char_p),
        ("ssn", ctypes.c_char_p),
        ("bank_account_number", ctypes.c_char_p),
        ("balance", ctypes.c_double)
        ]
    
class Bank(ctypes.Structure):
    _fields_ = [
        ("num_clients", ctypes.c_int),
        ("clients", ctypes.POINTER(Client))
    ]
    
class BankApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Cash Crop Credit Union")
        self.bank = None

        self.bank_lib = ctypes.CDLL(r"C:\Users\ashar\OneDrive\Documents\GitHub\bank-management-app-cash-flow\build\bank.dll")

        self.setup_library_functions()

        self.setup_gui()


    def setup_library_functions(self):
        self.bank_lib.Bank_load_clients_info.argtypes = [ctypes.POINTER(Bank), ctypes.c_char_p]
        self.bank_lib.Bank_deposit.argtypes = [ctypes.POINTER(Bank), ctypes.c_char_p, ctypes.c_double]
        self.bank_lib.Bank_withdraw.argtypes = [ctypes.POINTER(Bank), ctypes.c_char_p, ctypes.c_double]
        self.bank_lib.Bank_free_string.argtypes = [ctypes.c_char_p]
        self.bank_lib.Bank_find_client.argtypes = [ctypes.POINTER(Bank), ctypes.c_char_p]
        self.bank_lib.Bank_add_new_client.argtypes = [ctypes.POINTER(Bank), ctypes.POINTER(Client)]
        self.bank_lib.Bank_get_num_clients.argtypes = [ctypes.POINTER(Bank)]
        self.bank_lib.Bank_get_num_clients.restype = ctypes.c_int
        self.bank_lib.Bank_find_client.restype = ctypes.POINTER(Client)

    def setup_gui(self):
        Label(self.root, text="Bank Account Number:").grid(row=0, column=0, padx=10, pady=5)
        self.account_entry = Entry(self.root)
        self.account_entry.grid(row=0, column=1, padx=10, pady=5)

        Label(self.root, text="Amount:").grid(row=1, column=0, padx=10, pady=5)
        self.amount_entry = Entry(self.root)
        self.amount_entry.grid(row=1, column=1, padx=10, pady=5)

        Button(self.root, text="Load Clients", command=self.load_clients).grid(row=2, column=0, columnspan=2, pady=10)
        Button(self.root, text="Deposit", command=self.deposit).grid(row=3, column=0, pady=5)
        Button(self.root, text="Withdraw", command=self.withdraw).grid(row=3, column=1, pady=5)
        Button(self.root, text="Show Client Info", command=self.show_client_info).grid(row=4, column=0, columnspan=2, pady=10)
        Button(self.root, text="Add New Client", command=self.add_client).grid(row=5, column=0, columnspan=2, pady=5)


    def load_clients(self):
        """Load clients from a selected file."""
        file_path = filedialog.askopenfilename(title="Select Client Info File")
        if file_path:
            self.bank = Bank()
            self.bank_lib.Bank_load_clients_info(ctypes.byref(self.bank), file_path.encode())
            messagebox.showinfo("Success", "Clients loaded successfully!")


    def deposit(self):
        """Deposit money to the specified account"""
        account = self.account_entry.get()
        try:
            amount = float(self.amount_entry.get())
            message = ctypes.create_string_buffer(256)
            success = self.bank_lib.Bank_deposit(self.bank, account.encode(), amount)
            if success:
                messagebox.showinfo("Success", "Deposit successful!")
            else:
                messagebox.showerror("Error", "Account not found!")
        except ValueError:
            messagebox.showerror("Error", "Invalid amount.")

    def withdraw(self):
        """Withdraw money from the specified account"""
        account = self.account_entry.get()
        try:
            amount = float(self.amount_entry.get())
            message = ctypes.create_string_buffer(256)
            success = self.bank_lib.Bank_withdraw(self.bank, account.encode(), amount)
            if success:
                messagebox.showinfo("Success", "Withdrawl successful!")
            else:
                messagebox.showerror("Error", "Insufficient funds or account not found")
        except ValueError:
            messagebox.showerror("Error", "Invalid amount.")


    
    def show_client_info(self):
        """Display the client information in a message box"""
        account = self.account_entry.get()
        client_ptr = self.bank_lib.Bank_find_client(self.bank, account.encode())
        if client_ptr:
            client = client_ptr.contents
            info = f"Name: {client.client_name.decode()}\n" \
                   f"SSN: {client.ssn.decode()}\n" \
                   f"Account: {client.bank_account_number.decode()}\n" \
                   f"Balance: {client.balance:.2f}"
            messagebox.showinfo("Client Info", info)
        else:
            messagebox.showerror("Error", "Client not found,")

    def add_client(self):
        """Add a new client to the system."""
        new_window = Toplevel(self.root)
        new_window.title("Add New Client")

        Label(new_window, text="Name:").grid(row=0, column=0, padx=10, pady=5)
        name_entry = Entry(new_window)
        name_entry.grid(row=0, column=1, padx=10, pady=5)

        Label(new_window, text="SSN:").grid(row=1, column=0, padx=10, pady=5)
        ssn_entry = Entry(new_window)
        ssn_entry.grid(row=1, column=1, padx=10, pady=5)

        Label(new_window, text="Account Number:").grid(row=2, column=0, padx=10, pady=5)
        account_entry = Entry(new_window)
        account_entry.grid(row=2, column=1, padx=10, pady=5)

        Label(new_window, text="Initial Balance:").grid(row=3, column=0, padx=10, pady=5)
        balance_entry = Entry(new_window)
        balance_entry.grid(row=3, column=1, padx=10, pady=5)

        def submit_new_client():
            try:
                client = Client(
                    client_name=name_entry.get().encode(),
                    ssn=ssn_entry.get().encode(),
                    bank_account_number=account_entry.get().encode(),
                    balance=float(balance_entry.get())
                )
                self.bank_lib.Bank_add_new_client(self.bank, ctypes.byref(client))
                messagebox.showinfo("Success", "New client added successfully!")
                new_window.destroy()
            except ValueError:
                messagebox.showerror("Error", "Invalid input. Please try again.")

        Button(new_window, text="Submit", command=submit_new_client).grid(row=4, column=0, columnspan=2, pady=10)


if __name__ == "__main__":
    root = Tk()
    app = BankApp(root)
    root.mainloop()
