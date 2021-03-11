// qmail_upload project main.go
package main

import (
	"crypto/sha1"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
	"path/filepath"
	"regexp"
	"strings"

	"github.com/fatih/color"
)

type App struct {
	SID    string
	COOKIE string
}

func (this *App) UploadFile(fileName string) {
	file, err := os.Open(fileName)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return

	}
	defer file.Close()
	hash := sha1.New()
	fileSize, err := io.Copy(hash, file)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	fileName = filepath.Base(fileName)
	strName := url.QueryEscape(fileName)
	strSHA := fmt.Sprintf("%X", hash.Sum(nil))
	strUrl := fmt.Sprintf("https://mail.qq.com/cgi-bin/ftnCreatefile?uin=&ef=js&resp_charset=UTF8&s=ftnCreate&sid=%s&dirid=&path=%s&size=%d&md5=&sha=%s&sha3=&appid=2&loc=ftnCreatefile,ftnCreatefile,ftnCreate,ftn2",
		this.SID, strName, fileSize, strSHA)

	req, err := http.NewRequest("GET", strUrl, nil)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36 Edg/80.0.361.62")
	req.Header.Set("Cookie", this.COOKIE)
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	defer resp.Body.Close()
	bytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
	fmt.Printf("File: %s\n", fileName)
	fmt.Printf("Size: %d\n", fileSize)
	fmt.Printf("SHA1: %s\n", strSHA)
	fmt.Printf("Result: %s\n", string(bytes))
	this.CheckDelete(string(bytes))
}

func (this *App) UploadMeta(fileName, fileSize, strSHA string) {
	fileName = filepath.Base(fileName)
	strName := url.QueryEscape(fileName)
	strUrl := fmt.Sprintf("https://mail.qq.com/cgi-bin/ftnCreatefile?uin=&ef=js&resp_charset=UTF8&s=ftnCreate&sid=%s&dirid=&path=%s&size=%s&md5=&sha=%s&sha3=&appid=2&loc=ftnCreatefile,ftnCreatefile,ftnCreate,ftn2",
		this.SID, strName, fileSize, strSHA)
	req, err := http.NewRequest("GET", strUrl, nil)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36 Edg/80.0.361.62")
	req.Header.Set("Cookie", this.COOKIE)
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	defer resp.Body.Close()
	bytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
	fmt.Printf("File: %s\n", fileName)
	fmt.Printf("Size: %s\n", fileSize)
	fmt.Printf("SHA1: %s\n", strSHA)
	fmt.Printf("Result: %s\n", string(bytes))
	this.CheckDelete(string(bytes))
}

func (this *App) CheckDelete(result_str string) {
	str_pos := strings.Index(result_str, `exist : "1"`)
	if str_pos != -1 {
		return
	}
	reg := regexp.MustCompile(`sFileId\s*:\s*"(.+?)"`)
	strs := reg.FindStringSubmatch(result_str)
	if len(strs) < 2 {
		return
	}
	color.HiRed("Faild: cancel...\n")
	strUrl := fmt.Sprintf("https://mail.qq.com/cgi-bin/ftnDelFile?sid=%s&t=ftn.json&s=oper&ef=js", this.SID)
	param := strings.NewReader(`&fid=` + strs[1])
	req, err := http.NewRequest("POST", strUrl, param)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	req.Header.Set("User-Agent", "Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/80.0.3987.122 Safari/537.36 Edg/80.0.361.62")
	req.Header.Set("Cookie", this.COOKIE)
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return
	}
	defer resp.Body.Close()
	bytes, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
	fmt.Printf("Result: %s\n\n", string(bytes))
}

func NewApp() *App {
	return &App{SID: os.Getenv("SID"), COOKIE: os.Getenv("COOKIE")}
}

func showHelp() {
	fmt.Printf("\nUsage:\n\n./this_program file <filepath>\n./this_program meta <filename> <filesize> <filesha1>\n")
}

func main() {
	if len(os.Args) < 3 {
		showHelp()
		return
	}
	pApp := NewApp()
	if os.Args[1] == "file" {
		for i, fileName := range os.Args {
			if i <= 1 {
				continue
			}
			pApp.UploadFile(fileName)
		}
	} else if os.Args[1] == "meta" {
		if len(os.Args) < 5 {
			showHelp()
			return
		}
		pApp.UploadMeta(os.Args[2], os.Args[3], os.Args[4])
	}
}
