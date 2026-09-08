# contour-detection

C++ application that locates objects in a photo by matching their contours against a reference shape.

A pattern image is loaded first and its contour extracted. Every photo in `data/photos/` is then scanned for contours, and each candidate is compared against the pattern using OpenCV's `matchShapes`, which is based on Hu moment invariants. Because those moments are invariant to translation, scale and rotation, the same object is recognised regardless of where it sits in the frame or how it is turned.

## How it works

1. Load `data/pattern/pattern.jpg` and extract its contour.
2. Iterate over every `.jpg` in `data/photos/`.
3. Extract all contours from the photo.
4. Compare each one against the pattern with `matchShapes` (`CV_CONTOURS_MATCH_I1`).
5. Display the contours scoring below the match threshold, each in its own window.

The score is printed for every candidate, so the threshold — currently `0.05` — can be tuned by watching the output.

## Requirements

- **OpenCV 2.x.** The code uses pre-3.0 constants such as `CV_WINDOW_AUTOSIZE` and `CV_CONTOURS_MATCH_I1`.
- **Windows and Visual Studio.** Directory traversal uses `<io.h>` and `_findfirst`, which are MSVC-specific. The OpenCV parts themselves are portable.

Open `openCVContourDetection.sln`, point the project at your OpenCV include and lib directories, and build.

## Limitations

- Windows-only, because of the file-listing code.
- The match threshold is hard-coded.
- Source comments are in Polish.
