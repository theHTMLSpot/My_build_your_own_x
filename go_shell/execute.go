package main

import (
	"fmt"
	"os"
	"strconv"
)

var buildin = []string{
	"cd",
	"help",
	"exit",
	"ls",
	"mkdir",
	"touch",
	"rm",
	"pwd",
}

var buildinfuncs = []func(args []string) int{
	cd,
	help,
	exit,
	ls,
	mkdir,
	touch,
	rm,
	pwd,
}

func cd(args []string) int {
	if len(args) < 2 {
		fmt.Println("cd: missing argument")
		return 1
	}

	err := os.Chdir(args[1])
	if err != nil {
		fmt.Printf("cd: %s\n", err)
		return 1
	}
	return 0
}

func exit(args []string) int {
	return 2
}

func ls(args []string) int {
	var dir string
	var err error
	if len(args) > 2 {
		dir = args[2]
	} else {
		dir, err = os.Getwd()
		if err != nil {
			fmt.Println("Error getting current directory:", err)
			return 1
		}
	}

	files, err := os.ReadDir(dir)
	if err != nil {
		fmt.Println("Error reading directory:", err)
		return 1
	}
	for _, file := range files {
		fmt.Println(file)
	}
	return 0
}

func help(args []string) int {
	fmt.Println("Ethan Lagdens go_shell")
	fmt.Println("Here are the functions i have")
	for _, word := range buildin {
		fmt.Println(word)
	}
	return 0
}
func mkdir(args []string) int {
	if len(args) < 2 {
		return 1 // Error: insufficient arguments
	}
	mode, err := os.FileMode(0755), error(nil)
	if len(args) > 2 {
		mode, err = os.FileMode(0), nil
		if parsedMode, parseErr := strconv.ParseUint(args[2], 8, 32); parseErr == nil {
			mode = os.FileMode(parsedMode)
		} else {
			err = parseErr
		}
	}
	if err != nil {
		fmt.Println("Invalid file mode:", err)
		return 1
	}
	if err := os.Mkdir(args[1], mode); err != nil {
		fmt.Println("Error creating directory:", err)
		return 1
	}
	return 0
}
func touch(args []string) int {
	if _, err := os.Create(args[1]); err != nil {
		fmt.Println("Error creating file:", err)
		return 1
	}

	return 0
}
func rm(args []string) int {
	if len(args) > 2 {
		if args[1] == "-rf" || args[1] == "-r" {
			err := os.RemoveAll(args[2])
			if err != nil {
				fmt.Printf("Error deleting dir %s %s \n", args[2], err)
				return 1
			}
			return 0
		} else {
			fmt.Printf("Unknown flag %s", args[2])
			return 1
		}
	}

	err := os.Remove(args[1])
	if err != nil {
		fmt.Printf("Error deleting dir %s %s \n", args[1], err)
		return 1
	}

	return 0
}

func pwd(args []string) int {
	path, err := os.Getwd()
	if err != nil {
		fmt.Printf("unexpected error: %s\n", err)
		return 1
	}
	fmt.Println(path)
	return 0
}

func execute(args []string) int {
	if len(args) == 0 {
		return 1
	}
	for i, builtin := range buildin {
		if args[0] == builtin {
			return buildinfuncs[i](args)
		}
	}
	fmt.Printf("Command not found: %s\n", args[0])
	return 1
}
