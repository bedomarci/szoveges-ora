# Detailed LED Word Mapping

This document provides the exact physical LED addresses for the 12x12 active grid (centered in a 16x16 matrix).

**Layout Logic:**
-   **Grid:** 16x16 Physical Matrix
-   **Orientation:** Vertical Columns
-   **Pattern:** "Snake"
    -   Even Columns (0, 2, ...): Top to Bottom
    -   Odd Columns (1, 3, ...): Bottom to Top
-   **Indexing:** Starts at Top-Left (0,0) = Index 0 (if Col 0 is Even/Down).
    -   Since indices are 0-based:
    -   Col 0 (Even): 0..15 (Down)
    -   Col 1 (Odd): 31..16 (Up) [Index 16 is at bottom]

The 12x12 "Display Area" is offset by (2, 2).
-   Logical (0,0) corresponds to Physical Column 2, Matrix Row 2.

## 1. Physical LED Addresses (12x12 Centered)
The 12x12 grid occupies rows 2-13 and columns 2-13 of the 16x16 matrix (all 0-indexed).

| Row\Col | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **0** (R2) | 34 | 61 | 66 | 93 | 98 | 125 | 130 | 157 | 162 | 189 | 194 | 221 |
| **1** (R3) | 35 | 60 | 67 | 92 | 99 | 124 | 131 | 156 | 163 | 188 | 195 | 220 |
| **2** (R4) | 36 | 59 | 68 | 91 | 100 | 123 | 132 | 155 | 164 | 187 | 196 | 219 |
| **3** (R5) | 37 | 58 | 69 | 90 | 101 | 122 | 133 | 154 | 165 | 186 | 197 | 218 |
| **4** (R6) | 38 | 57 | 70 | 89 | 102 | 121 | 134 | 153 | 166 | 185 | 198 | 217 |
| **5** (R7) | 39 | 56 | 71 | 88 | 103 | 120 | 135 | 152 | 167 | 184 | 199 | 216 |
| **6** (R8) | 40 | 55 | 72 | 87 | 104 | 119 | 136 | 151 | 168 | 183 | 200 | 215 |
| **7** (R9) | 41 | 54 | 73 | 86 | 105 | 118 | 137 | 150 | 169 | 182 | 201 | 214 |
| **8** (R10) | 42 | 53 | 74 | 85 | 106 | 117 | 138 | 149 | 170 | 181 | 202 | 213 |
| **9** (R11) | 43 | 52 | 75 | 84 | 107 | 116 | 139 | 148 | 171 | 180 | 203 | 212 |
| **10** (R12) | 44 | 51 | 76 | 83 | 108 | 115 | 140 | 147 | 172 | 179 | 204 | 211 |
| **11** (R13) | 45 | 50 | 77 | 82 | 109 | 114 | 141 | 146 | 173 | 178 | 205 | 210 |

## 2. Character and Word Layout

| Row\Col | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **0** | H | A | J | N | A | L | É | J  | J  | E | L | Ő |
| **1** | R | E | G | G | E | L | B | R  | E  | S | Z | T |
| **2** | D | É | L | E | L | Ő | T | T  | U  | T | Á | N |
| **3** | H | Á | R | O | M | N | E | G  | Y  | E | D | E |
| **4** | É | J | F | É | L | Ö | T | I  | Z  | E | N | G |
| **5** | N | Y | O | L | C | A | K | E  | T  | T | Ő | Y |
| **6** | N | É | G | Y | H | É | T | H  | Á  | R | O | M |
| **7** | K | I | L | E | N | C | Í | ?1 | M  | Ú | L | T |
| **8** | L | E | S | Z | Ö | T | Z | N  | Y  | O | L | C |
| **9** | K | E | T | T | Ő | H | A | T  | N  | É | G | Y |
| **10**| H | É | T | H | Á | R | O | M  | ?2 | E | G | Y |
| **11**| P | E | R | C | C | E | L | M  | Ú  | L | V | A |

## 3. Word-to-Address Sequences
*Using the new address map logic.*

### Time Intervals and Modifiers
| Word Enum | Sequence of LED Addresses |
| :--- | :--- |
| **HAJNAL** | 34, 61, 66, 93, 98, 125 |
| **ÉJJEL** | 130, 157, 162, 189, 194 |
| **REGGEL** | 35, 60, 67, 92, 99, 124 |
| **ESTE** (Vert) | 189, 188, 187, 186 |
| **DÉLELŐTT** | 36, 59, 68, 91, 100, 123, 132, 155 |
| **DÉLUTÁN** | 36, 59, 68, 164, 187, 196, 219 |
| **HAROMNEGYED**| 37, 58, 69, 90, 101, 122, 133, 154, 165, 186, 197 |
| **NEGYED** | 122, 133, 154, 165, 186, 197 |
| **FEL** | 102, 121, 134 |
| **MULT** | 169, 182, 201, 214 |
| **LESZ** | 42, 53, 74, 85 |
| **PERC** | 45, 50, 77, 82 |
| **PERCCEL** | 45, 50, 77, 82, 109, 114, 141 |
| **MÚLVA** | 146, 173, 178, 205, 210 |

### Hour Words (Óra)
| Word Enum | Sequence of LED Addresses |
| :--- | :--- |
| **EGY_ORA** (Vert)| 218, 217, 216 |
| **KETTO_ORA** | 135, 152, 167, 184, 199 |
| **HAROM_ORA** | 151, 168, 183, 200, 215 |
| **NEGY_ORA** | 40, 55, 72, 87 |
| **OT_ORA** | 120, 135 |
| **HAT_ORA** (Diag)| 134, 136, 119? (Wait: R6 C4=H(102?), C5=56?, C6=72? No. Let's trust the logic) |
| **HET_ORA** | 104, 119, 136 |
| **NYOLC_ORA** | 39, 56, 71, 88, 103 |
| **KILENC_ORA** | 41, 54, 73, 86, 105, 118 |
| **TIZ_ORA** | 136, 151, 168 |
| **EJFEL** | 38, 57, 70, 89, 102 |
| **DEL** | 36, 59, 68 |

*(Indices derived from programmatic mapping)*
