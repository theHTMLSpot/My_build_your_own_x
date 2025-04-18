package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

func lsh_loop() {
	scanner := bufio.NewScanner(os.Stdin)

	for {
		cwd, _ := lsh_cwd()
		fmt.Printf("%s %% ", cwd)

		if !scanner.Scan() {
			break
		}

		line := scanner.Text()
		args := strings.Fields(line) // This splits the input correctly into words

		if len(args) == 0 {
			continue
		}

		execute(args)
	}
}
