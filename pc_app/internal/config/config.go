package config

import (
	"gopkg.in/ini.v1"
	"log"
)

type SerialConfig struct {
	Port string
	Baud int
}

func LoadConfig(path string) *SerialConfig {
	cfg, err := ini.Load(path)
	if err != nil {
		log.Fatalf("Failed to read config file: %v", err)
	}

	port := cfg.Section("serial").Key("port").String()
	baud, err := cfg.Section("serial").Key("baud").Int()
	if err != nil {
		log.Fatalf("Invalid baud rate in config: %v", err)
	}

	return &SerialConfig{
		Port: port,
		Baud: baud,
	}
}

