# FileEncryptor

A simple command-line tool for encrypting and decrypting files using the Caesar cipher.

## Features

- Encrypt or decrypt a file using a user-defined shift value.
- Progress reporting during file processing.
- Error handling for file existence and input validation.
- User-friendly command-line interface.

## Installation

1. Ensure you have a C++ compiler installed (e.g., GCC, Clang, or MSVC).
2. Clone this repository or download the source code.
3. Compile the program using the following command:
   ```bash
   g++ FileEncryptor.cpp -o FileEncryptor
   ```

## Usage

Run the program using:
```bash
./FileEncryptor
```

Follow the prompts to:
- Choose the operation (1 for encryption, 2 for decryption).
- Enter a Caesar cipher shift value (1-25).
- Provide the input file path (the file to be processed).
- Provide the output file path (the new file to be created).

**Note:** The output file must not already exist. If it does, the program will display an error.

## Example

To encrypt a file named `test.txt` with a shift value of 3 and save the result as `encrypted.txt`, run the program and enter:
- Operation: 1
- Shift value: 3
- Input file path: test.txt
- Output file path: encrypted.txt

## License

This project is open-source and available under the MIT License.

## How It Works

The program uses the Caesar cipher algorithm:
- Each letter is shifted by the specified number of positions in the alphabet
- Non-alphabetic characters remain unchanged
- Case sensitivity is preserved
- The shift value wraps around the alphabet (e.g., 'z' shifted by 1 becomes 'a')

## Security Note

This is a basic encryption tool using the Caesar cipher. While it provides basic text obfuscation, it should not be used for securing sensitive information. For real security needs, use established encryption libraries and algorithms.

## Project Structure

- `FileEncryptor.cpp` - Main source code file
- Input/output files are created by the user during runtime

## Author

- GitHub: [@Presktok](https://github.com/Presktok)

## Future Improvements

Potential enhancements for future versions:
- Add support for different encryption algorithms
- Implement file integrity checking
- Add password protection
- Support for binary files
- Add batch processing capability
- Implement a graphical user interface 