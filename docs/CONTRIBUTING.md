# Contributing to Sovereign Media Player

Thank you for contributing! This project uses an **Open-Core model** — the frontend UI is fully open-source (MIT), while the engine binary is a closed-source freeware library.

---

## What You Can Contribute

✅ **Frontend UI** (`frontend_ui/`) — Fully open. Build themes, UI components, playlist support, subtitle parsing, file managers, etc.

✅ **Documentation** (`docs/`) — Improve API docs, write tutorials, translate to other languages.

✅ **Build Scripts** — Improve cross-platform build automation.

❌ **Core Engine** (`core_engine/`) — Closed-source. PRs touching binaries or the header's internal definitions will not be merged.

---

## Getting Started

1. **Fork** the repository on GitHub.
2. **Clone** your fork locally.
3. Make changes to the `frontend_ui/` directory.
4. Run `python3 build_open_core.py` to build and test.
5. Open a **Pull Request** with a clear description of your changes.

---

## Code Style

- Follow Apple's [Swift API Design Guidelines](https://swift.org/documentation/api-design-guidelines/).
- All public Swift functions and classes must have documentation comments (`///`).
- No third-party Swift Package Manager dependencies — keep the build self-contained.

---

## Reporting Issues

Use [GitHub Issues](https://github.com/sovereign-player/sovereign-media-player/issues) to report bugs or suggest features. For security vulnerabilities, open a private GitHub Security Advisory.
