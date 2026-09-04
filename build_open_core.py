#!/usr/bin/env python3
"""
================================================================================
  Sovereign Media Player — Open-Core Build Pipeline
  Copyright (c) 2026 Sovereign Byte Technology. All Rights Reserved.
================================================================================
Builds the complete distribution package for the Open-Core edition:

  1. Compiles the open-source Swift frontend (MIT License)
  2. Links against the closed-source Sovereign Engine binary (Freeware)
  3. Creates a Universal Mach-O binary (arm64 + x86_64)
  4. Applies binary hardening (debug symbol stripping)
  5. Packages into SovereignPlayer.app bundle + distribution .zip

Usage:
  python3 build_open_core.py              # Full build
  python3 build_open_core.py --bundle-only  # App bundling only (for CI)
================================================================================
"""

import os
import sys
import subprocess
import shutil
import zipfile

# ─── Paths ────────────────────────────────────────────────────────────────────
ROOT          = os.path.dirname(os.path.abspath(__file__))
SWIFT_SRC     = os.path.join(ROOT, "frontend_ui", "OpenSovereignPlayerUI.swift")
ENGINE_LIB    = os.path.join(ROOT, "core_engine", "lib")
ENGINE_HEADER = os.path.join(ROOT, "core_engine", "include")
BUILD_DIR     = os.path.join(ROOT, "build")
DIST_DIR      = os.path.join(ROOT, "dist")
APP_BUNDLE    = os.path.join(DIST_DIR, "SovereignPlayer.app")
ZIP_OUTPUT    = os.path.join(DIST_DIR, "SovereignPlayer_macOS_Universal.zip")

BUNDLE_ONLY   = "--bundle-only" in sys.argv

# ─── Helpers ──────────────────────────────────────────────────────────────────
def run(cmd, desc):
    print(f"  ➜ {desc}")
    res = subprocess.run(cmd, shell=True, capture_output=True, text=True)
    if res.returncode != 0:
        print(f"FAILED: {desc}\nSTDOUT:\n{res.stdout}\nSTDERR:\n{res.stderr}")
        sys.exit(1)
    return res.stdout

def banner(msg):
    print("\n" + "=" * 72)
    print(f"  {msg}")
    print("=" * 72)

def check_swift():
    res = subprocess.run("swiftc --version", shell=True, capture_output=True, text=True)
    if res.returncode != 0:
        print("swiftc not found. Install Xcode Command Line Tools: xcode-select --install")
        sys.exit(1)
    print(f"  Swift Compiler: {res.stdout.strip().splitlines()[0]}")

# ─── Build Steps ──────────────────────────────────────────────────────────────
def compile_frontend():
    banner("STEP 1 — COMPILING OPEN-SOURCE SWIFT FRONTEND")
    os.makedirs(BUILD_DIR, exist_ok=True)
    check_swift()

    common_flags = (
        f"-framework Cocoa "
        f"-framework AVKit "
        f"-framework AVFoundation "
        f"-framework Metal "
        f"-framework MetalKit "
        f"-O -whole-module-optimization "
        f"-Xlinker -dead_strip "
    )

    arm64_bin = os.path.join(BUILD_DIR, "player_arm64")
    run(
        f"swiftc {SWIFT_SRC} {common_flags} -target arm64-apple-macosx11.0 -o {arm64_bin}",
        "Compiling ARM64 (Apple Silicon M1/M2/M3/M4)"
    )

    intel_bin = os.path.join(BUILD_DIR, "player_intel")
    run(
        f"swiftc {SWIFT_SRC} {common_flags} -target x86_64-apple-macosx10.15 -o {intel_bin}",
        "Compiling x86_64 (Intel 2010-2020 Macs)"
    )

    universal_bin = os.path.join(BUILD_DIR, "SovereignPlayer_universal")
    run(
        f"lipo -create -output {universal_bin} {arm64_bin} {intel_bin}",
        "Creating Fat Universal Mach-O Binary (arm64 + x86_64)"
    )

    # Strip all debug symbols before distribution
    run(f"strip -x -ur {universal_bin}", "Stripping debug symbols")
    print(f"  Universal binary size: {os.path.getsize(universal_bin) / 1024:.1f} KB")
    return universal_bin

