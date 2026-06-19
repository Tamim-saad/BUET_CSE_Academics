#include "bitmap_image.hpp"
#include <bits/stdc++.h>
using namespace std;

// Define infinity constant for edge cases
#define INFINITY_VAL numeric_limits<double>::infinity()

// Mathematical constant pi for calculations
double PI_CONSTANT = 2 * acos(0.0);

// Structure to represent a 3D coordinate point
struct Coordinate3D {
  double xPos, yPos, zPos;
};

// Structure for RGB color representation in rendering
struct RGBColor {
  double redComponent, greenComponent, blueComponent;
};

// Structure representing a triangle with three vertices and color
struct TriangleObject {
  Coordinate3D vertices[3];
  RGBColor triangleColor;
};

// ==================== UTILITY FUNCTIONS FOR TRIANGLE ANALYSIS
// ====================

/**
 * Finds the vertex with maximum Y coordinate in a triangle
 * Used for determining the top scanline during rasterization
 */
int locateMaxYVertex(TriangleObject triangleData) {
  if (triangleData.vertices[0].yPos > triangleData.vertices[1].yPos) {
    return (triangleData.vertices[0].yPos > triangleData.vertices[2].yPos) ? 0
                                                                           : 2;
  } else {
    return (triangleData.vertices[1].yPos > triangleData.vertices[2].yPos) ? 1
                                                                           : 2;
  }
}

/**
 * Finds the vertex with minimum Y coordinate in a triangle
 * Used for determining the bottom scanline during rasterization
 */
int locateMinYVertex(TriangleObject triangleData) {
  if (triangleData.vertices[0].yPos < triangleData.vertices[1].yPos) {
    return (triangleData.vertices[0].yPos < triangleData.vertices[2].yPos) ? 0
                                                                           : 2;
  } else {
    return (triangleData.vertices[1].yPos < triangleData.vertices[2].yPos) ? 1
                                                                           : 2;
  }
}

/**
 * Finds the vertex with maximum X coordinate in a triangle
 * Used for determining the rightmost boundary during clipping
 */
int locateMaxXVertex(TriangleObject triangleData) {
  if (triangleData.vertices[0].xPos > triangleData.vertices[1].xPos) {
    return (triangleData.vertices[0].xPos > triangleData.vertices[2].xPos) ? 0
                                                                           : 2;
  } else {
    return (triangleData.vertices[1].xPos > triangleData.vertices[2].xPos) ? 1
                                                                           : 2;
  }
}

/**
 * Finds the vertex with minimum X coordinate in a triangle
 * Used for determining the leftmost boundary during clipping
 */
int locateMinXVertex(TriangleObject triangleData) {
  if (triangleData.vertices[0].xPos < triangleData.vertices[1].xPos) {
    return (triangleData.vertices[0].xPos < triangleData.vertices[2].xPos) ? 0
                                                                           : 2;
  } else {
    return (triangleData.vertices[1].xPos < triangleData.vertices[2].xPos) ? 1
                                                                           : 2;
  }
}

// ==================== MATRIX AND TRANSFORMATION OPERATIONS
// ====================

/**
 * Performs matrix multiplication for 4x4 transformation matrices
 * Essential for all geometric transformations in graphics pipeline
 */
vector<vector<double>>
multiplyTransformationMatrices(vector<vector<double>> leftMatrix,
                               vector<vector<double>> rightMatrix) {
  vector<vector<double>> resultMatrix(4);
  for (int row = 0; row < 4; row++)
    resultMatrix[row].resize(4);

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      resultMatrix[row][col] = 0;
      for (int element = 0; element < 4; element++) {
        resultMatrix[row][col] +=
            leftMatrix[row][element] * rightMatrix[element][col];
      }
    }
  }
  return resultMatrix;
}

/**
 * Implements Rodrigues' rotation formula for arbitrary axis rotation
 * Used to rotate vectors around any given axis by specified angle
 */
