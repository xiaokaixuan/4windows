// SSH_Exec project main.go
package main

/*
import (
	"fmt"
	"io/ioutil"
	"log"
	"os"
	"strings"

	"github.com/melbahja/goph"
)

func main() {
	userName := os.Args[1]
	host := os.Args[2]
	password := os.Args[3]

	client, err := goph.NewUnknown(userName, host, goph.Password(password))
	if err != nil {
		log.Fatal(err)
	}

	// Defer closing the network connection.
	defer client.Close()

	// Execute your command.
	bytes, err := ioutil.ReadFile("commands")
	commands := string(bytes)
	commands = strings.ReplaceAll(commands, "\n", ";")
	out, err := client.Run(commands)

	if err != nil {
		log.Fatal(err)
	}

	// Get your output as []byte.
	fmt.Println(string(out))
}
*/
import (
	"fmt"
	"github.com/pytool/ssh"
	"io/ioutil"
	"os"
	"time"
)

func runBash(host string, port string, userName string, password string) {
	fmt.Println(host, "正在登录...")
	c, err := ssh.NewClient(host, port, userName, password)
	for i := 0; i < 2; i++ {
		if err == nil {
			break
		}
		time.Sleep(200 * time.Millisecond)
		c, err = ssh.NewClient(host, port, userName, password)
	}
	if err != nil {
		fmt.Println(host, "登录失败")
		os.Exit(1)
	} else {
		fmt.Println(host, "登录成功")
	}
	defer c.Close()

	bytes, err := ioutil.ReadFile("commands")
	if err != nil {
		fmt.Println("命令文件commands没找到")
		os.Exit(1)
	}
	fmt.Println(host, "正在执行...")
	err = c.Exec1(string(bytes))
	if err != nil {
		fmt.Println(host, "执行失败", err)
		os.Exit(1)
	}
	fmt.Println(host, "执行完成")
}

func main() {
	host := os.Args[1]
	port := os.Args[2]
	userName := os.Args[3]
	password := os.Args[4]
	runBash(host, port, userName, password)
}
