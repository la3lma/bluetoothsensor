package main

import (
	"btcrawl/internal/crawler"
	"btcrawl/internal/persistence"
	_ "embed"
	"fmt"
	_ "github.com/mattn/go-sqlite3"
	"time"
)

/// This bit of magic will read a static file, which is quite wonderful, really
//go:embed schema.sql
var sqlSchema string

func main() {

	dbx, err := persistence.ReuseIfExistsOrCreateAndInitializeFromSqlString("production-database.db", sqlSchema)
	if err != nil {
		fmt.Println("Couldn' open databasefile", err)
		return
	}
	db := persistence.NewDatabase(dbx)

	for {
		crawler.ProbeMdnsThenScanForBluetoothReports(db)
		time.Sleep(3 * time.Second)
	}
}
