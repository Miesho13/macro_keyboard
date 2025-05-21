package main

import (
	"macro_listener/internal/config"
	"macro_listener/internal/serialreader"
)

func main() {
	cfg := config.LoadConfig("configs/config.cfg")
	serialreader.Listen(cfg)
}