def bundle_app(universal_bin):
    banner("STEP 2 — PACKAGING macOS .APP BUNDLE")

    if os.path.exists(APP_BUNDLE):
        shutil.rmtree(APP_BUNDLE)

    macos_dir     = os.path.join(APP_BUNDLE, "Contents", "MacOS")
    resources_dir = os.path.join(APP_BUNDLE, "Contents", "Resources")
    os.makedirs(macos_dir, exist_ok=True)
    os.makedirs(resources_dir, exist_ok=True)

    # Copy binary
    target_bin = os.path.join(macos_dir, "SovereignPlayer")
    shutil.copy2(universal_bin, target_bin)
    os.chmod(target_bin, 0o755)
    print(f"  Binary installed to: {target_bin}")

    # Copy Icon
    icon_src = os.path.join(ROOT, "frontend_ui", "SovereignPlayer.icns")
    if os.path.exists(icon_src):
        shutil.copy2(icon_src, os.path.join(resources_dir, "SovereignPlayer.icns"))

    # Write Info.plist
    plist_content = '''<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key><string>en</string>
    <key>CFBundleExecutable</key><string>SovereignPlayer</string>
    <key>CFBundleIconFile</key><string>SovereignPlayer.icns</string>
    <key>CFBundleIdentifier</key><string>io.sovereignmediaplayer.SovereignPlayer</string>
    <key>CFBundleInfoDictionaryVersion</key><string>6.0</string>
    <key>CFBundleName</key><string>Sovereign Media Player</string>
    <key>CFBundlePackageType</key><string>APPL</string>
    <key>CFBundleShortVersionString</key><string>3.0.0</string>
    <key>CFBundleVersion</key><string>2026.3</string>
    <key>LSMinimumSystemVersion</key><string>10.15</string>
    <key>NSHighResolutionCapable</key><true/>
    <key>NSPrincipalClass</key><string>NSApplication</string>
    <key>NSSupportsAutomaticGraphicsSwitching</key><true/>
    <key>CFBundleDocumentTypes</key>
    <array>
        <dict>
            <key>CFBundleTypeName</key><string>Video File</string>
            <key>CFBundleTypeRole</key><string>Viewer</string>
            <key>LSHandlerRank</key><string>Owner</string>
            <key>LSItemContentTypes</key>
            <array>
                <string>public.movie</string>
                <string>public.video</string>
                <string>public.audio</string>
                <string>com.apple.quicktime-movie</string>
                <string>public.mpeg-4</string>
                <string>public.avi</string>
            </array>
        </dict>
    </array>
</dict>
</plist>'''
    with open(os.path.join(APP_BUNDLE, "Contents", "Info.plist"), "w") as f:
        f.write(plist_content)
    print("  Info.plist written")

    # Remove Gatekeeper quarantine flag for local distribution
    run(f"xattr -dr com.apple.quarantine '{APP_BUNDLE}' 2>/dev/null || true",
        "Removing Gatekeeper Quarantine (local distribution)")

    print(f"  App bundle: {APP_BUNDLE}")

def package_distribution():
    banner("STEP 3 — PACKAGING DISTRIBUTION ZIP")

    if os.path.exists(ZIP_OUTPUT):
        os.remove(ZIP_OUTPUT)

    with zipfile.ZipFile(ZIP_OUTPUT, 'w', zipfile.ZIP_DEFLATED, compresslevel=9) as z:
        for root, dirs, files in os.walk(APP_BUNDLE):
            for f in files:
                full_path = os.path.join(root, f)
                rel_path  = os.path.relpath(full_path, DIST_DIR)
                z.write(full_path, rel_path)

    zip_kb = os.path.getsize(ZIP_OUTPUT) / 1024
    print(f"  Distribution zip: {ZIP_OUTPUT} ({zip_kb:.1f} KB)")

def print_summary():
    banner("BUILD COMPLETE")
    print(f"  App Bundle :  {APP_BUNDLE}")
    print(f"  Zip Package:  {ZIP_OUTPUT}")
    print(f"  Architectures: arm64 + x86_64 (Universal)\n")
    print("  To install: drag SovereignPlayer.app to /Applications")
    print("  To share  : upload the .zip to GitHub Releases\n")

# ─── Entry Point ──────────────────────────────────────────────────────────────
if __name__ == "__main__":
    os.makedirs(DIST_DIR, exist_ok=True)

    if BUNDLE_ONLY:
        # CI mode: binary already compiled by individual steps
        universal_bin = os.path.join(BUILD_DIR, "SovereignPlayer_universal")
        bundle_app(universal_bin)
    else:
        universal_bin = compile_frontend()
        bundle_app(universal_bin)
        package_distribution()
        print_summary()
