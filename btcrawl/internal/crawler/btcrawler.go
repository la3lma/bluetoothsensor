package crawler

import (
	"io"
	"net/http"
	"time"
)

func GetBluetoothReportStringFromScanner(url string) ([]byte, error) {

	client := http.Client{
		Timeout: 25 * time.Second, // TODO: Set to 45 or 20 for prod
	}

	resp, err := client.Get(url)
	if err != nil {
		return []byte{}, err
	}

	body, err := io.ReadAll(resp.Body)
	defer resp.Body.Close()
	if err != nil {
		return []byte{}, err
	}

	return body, err
}
