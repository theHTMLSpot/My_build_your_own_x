package main

import (
	"fmt"
	"os"
	"strings"
)

func lsh_cwd() (string, error) {
	const maxSegments = 5

	cwd, err := os.Getwd()
	if err != nil {
		fmt.Println("Error:", err)
		return "", err
	}

	home, err := os.UserHomeDir()
	if err == nil && strings.HasPrefix(cwd, home) {
		cwd = strings.Replace(cwd, home, "~", 1)
	}

	cwdArr := strings.Split(cwd, "/")

	if len(cwdArr) > maxSegments {
		start := cwdArr[:maxSegments/2]
		end := cwdArr[len(cwdArr)-(maxSegments/2):]
		shortened := append(start, "...")
		shortened = append(shortened, end...)
		return strings.Join(shortened, "/"), nil
	}

	return cwd, nil
}