Coordinate3D rodriguesRotationFormula(Coordinate3D vectorToRotate,
                                      Coordinate3D rotationAxis,
                                      double angleInDegrees) {
  double cosineAngle = cos((angleInDegrees * PI_CONSTANT) / 180);
  double sineAngle = sin((angleInDegrees * PI_CONSTANT) / 180);

  // First term: v * cos(θ)
  Coordinate3D firstTerm = {cosineAngle * vectorToRotate.xPos,
                            cosineAngle * vectorToRotate.yPos,
                            cosineAngle * vectorToRotate.zPos};

  // Second term: (1 - cos(θ)) * (v · a) * a
  double dotProduct =
      (1 - cosineAngle) * (vectorToRotate.xPos * rotationAxis.xPos +
                           vectorToRotate.yPos * rotationAxis.yPos +
                           vectorToRotate.zPos * rotationAxis.zPos);
  Coordinate3D secondTerm = {dotProduct * rotationAxis.xPos,
                             dotProduct * rotationAxis.yPos,
                             dotProduct * rotationAxis.zPos};

  // Third term: sin(θ) * (a × v)
  Coordinate3D crossProduct = {rotationAxis.yPos * vectorToRotate.zPos -
                                   rotationAxis.zPos * vectorToRotate.yPos,
                               rotationAxis.zPos * vectorToRotate.xPos -
                                   rotationAxis.xPos * vectorToRotate.zPos,
                               rotationAxis.xPos * vectorToRotate.yPos -
                                   rotationAxis.yPos * vectorToRotate.xPos};
  Coordinate3D thirdTerm = {crossProduct.xPos * sineAngle,
                            crossProduct.yPos * sineAngle,
                            crossProduct.zPos * sineAngle};

  // Final result: sum of all three terms
  return {firstTerm.xPos + secondTerm.xPos + thirdTerm.xPos,
          firstTerm.yPos + secondTerm.yPos + thirdTerm.yPos,
          firstTerm.zPos + secondTerm.zPos + thirdTerm.zPos};
}

/**
 * Outputs transformation results for debugging purposes
 * Displays 4x4 matrix with proper formatting
 */
void displayTransformationMatrix(vector<vector<double>> matrixData) {
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      cout << setprecision(7) << matrixData[row][col] << " ";
    }
    cout << endl;
  }
}

/**
 * Prints transformed triangle coordinates after perspective division
 * Converts homogeneous coordinates to Cartesian coordinates
 */
void outputTriangleCoordinates(vector<vector<double>> transformedPoints,
                               int matrixDimension) {
  for (int vertexIndex = 0; vertexIndex < matrixDimension; vertexIndex++) {
    for (int coordinateIndex = 0; coordinateIndex < matrixDimension;
         coordinateIndex++) {
      if (vertexIndex != 3 && coordinateIndex != 3)
        cout << fixed << setprecision(7)
             << transformedPoints[coordinateIndex][vertexIndex] /
                    transformedPoints[matrixDimension - 1][vertexIndex]
             << " ";
    }
    cout << endl;
  }
}

// ==================== MAIN GRAPHICS PIPELINE IMPLEMENTATION
// ====================

