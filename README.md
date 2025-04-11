# Star Trail Converter
This is the final project on ADIP(Advance Digital Image Processing) course

Team member : Daniel Lai, Morris Huang, Sophia Huang

# Abstract
Due to the Earth's rotation, the starry sky is constantly rising in the east and setting in the west. Star trails are created by using long exposure photography to capture the movement of stars. This project aims to generate star trail images through image processing techniques. By separating the foreground and background, identifying all the stars in the sky, and applying different operations to the stars, various types of star trails—such as circular, radial, and spiral—can be produced. Experiments will be conducted on different types of data, including dark foregrounds, bright foregrounds, and starry skies with auroras. The goal is to design a general and robust algorithm, integrated with a user-friendly UI and CUDA acceleration, to provide users with an easy-to-use and visually appealing experience.

# Results
<table>
  <tr>
    <td align="center" width="50%">
      <h4>Star Sky (Image)</h4>
      <img src="https://github.com/DWEIEEE/Star-Trail-Converter/blob/main/Dataset/light_foreground_1.jpg" width="100%">
    </td>
    <td align="center" width="50%">
      <h4>Star Trail (GIF)</h4>
      <img src="https://github.com/DWEIEEE/Star-Trail-Converter/blob/main/samples/Circular_light_foreground_1_20231220144232.gif?raw=true" width="100%">
    </td>
  </tr>
  <tr>
    <td align="center" width="50%">
      <h4>Star Sky (Image)</h4>
      <img src="https://github.com/DWEIEEE/Star-Trail-Converter/blob/main/Dataset/light_foreground_4.jpg" width="100%">
    </td>
    <td align="center" width="50%">
      <h4>Star Trail (GIF)</h4>
      <img src="https://github.com/DWEIEEE/Star-Trail-Converter/blob/main/samples/Spiral_light_foreground_4_20231220163431.gif?raw=true" width="100%">
    </td>
  </tr>
  <tr>
    <td align="center" width="50%">
      <h4>Star Trail (Image)</h4>
      <img src="https://github.com/DWEIEEE/Star-Trail-Converter/blob/main/Dataset/star_trail/star_trail_3.jpg" width="100%">
    </td>
    <td align="center" width="50%">
      <h4>Star Sky (Restored)</h4>
      <img src="https://github.com/DWEIEEE/Star-Trail-Converter/blob/main/samples/StarSky_star_trail_3_20231224144919.png?raw=true" width="100%">
    </td>
  </tr>
</table>

# Libraries
- C++
- OpenCV
- Qt
- CUDA
- 
# Expected running time
CPU : Intel Core i7-14700K, RAM : 64GB, GPU : Nvidia RTX3090 24GB

#### Star Trail Generation Time
| Filename            | Dimensions   | Circular (sec) | Radial (sec) | Spiral (sec) |
|---------------------|--------------|----------------|--------------|--------------|
| foreground_1        | 1280×830     | 0.339          | 0.332        | 0.409        |
| foreground_2        | 650×934      | 0.396          | 0.359        | 0.421        |
| foreground_3        | 4000×6000    | 10.774         | 10.767       | 11.49        |
| light-foreground_1  | 800×562      | 0.252          | 0.262        | 0.277        |
| light-foreground_2  | 4000×6000    | 4.847          | 4.752        | 5.628        |
| light-foreground_3  | 3883×5816    | 5.223          | 5.184        | 6.54         |
| light-foreground_4  | 1125×750     | 0.514          | 0.508        | 0.557        |
| light-foreground_5  | 3840×5760    | 12.842         | 12.651       | 12.785       |
| aurora_1            | 7952×5304    | 7.885          | 7.764        | 9.135        |
| aurora_2            | 3960×4950    | 5.152          | 5.054        | 5.348        |
| aurora_3            | 5000×3205    | 3.583          | 3.274        | 3.6          |
| aurora_4            | 1125×750     | 0.88           | 0.871        | 0.918        |
| aurora_5            | 2432×3238    | 1.558          | 1.55         | 1.801        |
| aurora_6            | 2848×3560    | 1.977          | 1.956        | 2.376        |

#### Starry Sky Restoration Time
| Filename         | Dimensions   | Restoration (sec) |
|------------------|--------------|--------------------|
| star_trail_1     | 1000×667     | 0.145              |
| star_trail_2     | 640×425      | 0.131              |
| star_trail_3     | 1367×2048    | 0.639              |

# Reference
[1]	Baygin, Mehmet, et al. "Machine vision based defect detection approach using image processing." 2017 international artificial intelligence and data processing symposium (IDAP). Ieee, 2017.

[2]	Zhang, Nan, Yun-shan Chen, and Jian-li Wang. "Image parallel processing based on GPU." 2010 2nd international conference on advanced computer control. Vol. 3. IEEE, 2010.
