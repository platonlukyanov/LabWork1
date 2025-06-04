# Performance Analysis Report

## Current Performance Results

### 1. Clockwise Rotation (CW)
#### Single Operation
- Average: 0.006202 seconds
- Minimum: 0.005676034 seconds
- Maximum: 0.007966312 seconds

#### Batch Processing (BATCH)
- Average: 0.459048 seconds
- Minimum: 0.436461455 seconds
- Maximum: 0.495611637 seconds

### 2. Anticlockwise Rotation (ACW)
#### Single Operation
- Average: 0.006799 seconds
- Minimum: 0.005792107 seconds
- Maximum: 0.022950281 seconds

#### Batch Processing (BATCH)
- Average: 0.563211 seconds
- Minimum: 0.483939252 seconds
- Maximum: 0.69851312 seconds

### 3. Gauss Filter (GAUSS)
#### Single Operation
- Average: 0.04964 seconds
- Minimum: 0.042546123 seconds
- Maximum: 0.090098986 seconds

#### Batch Processing (BATCH)
- Average: 4.239084 seconds
- Minimum: 4.158127572 seconds
- Maximum: 4.331118435 seconds

## Results with 4 Threads

### 1. Clockwise Rotation (CW)
#### Single Operation
- Average: 0.006695 seconds
- Minimum: 0.005547669 seconds
- Maximum: 0.02730774 seconds
- Improvement: -7.36% (average), +2.31% (minimum), -70.83% (maximum)

#### Batch Processing (BATCH)
- Average: 0.436421 seconds
- Minimum: 0.430735259 seconds
- Maximum: 0.446211839 seconds
- Improvement: +5.18% (average), +1.33% (minimum), +11.07% (maximum)

### 2. Anticlockwise Rotation (ACW)
#### Single Operation
- Average: 0.006566 seconds
- Minimum: 0.005683606 seconds
- Maximum: 0.012329075 seconds
- Improvement: +3.55% (average), +1.91% (minimum), +86.15% (maximum)

#### Batch Processing (BATCH)
- Average: 0.460237 seconds
- Minimum: 0.423494777 seconds
- Maximum: 0.503959485 seconds
- Improvement: +22.37% (average), +14.27% (minimum), +38.61% (maximum)

### 3. Gauss Filter (GAUSS)
#### Single Operation
- Average: 0.024281 seconds
- Minimum: 0.017321756 seconds
- Maximum: 0.51902581 seconds
- Improvement: +104.44% (average), +145.62% (minimum), -82.64% (maximum)

#### Batch Processing (BATCH)
- Average: 1.682054 seconds
- Minimum: 1.622973783 seconds
- Maximum: 1.840049792 seconds
- Improvement: +152.02% (average), +156.20% (minimum), +135.38% (maximum)

## Analysis

1. **Gauss Filter** showed the most significant performance improvement with multithreading:
   - Substantial speedup in batch processing (over 150%)
   - Significant improvement in single operations (over 100%)

2. **Anticlockwise Rotation**:
   - Moderate improvement in batch processing (about 22%)
   - Small improvement in single operations

3. **Clockwise Rotation**:
   - Improvement in batch processing (about 5%)
   - Slight performance decrease in single operations

Overall, multithreaded processing is most effective for the Gauss Filter, which is expected due to its computational complexity. For rotation operations, the benefits of multithreading are less significant, but still present.

Link to table with results: https://docs.google.com/spreadsheets/d/1esjR3BVrsozCWV8pH_HuAtmV8JFpt_5K_MOApWmhAzQ/edit?usp=sharing

# How to reproduce testing process?
```bash
chmod u+x ./exec_time_test.sh
make && ./exec_time_test.sh
```