#!/bin/bash
# Enhanced macOS log cleaner (use with caution)

echo "[*] Clearing unified logs..."
log erase --all || echo "[-] Failed to erase unified logs."

echo "[*] Deleting common log files..."
rm -f /var/log/asl/*.asl
rm -f /var/log/*.log
rm -rf /Library/Logs/*
rm -rf ~/Library/Logs/*

echo "[*] Flushing system caches..."
dscacheutil -flushcache
killall -HUP mDNSResponder 2>/dev/null

echo "[*] Clearing LaunchAgents and Daemon logs..."
rm -rf ~/Library/LaunchAgents/com.*.plist
rm -rf /Library/LaunchDaemons/com.*.plist
rm -rf /private/var/db/launchd.db/com.apple.launchd/overrides.plist

echo "[*] Done. Some actions may require sudo privileges."
