# Detailed LED Word Mapping

This document provides the exact physical LED addresses for the 12x12 active grid (centered in a 16x16 matrix) using the **OXPLOW (Boustrophedon)** layout.

## 1. Physical LED Addresses (12x12 Centered)
The 12x12 grid occupies rows 2-13 and columns 2-13 of the 16x16 matrix (all 0-indexed).
Below is the mapping of each cell in the 12x12 layout to its physical LED index.

| Row\Col | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 |
| :--- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **0** (R2) | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 |
| **1** (R3) | 61 | 60 | 59 | 58 | 57 | 56 | 55 | 54 | 53 | 52 | 51 | 50 |
| **2** (R4) | 66 | 67 | 68 | 69 | 70 | 71 | 72 | 73 | 74 | 75 | 76 | 77 |
| **3** (R5) | 93 | 92 | 91 | 90 | 89 | 88 | 87 | 86 | 85 | 84 | 83 | 82 |
| **4** (R6) | 98 | 99 | 100| 101| 102| 103| 104| 105| 106| 107| 108| 109|
| **5** (R7) | 125| 124| 123| 122| 121| 120| 119| 118| 117| 116| 115| 114|
| **6** (R8) | 130| 131| 132| 133| 134| 135| 136| 137| 138| 139| 140| 141|
| **7** (R9) | 157| 156| 155| 154| 153| 152| 151| 150| 149| 148| 147| 146|
| **8** (R10)|162| 163| 164| 165| 166| 167| 168| 169| 170| 171| 172| 173|
| **9** (R11)| 189| 188| 187| 186| 185| 184| 183| 182| 181| 180| 179| 178|
| **10**(R12)| 194| 195| 196| 197| 198| 199| 200| 201| 202| 203| 204| 205|
| **11**(R13)| 221| 220| 219| 218| 217| 216| 215| 214| 213| 212| 211| 210|

## 2. Character and Word Layout
The characters are mapped to the addresses above as follows:

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
Each word is mapped to a sequence of physical LED indices. This accounts for horizontal, vertical, and diagonal layouts.

### Time Intervals and Modifiers
| Word Enum | Sequence of LED Addresses |
| :--- | :--- |
| **HAJNAL** | 34, 35, 36, 37, 38, 39 |
| **ÉJJEL** | 40, 41, 42, 43, 44 |
| **REGGEL** | 61, 60, 59, 58, 57, 56 |
| **ESTE** (Vert) | 43, 52, 75, 84 |
| **DÉLELŐTT** | 66, 67, 68, 69, 70, 71, 72, 73 |
| **DÉLUTÁN** | 66, 67, 68, 74, 75, 76, 77 |
| **HAROMNEGYED**| 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83 |
| **NEGYED** | 88, 87, 86, 85, 84, 83 |
| **FEL** | 100, 101, 102 |
| **MULT** | 149, 148, 147, 146 |
| **LESZ** | 162, 163, 164, 165 |
| **PERC** | 221, 220, 219, 218 |
| **PERCCEL** | 221, 220, 219, 218, 217, 216, 215 |
| **MÚLVA** | 214, 213, 212, 211, 210 |
| **TIZEN** (Prefix)| 104, 105, 106, 107, 108 |

### Hour Words (Óra)
| Word Enum | Sequence of LED Addresses |
| :--- | :--- |
| **EGY_ORA** (Vert)| 82, 109, 114 |
| **KETTO_ORA** | 118, 117, 116, 115, 114 |
| **HAROM_ORA** | 137, 138, 139, 140, 141 |
| **NEGY_ORA** | 130, 131, 132, 133 |
| **OT_ORA** | 103, 104 |
| **HAT_ORA** (Diag)| 134, 120, 104 |
| **HET_ORA** | 134, 135, 136 |
| **NYOLC_ORA** | 125, 124, 123, 122, 121 |
| **KILENC_ORA** | 157, 156, 155, 154, 153, 152 |
| **TIZ_ORA** | 104, 105, 106 |
| **EJFEL** | 98, 99, 100, 101, 102 |
| **DEL** | 66, 67, 68 |

### Minute Words (Perc)
| Word Enum | Sequence of LED Addresses |
| :--- | :--- |
| **EGY_PERC** | 203, 204, 205 |
| **KETTO_PERC** | 189, 188, 187, 186, 185 |
| **HAROM_PERC** | 197, 198, 199, 200, 201 |
| **NEGY_PERC** | 181, 180, 179, 178 |
| **OT_PERC** | 166, 167 |
| **HAT_PERC** | 184, 183, 182 |
| **HET_PERC** | 194, 195, 196 |
| **NYOLC_PERC** | 169, 170, 171, 172, 173 |

> [!NOTE]
> - **Vertical and Diagonal Overlaps**: Some words share LEDs or span multiple rows (e.g., ESTE, HAT_ORA, EGY_ORA).
> - **Compound Words**: TIZENEGY = TIZEN + EGY_ORA. TIZENKETTŐ = TIZEN + KETTO_ORA.
