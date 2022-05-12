
## TODO

* Rewrite tests so that they work with the new location of the schemafile
* Fix naming inconsistencies between Go and SQL code
* Make the mdns code work in conjunction with the crawler code
* Make a goroutine to write to the database, and connect it to the crawler code.
* Fix gripes from goland
* Make static build that includes the sql file so that this thing
  can go from zero to sixty just from an executable
* Add logging functionality
* Add rudimentary command line parsing, with a verbose mode
* Make it run in docker (maybe)
* Make it run on raspberry pi (cross compilation, network config)
* Make it onto a system that just runs continously with very little maintenance.
* Refactor persistence code to be split into multiple files
* Refactor persistence code to be less repetitive
* Put the persisted database files somewhere more correct than in the source tree.

## Resources

* Project template: https://github.com/golang-standards/project-layout
* Standard SQL interface https://pkg.go.dev/database/sql
* Some documentation https://astaxie.gitbooks.io/build-web-application-with-golang/content/en/05.3.html
* Transactions https://www.sohamkamani.com/golang/sql-transactions/
* The conforming driver: https://github.com/mattn/go-sqlite3
* https://stackoverflow.com/questions/17796043/how-to-embed-files-into-go-binaries