int main() {
  // ================== TEST CASE SELECTION ==================

  int testCaseNumber = 0;
  cout << "Enter test case number (1-4): ";
  cin >> testCaseNumber;

  string configPath, scenePath;
  if (testCaseNumber >= 1 && testCaseNumber <= 4) {
    configPath = "Test Cases/" + to_string(testCaseNumber) + "/config.txt";
    scenePath = "Test Cases/" + to_string(testCaseNumber) + "/scene.txt";
    cout << "Using test case " << testCaseNumber << endl;
  } else {
    cout << "Invalid test case number. Please enter 1-4." << endl;
    return 1;
  }

  // ================== STAGE 1: MODELING TRANSFORMATION ==================

  // Setup input/output files for modeling transformation stage
  freopen(scenePath.c_str(), "r", stdin);
  freopen("stage1.txt", "w", stdout);

  int matrixDimension = 4;
  int totalTriangleCount = 0;

  // Camera and projection parameters
  struct Coordinate3D cameraPosition, lookAtPoint, upVector;
  double fieldOfViewY, aspectRatioValue, nearPlaneDistance, farPlaneDistance;

  // Read camera configuration from input file
  cin >> cameraPosition.xPos >> cameraPosition.yPos >> cameraPosition.zPos;
  cin >> lookAtPoint.xPos >> lookAtPoint.yPos >> lookAtPoint.zPos;
  cin >> upVector.xPos >> upVector.yPos >> upVector.zPos;
  cin >> fieldOfViewY >> aspectRatioValue >> nearPlaneDistance >>
      farPlaneDistance;

  // Stack for transformation matrices and push/pop operations
  stack<vector<vector<double>>> transformationStack;
  stack<int> pushedTransformationCount;
  int currentTransformationCount = 0;
  int pushModeActive = 0;

  // Initialize identity matrix for transformation base
  vector<vector<double>> identityTransformation(matrixDimension);
  for (int row = 0; row < matrixDimension; row++)
    identityTransformation[row].resize(matrixDimension);

  for (int row = 0; row < matrixDimension; row++) {
    for (int col = 0; col < matrixDimension; col++) {
      identityTransformation[row][col] = (row == col) ? 1 : 0;
    }
  }

  transformationStack.push(identityTransformation);

  // Process modeling transformation commands
  while (true) {
    string commandType;
    cin >> commandType;

    if (commandType == "triangle") {
      // Handle triangle definition and transformation
      totalTriangleCount++;
      vector<vector<double>> triangleVertexMatrix(matrixDimension);
      for (int row = 0; row < matrixDimension; row++)
        triangleVertexMatrix[row].resize(matrixDimension);

      // Read triangle vertex coordinates
      cin >> triangleVertexMatrix[0][0] >> triangleVertexMatrix[1][0] >>
          triangleVertexMatrix[2][0];
      cin >> triangleVertexMatrix[0][1] >> triangleVertexMatrix[1][1] >>
          triangleVertexMatrix[2][1];
      cin >> triangleVertexMatrix[0][2] >> triangleVertexMatrix[1][2] >>
          triangleVertexMatrix[2][2];

      // Set homogeneous coordinates
      for (int row = 0; row < matrixDimension; row++) {
        for (int col = 0; col < matrixDimension; col++) {
          if (row == 3)
            triangleVertexMatrix[row][col] = 1;
          else if (col == 3)
            triangleVertexMatrix[row][col] = 0;
        }
      }

      // Apply current transformation and output result
      vector<vector<double>> transformedTriangle =
          multiplyTransformationMatrices(transformationStack.top(),
                                         triangleVertexMatrix);
      outputTriangleCoordinates(transformedTriangle, matrixDimension);
    }

    else if (commandType == "translate") {
      // Handle translation transformation
      vector<vector<double>> translationMatrix(matrixDimension);
      for (int row = 0; row < matrixDimension; row++)
        translationMatrix[row].resize(4);

      for (int row = 0; row < matrixDimension; row++) {
        for (int col = 0; col < matrixDimension; col++) {
          if (row == col)
            translationMatrix[row][col] = 1;
          else if (row < 3 && col == 3)
            cin >> translationMatrix[row][col];
          else
            translationMatrix[row][col] = 0;
        }
      }
      transformationStack.push(multiplyTransformationMatrices(
          transformationStack.top(), translationMatrix));
      if (pushModeActive == 1)
        currentTransformationCount++;
    }

    else if (commandType == "scale") {
      // Handle scaling transformation
      vector<vector<double>> scalingMatrix(matrixDimension);
      for (int row = 0; row < matrixDimension; row++)
        scalingMatrix[row].resize(4);

      for (int row = 0; row < matrixDimension; row++) {
        for (int col = 0; col < matrixDimension; col++) {
          if (row == col && row != matrixDimension - 1)
            cin >> scalingMatrix[row][col];
          else if (row == matrixDimension - 1 && col == matrixDimension - 1)
            scalingMatrix[row][col] = 1;
          else
            scalingMatrix[row][col] = 0;
        }
      }
      transformationStack.push(multiplyTransformationMatrices(
          transformationStack.top(), scalingMatrix));
      if (pushModeActive == 1)
        currentTransformationCount++;
    }

    else if (commandType == "rotate") {
      // Handle rotation transformation using Rodrigues' formula
      double rotationAngleDegrees;
      struct Coordinate3D axisOfRotation;
      cin >> rotationAngleDegrees >> axisOfRotation.xPos >>
          axisOfRotation.yPos >> axisOfRotation.zPos;

      // Normalize rotation axis
      double axisLength =
          sqrt(pow(axisOfRotation.xPos, 2) + pow(axisOfRotation.yPos, 2) +
               pow(axisOfRotation.zPos, 2));
      axisOfRotation.xPos /= axisLength;
      axisOfRotation.yPos /= axisLength;
      axisOfRotation.zPos /= axisLength;

      // Calculate rotation matrix columns using Rodrigues' formula
      struct Coordinate3D xAxisVector = {1, 0, 0};
      struct Coordinate3D rotatedXAxis = rodriguesRotationFormula(
          xAxisVector, axisOfRotation, rotationAngleDegrees);

      struct Coordinate3D yAxisVector = {0, 1, 0};
      struct Coordinate3D rotatedYAxis = rodriguesRotationFormula(
          yAxisVector, axisOfRotation, rotationAngleDegrees);

      struct Coordinate3D zAxisVector = {0, 0, 1};
      struct Coordinate3D rotatedZAxis = rodriguesRotationFormula(
          zAxisVector, axisOfRotation, rotationAngleDegrees);

      // Construct rotation matrix
      vector<vector<double>> rotationMatrix(matrixDimension);
      for (int row = 0; row < matrixDimension; row++)
        rotationMatrix[row].resize(matrixDimension);

      rotationMatrix[0][0] = rotatedXAxis.xPos;
      rotationMatrix[1][0] = rotatedXAxis.yPos;
      rotationMatrix[2][0] = rotatedXAxis.zPos;
      rotationMatrix[0][1] = rotatedYAxis.xPos;
      rotationMatrix[1][1] = rotatedYAxis.yPos;
      rotationMatrix[2][1] = rotatedYAxis.zPos;
      rotationMatrix[0][2] = rotatedZAxis.xPos;
      rotationMatrix[1][2] = rotatedZAxis.yPos;
      rotationMatrix[2][2] = rotatedZAxis.zPos;

      // Set homogeneous coordinate elements
      for (int row = 0; row < matrixDimension; row++) {
        for (int col = 0; col < matrixDimension; col++) {
          if (row == 3 && col == 3)
            rotationMatrix[row][col] = 1;
          else if (row == 3 || col == 3)
            rotationMatrix[row][col] = 0;
        }
      }

      transformationStack.push(multiplyTransformationMatrices(
          transformationStack.top(), rotationMatrix));
      if (pushModeActive == 1)
        currentTransformationCount++;
    }

    else if (commandType == "push") {
      // Handle matrix stack push operation
      if (pushModeActive == 0)
        pushModeActive = 1;
      else if (pushModeActive == 1) {
        pushedTransformationCount.push(currentTransformationCount);
        currentTransformationCount = 0;
      }
    }

    else if (commandType == "pop") {
      // Handle matrix stack pop operation
      for (int popIndex = 0; popIndex < currentTransformationCount; popIndex++)
        transformationStack.pop();
      if (!pushedTransformationCount.empty()) {
        currentTransformationCount = pushedTransformationCount.top();
        pushedTransformationCount.pop();
      } else
        currentTransformationCount = 0;
    }

    else if (commandType == "end") {
      break;
    }
  }
  cin.clear();
  fclose(stdin);
  fclose(stdout);

  // ================== STAGE 2: VIEW TRANSFORMATION ==================

  freopen("stage1.txt", "r", stdin);
  freopen("stage2.txt", "w", stdout);

  // Calculate view transformation vectors
  vector<double> lookDirection(3), rightDirection(3), upDirection(3);
  lookDirection[0] = lookAtPoint.xPos - cameraPosition.xPos;
  lookDirection[1] = lookAtPoint.yPos - cameraPosition.yPos;
  lookDirection[2] = lookAtPoint.zPos - cameraPosition.zPos;

  // Normalize look direction
  double vectorMagnitude =
      sqrt(pow(lookDirection[0], 2) + pow(lookDirection[1], 2) +
           pow(lookDirection[2], 2));
  lookDirection[0] /= vectorMagnitude;
  lookDirection[1] /= vectorMagnitude;
  lookDirection[2] /= vectorMagnitude;

  // Calculate right direction using cross product
  rightDirection[0] =
      (lookDirection[1] * upVector.zPos) - (lookDirection[2] * upVector.yPos);
  rightDirection[1] =
      (lookDirection[2] * upVector.xPos) - (lookDirection[0] * upVector.zPos);
  rightDirection[2] =
      (lookDirection[0] * upVector.yPos) - (lookDirection[1] * upVector.xPos);

  // Normalize right direction
  vectorMagnitude = sqrt(pow(rightDirection[0], 2) + pow(rightDirection[1], 2) +
                         pow(rightDirection[2], 2));
  rightDirection[0] /= vectorMagnitude;
  rightDirection[1] /= vectorMagnitude;
  rightDirection[2] /= vectorMagnitude;

  // Calculate up direction using cross product
  upDirection[0] = (rightDirection[1] * lookDirection[2]) -
                   (rightDirection[2] * lookDirection[1]);
  upDirection[1] = (rightDirection[2] * lookDirection[0]) -
                   (rightDirection[0] * lookDirection[2]);
  upDirection[2] = (rightDirection[0] * lookDirection[1]) -
                   (rightDirection[1] * lookDirection[0]);

  // Normalize up direction
  vectorMagnitude = sqrt(pow(upDirection[0], 2) + pow(upDirection[1], 2) +
                         pow(upDirection[2], 2));
  upDirection[0] /= vectorMagnitude;
  upDirection[1] /= vectorMagnitude;
  upDirection[2] /= vectorMagnitude;

  // Create translation matrix for view transformation
  vector<double> cameraPositionVector(matrixDimension - 1);
  cameraPositionVector[0] = cameraPosition.xPos;
  cameraPositionVector[1] = cameraPosition.yPos;
  cameraPositionVector[2] = cameraPosition.zPos;

  vector<vector<double>> viewTranslationMatrix(matrixDimension);
  for (int row = 0; row < matrixDimension; row++)
    viewTranslationMatrix[row].resize(matrixDimension);

  for (int row = 0; row < matrixDimension; row++) {
    for (int col = 0; col < matrixDimension; col++) {
      if (row == col)
        viewTranslationMatrix[row][col] = 1;
      else if (col == 3 && row < 3)
        viewTranslationMatrix[row][col] = -cameraPositionVector[row];
      else
        viewTranslationMatrix[row][col] = 0;
    }
  }

  // Create rotation matrix for view transformation
  vector<vector<double>> viewRotationMatrix(matrixDimension);
  for (int row = 0; row < matrixDimension; row++)
    viewRotationMatrix[row].resize(matrixDimension);

  for (int row = 0; row < matrixDimension; row++) {
    for (int col = 0; col < matrixDimension; col++) {
      if (row == 0 && col < 3)
        viewRotationMatrix[row][col] = rightDirection[col];
      else if (row == 1 && col < 3)
        viewRotationMatrix[row][col] = upDirection[col];
      else if (row == 2 && col < 3)
        viewRotationMatrix[row][col] = -lookDirection[col];
      else if (row == 3 && col == 3)
        viewRotationMatrix[row][col] = 1;
      else
        viewRotationMatrix[row][col] = 0;
    }
  }

  // Combine rotation and translation for complete view transformation
  vector<vector<double>> completeViewTransformation =
      multiplyTransformationMatrices(viewRotationMatrix, viewTranslationMatrix);

  // Apply view transformation to all triangles
  int triangleInputCounter = 0;
  while (triangleInputCounter < totalTriangleCount) {
    vector<vector<double>> triangleVertexMatrix(matrixDimension);
    for (int row = 0; row < matrixDimension; row++)
      triangleVertexMatrix[row].resize(matrixDimension);

    // Read triangle coordinates from stage 1 output
    cin >> triangleVertexMatrix[0][0] >> triangleVertexMatrix[1][0] >>
        triangleVertexMatrix[2][0];
    cin >> triangleVertexMatrix[0][1] >> triangleVertexMatrix[1][1] >>
        triangleVertexMatrix[2][1];
    cin >> triangleVertexMatrix[0][2] >> triangleVertexMatrix[1][2] >>
        triangleVertexMatrix[2][2];

    // Set homogeneous coordinates
    for (int row = 0; row < matrixDimension; row++) {
      for (int col = 0; col < matrixDimension; col++) {
        if (row == 3)
          triangleVertexMatrix[row][col] = 1;
        else if (col == 3)
          triangleVertexMatrix[row][col] = 0;
      }
    }

    // Apply view transformation and output result
    vector<vector<double>> viewTransformedTriangle =
        multiplyTransformationMatrices(completeViewTransformation,
                                       triangleVertexMatrix);
    outputTriangleCoordinates(viewTransformedTriangle, matrixDimension);
    triangleInputCounter++;
  }

  fclose(stdin);
  fclose(stdout);

  // ================== STAGE 3: PROJECTION TRANSFORMATION ==================

  freopen("stage2.txt", "r", stdin);
  freopen("stage3.txt", "w", stdout);

  // Calculate projection parameters
  double fieldOfViewX = fieldOfViewY * aspectRatioValue;
  double topClippingPlane =
      nearPlaneDistance * tan(((fieldOfViewY / 2) * PI_CONSTANT) / 180);
  double rightClippingPlane =
      nearPlaneDistance * tan(((fieldOfViewX / 2) * PI_CONSTANT) / 180);

  // Create perspective projection matrix
  vector<vector<double>> perspectiveProjectionMatrix(matrixDimension);
  for (int row = 0; row < matrixDimension; row++)
    perspectiveProjectionMatrix[row].resize(matrixDimension);

  for (int row = 0; row < matrixDimension; row++) {
    for (int col = 0; col < matrixDimension; col++) {
      if (row == 0 && col == 0)
        perspectiveProjectionMatrix[row][col] =
            nearPlaneDistance / rightClippingPlane;
      else if (row == 1 && col == 1)
        perspectiveProjectionMatrix[row][col] =
            nearPlaneDistance / topClippingPlane;
      else if (row == 2 && col == 2)
        perspectiveProjectionMatrix[row][col] =
            -(farPlaneDistance + nearPlaneDistance) /
            (farPlaneDistance - nearPlaneDistance);
      else if (row == 2 && col == 3)
        perspectiveProjectionMatrix[row][col] =
            -(2 * farPlaneDistance * nearPlaneDistance) /
            (farPlaneDistance - nearPlaneDistance);
      else if (row == 3 && col == 2)
        perspectiveProjectionMatrix[row][col] = -1;
      else
        perspectiveProjectionMatrix[row][col] = 0;
    }
  }

  // Apply projection transformation to all triangles
  triangleInputCounter = 0;
  while (triangleInputCounter < totalTriangleCount) {
    vector<vector<double>> triangleVertexMatrix(matrixDimension);
    for (int row = 0; row < matrixDimension; row++)
      triangleVertexMatrix[row].resize(matrixDimension);

    // Read triangle coordinates from stage 2 output
    cin >> triangleVertexMatrix[0][0] >> triangleVertexMatrix[1][0] >>
        triangleVertexMatrix[2][0];
    cin >> triangleVertexMatrix[0][1] >> triangleVertexMatrix[1][1] >>
        triangleVertexMatrix[2][1];
    cin >> triangleVertexMatrix[0][2] >> triangleVertexMatrix[1][2] >>
        triangleVertexMatrix[2][2];

    // Set homogeneous coordinates
    for (int row = 0; row < matrixDimension; row++) {
      for (int col = 0; col < matrixDimension; col++) {
        if (row == 3)
          triangleVertexMatrix[row][col] = 1;
        else if (col == 3)
          triangleVertexMatrix[row][col] = 0;
      }
    }

    // Apply projection transformation and output result
    vector<vector<double>> projectedTriangle = multiplyTransformationMatrices(
        perspectiveProjectionMatrix, triangleVertexMatrix);
    outputTriangleCoordinates(projectedTriangle, matrixDimension);
    triangleInputCounter++;
  }

  fclose(stdin);
  fclose(stdout);

  // ================== STAGE 4: RASTERIZATION WITH Z-BUFFER ==================

  // Read clipping and screen configuration
  freopen(configPath.c_str(), "r", stdin);
  freopen("z_buffer.txt", "w", stdout);

  double screenPixelWidth, screenPixelHeight, leftClippingBoundary,
      rightClippingBoundary, bottomClippingBoundary, topClippingBoundary,
      frontClippingLimit, rearClippingLimit;

  cin >> screenPixelWidth >> screenPixelHeight;
  cin >> leftClippingBoundary >> bottomClippingBoundary;
  cin >> frontClippingLimit >> rearClippingLimit;
  cin.clear();

  // Calculate symmetric clipping boundaries
  rightClippingBoundary = -leftClippingBoundary;
  topClippingBoundary = -bottomClippingBoundary;

  fclose(stdin);
  freopen("stage3.txt", "r", stdin);

  // Load all triangles from projection stage
  struct TriangleObject renderingTriangles[totalTriangleCount];
  triangleInputCounter = 0;

  while (triangleInputCounter < totalTriangleCount) {
    // Read triangle vertex coordinates
    cin >> renderingTriangles[triangleInputCounter].vertices[0].xPos >>
        renderingTriangles[triangleInputCounter].vertices[0].yPos >>
        renderingTriangles[triangleInputCounter].vertices[0].zPos;
    cin >> renderingTriangles[triangleInputCounter].vertices[1].xPos >>
        renderingTriangles[triangleInputCounter].vertices[1].yPos >>
        renderingTriangles[triangleInputCounter].vertices[1].zPos;
    cin >> renderingTriangles[triangleInputCounter].vertices[2].xPos >>
        renderingTriangles[triangleInputCounter].vertices[2].yPos >>
        renderingTriangles[triangleInputCounter].vertices[2].zPos;

    // Assign random colors to triangles for visualization
    renderingTriangles[triangleInputCounter].triangleColor.redComponent =
        rand() % 256;
    renderingTriangles[triangleInputCounter].triangleColor.greenComponent =
        rand() % 256;
    renderingTriangles[triangleInputCounter].triangleColor.blueComponent =
        rand() % 256;

    triangleInputCounter++;
  }
  triangleInputCounter = 0;

  // Initialize z-buffer and frame buffer
  vector<vector<double>> depthBuffer(screenPixelWidth);
  vector<vector<RGBColor>> colorFrameBuffer(screenPixelWidth);

  for (int widthIndex = 0; widthIndex < screenPixelWidth; widthIndex++) {
    depthBuffer[widthIndex].resize(screenPixelHeight);
    colorFrameBuffer[widthIndex].resize(screenPixelHeight);
  }

  // Initialize buffers with default values
  for (int widthIndex = 0; widthIndex < screenPixelWidth; widthIndex++) {
    for (int heightIndex = 0; heightIndex < screenPixelHeight; heightIndex++) {
      depthBuffer[widthIndex][heightIndex] = rearClippingLimit;
      colorFrameBuffer[widthIndex][heightIndex].redComponent = 0;
      colorFrameBuffer[widthIndex][heightIndex].greenComponent = 0;
      colorFrameBuffer[widthIndex][heightIndex].blueComponent = 0;
    }
  }

  // Calculate pixel spacing and screen coordinate centers
  double pixelWidthSpacing =
      (rightClippingBoundary - leftClippingBoundary) / screenPixelWidth;
  double pixelHeightSpacing =
      (topClippingBoundary - bottomClippingBoundary) / screenPixelHeight;
  double topScreenCoordinate = topClippingBoundary - (pixelHeightSpacing / 2);
  double leftScreenCoordinate = leftClippingBoundary + (pixelWidthSpacing / 2);

  // Rasterize each triangle using scanline algorithm
  for (int triangleIndex = 0; triangleIndex < totalTriangleCount;
       triangleIndex++) {
    // Find triangle bounding box for Y coordinates
    int topVertexIndex = locateMaxYVertex(renderingTriangles[triangleIndex]);
    double topScanlineY =
        renderingTriangles[triangleIndex].vertices[topVertexIndex].yPos >
                topClippingBoundary
            ? topClippingBoundary
            : (renderingTriangles[triangleIndex].vertices[topVertexIndex].yPos <
                       bottomClippingBoundary
                   ? 0
                   : renderingTriangles[triangleIndex]
                         .vertices[topVertexIndex]
                         .yPos);
    int topScanlineIndex =
        round((topScreenCoordinate - topScanlineY) / pixelHeightSpacing);

    int bottomVertexIndex = locateMinYVertex(renderingTriangles[triangleIndex]);
    double bottomScanlineY =
        renderingTriangles[triangleIndex].vertices[bottomVertexIndex].yPos <
                bottomClippingBoundary
            ? bottomClippingBoundary
            : (renderingTriangles[triangleIndex]
                           .vertices[bottomVertexIndex]
                           .yPos > topClippingBoundary
                   ? 0
                   : renderingTriangles[triangleIndex]
                         .vertices[bottomVertexIndex]
                         .yPos);
    int bottomScanlineIndex =
        round((topScreenCoordinate - bottomScanlineY) / pixelHeightSpacing);

    // Find triangle bounding box for X coordinates
    int leftVertexIndex = locateMinXVertex(renderingTriangles[triangleIndex]);
    double leftmostXCoord =
        renderingTriangles[triangleIndex].vertices[leftVertexIndex].xPos;

    int rightVertexIndex = locateMaxXVertex(renderingTriangles[triangleIndex]);
    double rightmostXCoord =
        renderingTriangles[triangleIndex].vertices[rightVertexIndex].xPos;

    double middleXCoord = renderingTriangles[triangleIndex]
                              .vertices[3 - leftVertexIndex - rightVertexIndex]
                              .xPos;

    // Extract vertex coordinates for easier access
    double vertexX1 = leftmostXCoord;
    double vertexX2 = rightmostXCoord;
    double vertexX3 = middleXCoord;
    double vertexY1 =
        renderingTriangles[triangleIndex].vertices[leftVertexIndex].yPos;
    double vertexY2 =
        renderingTriangles[triangleIndex].vertices[rightVertexIndex].yPos;
    double vertexY3 = renderingTriangles[triangleIndex]
                          .vertices[3 - leftVertexIndex - rightVertexIndex]
                          .yPos;
    double vertexZ1 =
        renderingTriangles[triangleIndex].vertices[leftVertexIndex].zPos;
    double vertexZ2 =
        renderingTriangles[triangleIndex].vertices[rightVertexIndex].zPos;
    double vertexZ3 = renderingTriangles[triangleIndex]
                          .vertices[3 - leftVertexIndex - rightVertexIndex]
                          .zPos;

    // Scanline rasterization loop
    for (int scanlineIndex = topScanlineIndex;
         scanlineIndex <= bottomScanlineIndex; scanlineIndex++) {
      double currentScanlineY =
          topScreenCoordinate - (scanlineIndex * pixelHeightSpacing);

      // Calculate edge intersections for current scanline
      double intersectionXA =
          vertexX1 - ((vertexX1 - vertexX2) *
                      ((vertexY1 - currentScanlineY) / (vertexY1 - vertexY2)));
      double intersectionZA =
          vertexZ1 - ((vertexZ1 - vertexZ2) *
                      ((vertexY1 - currentScanlineY) / (vertexY1 - vertexY2)));
      double intersectionXB =
          vertexX1 - ((vertexX1 - vertexX3) *
                      ((vertexY1 - currentScanlineY) / (vertexY1 - vertexY3)));
      double intersectionZB =
          vertexZ1 - ((vertexZ1 - vertexZ3) *
                      ((vertexY1 - currentScanlineY) / (vertexY1 - vertexZ3)));
      double intersectionXC =
          vertexX2 - ((vertexX2 - vertexX3) *
                      ((vertexY2 - currentScanlineY) / (vertexY2 - vertexY3)));
      double intersectionZC =
          vertexZ2 - ((vertexZ2 - vertexZ3) *
                      ((vertexY2 - currentScanlineY) / (vertexY2 - vertexY3)));

      // Validate intersection points and handle edge cases
      if (intersectionXA != INFINITY_VAL && !isnan(intersectionXA))
        if (intersectionXA < leftmostXCoord || intersectionXA > rightmostXCoord)
          intersectionXA = INFINITY_VAL;
      if (intersectionXB != INFINITY_VAL && !isnan(intersectionXB))
        if (intersectionXB < leftmostXCoord || intersectionXB > middleXCoord)
          intersectionXB = INFINITY_VAL;
      if (intersectionXC != INFINITY_VAL && !isnan(intersectionXC))
        if (intersectionXC < middleXCoord || intersectionXC > rightmostXCoord)
          intersectionXC = INFINITY_VAL;

      // Handle degenerate cases by selecting valid intersections
      if (intersectionXA == INFINITY_VAL && intersectionXB != INFINITY_VAL &&
          intersectionXC != INFINITY_VAL) {
        intersectionXA = intersectionXC;
        intersectionZA = intersectionZC;
      } else if (intersectionXB == INFINITY_VAL &&
                 intersectionXA != INFINITY_VAL &&
                 intersectionXC != INFINITY_VAL) {
        intersectionXB = intersectionXC;
        intersectionZB = intersectionZC;
      } else if ((intersectionXA == INFINITY_VAL &&
                  intersectionXB == INFINITY_VAL) ||
                 (intersectionXA == INFINITY_VAL &&
                  intersectionXC == INFINITY_VAL) ||
                 (intersectionXB == INFINITY_VAL &&
                  intersectionXC == INFINITY_VAL))
        continue;

      // Ensure proper ordering of intersection points
      double tempXCoord = intersectionXA, tempZCoord = intersectionZA;
      if (intersectionXA > intersectionXB) {
        intersectionXA = intersectionXB;
        intersectionZA = intersectionZB;
        intersectionXB = tempXCoord;
        intersectionZB = tempZCoord;
      }

      // Apply horizontal clipping
      intersectionXA =
          intersectionXA < leftClippingBoundary
              ? leftClippingBoundary
              : (intersectionXA > rightClippingBoundary ? rightClippingBoundary
                                                        : intersectionXA);
      intersectionXB =
          intersectionXB > rightClippingBoundary
              ? rightClippingBoundary
              : (intersectionXB < leftClippingBoundary ? leftClippingBoundary
                                                       : intersectionXB);

      // Calculate pixel column ranges
      int leftPixelColumn =
          round(abs(intersectionXA - leftScreenCoordinate) / pixelWidthSpacing);
      int rightPixelColumn =
          round(abs(intersectionXB - leftScreenCoordinate) / pixelWidthSpacing);

      // Interpolate Z values across the scanline
      double currentDepthValue;
      double depthIncrement = pixelWidthSpacing *
                              (intersectionZB - intersectionZA) /
                              (intersectionXB - intersectionXA);

      for (int pixelColumn = leftPixelColumn; pixelColumn <= rightPixelColumn;
           pixelColumn++) {
        if (pixelColumn == leftPixelColumn)
          currentDepthValue =
              intersectionZA +
              (((leftScreenCoordinate + leftPixelColumn * pixelWidthSpacing) -
                intersectionXA) *
               (intersectionZB - intersectionZA) /
               (intersectionXB - intersectionXA));
        else
          currentDepthValue = currentDepthValue + depthIncrement;

        // Perform depth test and update buffers
        if (currentDepthValue > frontClippingLimit &&
            currentDepthValue < depthBuffer[scanlineIndex][pixelColumn]) {
          depthBuffer[scanlineIndex][pixelColumn] = currentDepthValue;
          colorFrameBuffer[scanlineIndex][pixelColumn] =
              renderingTriangles[triangleIndex].triangleColor;
        }
      }
    }
  }

  // Output Z-buffer values to file
  for (int widthIndex = 0; widthIndex < screenPixelWidth; widthIndex++) {
    for (int heightIndex = 0; heightIndex < screenPixelHeight; heightIndex++) {
      if (depthBuffer[widthIndex][heightIndex] < rearClippingLimit)
        cout << fixed << setprecision(6) << depthBuffer[widthIndex][heightIndex]
             << "\t";
    }
    cout << endl;
  }

  fclose(stdout);

  // Generate final rendered image
  bitmap_image finalRenderedImage(screenPixelHeight, screenPixelWidth);
  for (int widthIndex = 0; widthIndex < screenPixelWidth; widthIndex++) {
    for (int heightIndex = 0; heightIndex < screenPixelHeight; heightIndex++) {
      finalRenderedImage.set_pixel(
          heightIndex, widthIndex,
          colorFrameBuffer[widthIndex][heightIndex].redComponent,
          colorFrameBuffer[widthIndex][heightIndex].greenComponent,
          colorFrameBuffer[widthIndex][heightIndex].blueComponent);
    }
  }
  finalRenderedImage.save_image("out.bmp");

  // Clean up memory allocations
  for (int widthIndex = 0; widthIndex < screenPixelWidth; widthIndex++) {
    depthBuffer[widthIndex].clear();
    colorFrameBuffer[widthIndex].clear();
    depthBuffer[widthIndex].shrink_to_fit();
    colorFrameBuffer[widthIndex].shrink_to_fit();
  }
  depthBuffer.clear();
  colorFrameBuffer.clear();
  depthBuffer.shrink_to_fit();
  colorFrameBuffer.shrink_to_fit();

  return 0;
}