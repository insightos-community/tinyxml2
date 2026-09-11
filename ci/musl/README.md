# InsightOS tinyxml2 11.0.0 musl release

The `insightos/musl` branch maintains upstream `11.0.0` with a build/release pipeline. No library source patches are required.

## Assets and runtime

- `tinyxml2-11.0.0-musl-x86_64-prefix.tar.gz`: installed shared library, development headers/metadata, licenses, and manifest.
- `build-manifest.json` and `SHA256SUMS`.
- Build/test/ELF logs are uploaded as separate GitHub Actions artifacts.

This is a musl x86_64 installed prefix, not a Python wheel, fully static binary, or complete application runtime. Runtime requirements: musl x86_64, libstdc++.so.6, libgcc_s.so.1. Compiler runtime libraries are used in the clean-container test but are not included in the archive. This library has no Python ABI dependency.

Extract the archive and add its `prefix/lib` to the application's library search path, for example `LD_LIBRARY_PATH`. CI tests loading from a different installation path. CMake SDK metadata was built for `/work/prefix`; SDK relocation is not validated.

## CI and release

CI runs on a standard GitHub `ubuntu-24.04` runner inside a digest-pinned Python/Alpine musl container. Python is a build/test helper, not a dependency of the produced library.

The pipeline compiles shared libraries, runs the upstream xmltest suite through CTest, audits installed ELF files for GLIBC version requirements and unresolved dependencies, and tests the actual release archive after extraction at a new path in a clean, offline musl container. Release publication requires all checks to pass.

Pushes/PRs to `insightos/musl` and manual dispatches upload build artifacts. Push a new tag such as `musl-v11.0.0-1` to publish a GitHub Release. Tags must contain this workflow. Published releases cannot be overwritten by rerunning the workflow; use a new build tag. Upstream tags remain unchanged.

APK toolchain versions are recorded in the manifest/logs but transitive build dependencies are not all pinned; byte-for-byte reproducibility is not claimed. Full Pinocchio/Robot SDK/installer integration is outside this library CI.

## Local build

From a clean checkout, use a fresh output directory outside the source tree:

```sh
musl_work=$(mktemp -d)
docker run --rm --cpus=2 --memory=12g --memory-swap=12g \
  --mount "type=bind,src=$PWD,dst=/src,readonly" \
  --mount "type=bind,src=$musl_work,dst=/work" \
  python:3.13-alpine3.23@sha256:75f27d686432419c9d42420b2b9ef605868c7a0682a6be10a6601fad46c2df01 \
  sh /src/ci/musl/build.sh
```

Then run `ci/musl/clean.sh` in a new container of the same base image, with the same mounts and `--network none`.
