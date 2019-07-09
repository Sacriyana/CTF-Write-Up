// Package cryptofs provides a basic abstraction for file encryption/decryption
package main

import (
	"crypto/aes"
	"crypto/cipher"
	"io"
	"os"
)

// Type File wrap an os.FileInfo
type File struct {
	os.FileInfo
	Extension string // The file extension without dot
	Path      string // The absolute path of the file
}

// Decrypt the file content with AES-CTR with the given key
// sending then to dst
func (file *File) Decrypt(key string, dst io.Writer) error {
	// Open the encrypted file
	inFile, err := os.Open(file.Path)
	if err != nil {
		return err
	}
	defer inFile.Close()

	// Create a 128 bits cipher.Block for AES-256
	block, err := aes.NewCipher([]byte(key))
	if err != nil {
		return err
	}

	// Retrieve the iv from the encrypted file
	iv := make([]byte, aes.BlockSize)
	inFile.Read(iv)

	// Get a stream for encrypt/decrypt in counter mode (best performance I guess)
	stream := cipher.NewCTR(block, iv)

	// Open a stream to decrypt and write to dst
	reader := &cipher.StreamReader{S: stream, R: inFile}

	// Copy the input file to the dst, decrypting as we go.
	if _, err = io.Copy(dst, reader); err != nil {
		return err
	}

	return nil
}

func main(){
	key := "95511870061fb3a2899aa6b2dc9838aa"
	file := new(File)
	file.Path = "data"
	outFile, err := os.OpenFile("flag.docx", os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0600)
	if err != nil {
		err = nil
	}
	defer outFile.Close()
	file.Decrypt(key, outFile)
}
