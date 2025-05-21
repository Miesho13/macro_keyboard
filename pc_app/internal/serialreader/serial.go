package serialreader

import (
	"bufio"
	"fmt"
	"log"

	"github.com/tarm/serial"
	"macro_listener/internal/config"
)

func Listen(cfg *config.SerialConfig) {
	c := &serial.Config{Name: cfg.Port, Baud: cfg.Baud}
	s, err := serial.OpenPort(c)
	if err != nil {
		log.Fatalf("Failed to open port %s: %v", cfg.Port, err)
	}
	defer s.Close()

	fmt.Printf("Listening on port %s at %d baud...\n", cfg.Port, cfg.Baud)

	scanner := bufio.NewScanner(s)
	for scanner.Scan() {
		fmt.Println("Received:", scanner.Text())
	}

	if err := scanner.Err(); err != nil {
		log.Fatalf("Error reading from serial port: %v", err)
	}
}

