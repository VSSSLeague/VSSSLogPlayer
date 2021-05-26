#ifndef DEFAULT_3V3_FIELD_H
#define DEFAULT_3V3_FIELD_H

#include <vector>
#include <math.h>
#include <src/player/gui/fieldview/fieldobjects/fieldobjects.h>

namespace DEFAULT_3V3_FIELD {
    const double kFieldLength = 1500.0;
    const double kFieldWidth = 1300.0;
    const double kGoalWidth = 400.0;
    const double kGoalDepth = 100.0;
    const double kBoundaryWidth = 300.0;

    const double kCenterRadius = 200.0;
    const double kDefenseRadius = 150.0;
    const double kDefenseStretch = 700.0;
    const double kLineThickness = 3.0;
    const double kXMax = kFieldLength/2;
    const double kXMin = -kXMax;
    const double kYMax = kFieldWidth/2;
    const double kYMin = -kYMax;
    const double kFieldCorner = kFieldLength*5/150;
    const double kMarkDistanceX = kFieldLength/4;
    const double kMarkDistanceY = 4*kFieldLength/15;
    const double kMarkLength = kFieldLength*5/150;
    const double kMarkCircleDistance = 2*kFieldLength/15;
    const double kMarkCircleRadius = kMarkLength/10;

    const std::size_t kNumFieldLines = 23;

    const std::size_t kNumLeftGoalLines = 3;
    const FieldLine kLeftGoalLines[kNumLeftGoalLines] = {
        FieldLine("LeftGoalStretch", QVector2D(kXMin-kGoalDepth, -kGoalWidth/2), QVector2D(kXMin-kGoalDepth, kGoalWidth/2), kLineThickness),
        FieldLine("LeftGoalLeftLine", QVector2D(kXMin, kGoalWidth/2), QVector2D(kXMin-kGoalDepth-kLineThickness/2, kGoalWidth/2), kLineThickness),
        FieldLine("LeftGoalRightLine", QVector2D(kXMin, -kGoalWidth/2), QVector2D(kXMin-kGoalDepth-kLineThickness/2, -kGoalWidth/2), kLineThickness)
    };

    const std::size_t kNumRightGoalLines = 3;
    const FieldLine kRightGoalLines[kNumRightGoalLines] = {
        FieldLine("RightGoalStretch", QVector2D(kXMax+kGoalDepth, -kGoalWidth/2), QVector2D(kXMax+kGoalDepth, kGoalWidth/2), kLineThickness),
        FieldLine("RightGoalLeftLine", QVector2D(kXMax, kGoalWidth/2), QVector2D(kXMax+kGoalDepth+kLineThickness/2, kGoalWidth/2), kLineThickness),
        FieldLine("RightGoalRightLine", QVector2D(kXMax, -kGoalWidth/2), QVector2D(kXMax+kGoalDepth+kLineThickness/2, -kGoalWidth/2), kLineThickness),
    };

    const FieldLine kFieldLines[kNumFieldLines] = {
      FieldLine("LeftGoalLine", QVector2D(kXMin, kYMin), QVector2D(kXMin, kYMax), kLineThickness),
      FieldLine("RightGoalLine", QVector2D(kXMax, kYMin), QVector2D(kXMax, kYMax), kLineThickness),
      FieldLine("TopTouchLine", QVector2D(kXMin-kLineThickness/2, kYMax), QVector2D(kXMax+kLineThickness/2, kYMax), kLineThickness),
      FieldLine("BottomTouchLine", QVector2D(kXMin-kLineThickness/2, kYMin), QVector2D(kXMax+kLineThickness/2, kYMin), kLineThickness),
      FieldLine("HalfwayLine", QVector2D(0, kYMin), QVector2D(0, kYMax), kLineThickness),
      FieldLine("LeftPenaltyStretch", QVector2D(kXMin+kDefenseRadius, -kDefenseStretch/2), QVector2D(kXMin+kDefenseRadius, kDefenseStretch/2), kLineThickness),
      FieldLine("RightPenaltyStretch", QVector2D(kXMax-kDefenseRadius, -kDefenseStretch/2), QVector2D(kXMax-kDefenseRadius, kDefenseStretch/2), kLineThickness),
      FieldLine("LeftFieldLeftDefenseLine", QVector2D(kXMin, kDefenseStretch/2), QVector2D(kXMin+kDefenseRadius+kLineThickness/2, kDefenseStretch/2), kLineThickness),
      FieldLine("LeftFieldRightDefenseLine", QVector2D(kXMin, -kDefenseStretch/2), QVector2D(kXMin+kDefenseRadius+kLineThickness/2, -kDefenseStretch/2), kLineThickness),
      FieldLine("RightFieldLeftDefenseLine", QVector2D(kXMax, kDefenseStretch/2), QVector2D(kXMax-kDefenseRadius-kLineThickness/2, kDefenseStretch/2), kLineThickness),
      FieldLine("RightFieldRightDefenseLine", QVector2D(kXMax, -kDefenseStretch/2), QVector2D(kXMax-kDefenseRadius-kLineThickness/2, -kDefenseStretch/2), kLineThickness),
      FieldLine("FBRightTopH", QVector2D(kMarkDistanceX-kMarkLength/2, kMarkDistanceY), QVector2D(kMarkDistanceX+kMarkLength/2, kMarkDistanceY), kLineThickness),
      FieldLine("FBRightTopV", QVector2D(kMarkDistanceX, kMarkDistanceY-kMarkLength/2), QVector2D(kMarkDistanceX, kMarkDistanceY+kMarkLength/2), kLineThickness),
      FieldLine("FBRightBottomH", QVector2D(kMarkDistanceX-kMarkLength/2, -kMarkDistanceY), QVector2D(kMarkDistanceX+kMarkLength/2, -kMarkDistanceY), kLineThickness),
      FieldLine("FBRightBottomV", QVector2D(kMarkDistanceX, -kMarkDistanceY-kMarkLength/2), QVector2D(kMarkDistanceX, -kMarkDistanceY+kMarkLength/2), kLineThickness),
      FieldLine("FBRightTopH", QVector2D(-kMarkDistanceX-kMarkLength/2, kMarkDistanceY), QVector2D(-kMarkDistanceX+kMarkLength/2, kMarkDistanceY), kLineThickness),
      FieldLine("FBRightTopV", QVector2D(-kMarkDistanceX, kMarkDistanceY-kMarkLength/2), QVector2D(-kMarkDistanceX, kMarkDistanceY+kMarkLength/2), kLineThickness),
      FieldLine("FBLeftBottomH", QVector2D(-kMarkDistanceX-kMarkLength/2, -kMarkDistanceY), QVector2D(-kMarkDistanceX+kMarkLength/2, -kMarkDistanceY), kLineThickness),
      FieldLine("FBLeftBottomV", QVector2D(-kMarkDistanceX, -kMarkDistanceY-kMarkLength/2), QVector2D(-kMarkDistanceX, -kMarkDistanceY+kMarkLength/2), kLineThickness),
      FieldLine("PKRightH", QVector2D(kMarkDistanceX-kMarkLength/2, 0), QVector2D(kMarkDistanceX+kMarkLength/2, 0), kLineThickness),
      FieldLine("PKRightV", QVector2D(kMarkDistanceX, -kMarkLength/2), QVector2D(kMarkDistanceX, kMarkLength/2), kLineThickness),
      FieldLine("PKLeftH", QVector2D(-kMarkDistanceX-kMarkLength/2, 0), QVector2D(-kMarkDistanceX+kMarkLength/2, 0), kLineThickness),
      FieldLine("PKLeftV", QVector2D(-kMarkDistanceX, -kMarkLength/2), QVector2D(-kMarkDistanceX, kMarkLength/2), kLineThickness),
    };

    const std::size_t kNumFieldArcs = 11;

    const FieldCircularArc kFieldArcs[kNumFieldArcs] = {
      FieldCircularArc("CenterCircle", QVector2D(0, 0), kCenterRadius-kLineThickness/2, 0, 2*M_PI, kLineThickness),
      FieldCircularArc("FBRightTopLeft", QVector2D(kMarkDistanceX-kMarkCircleDistance, kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("FBRightTopRight", QVector2D(kMarkDistanceX+kMarkCircleDistance, kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("FBRightBottomLeft", QVector2D(kMarkDistanceX-kMarkCircleDistance, -kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("FBRightBottomRight", QVector2D(kMarkDistanceX+kMarkCircleDistance, -kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("FBLeftTopLeft", QVector2D(-kMarkDistanceX-kMarkCircleDistance, kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("FBLeftTopRight", QVector2D(-kMarkDistanceX+kMarkCircleDistance, kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("FBLeftBottomLeft", QVector2D(-kMarkDistanceX-kMarkCircleDistance, -kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("FBLeftBottomRight", QVector2D(-kMarkDistanceX+kMarkCircleDistance, -kMarkDistanceY), kMarkCircleRadius, 0, 2*M_PI, 2*kMarkCircleRadius),
      FieldCircularArc("GoalRightArc", QVector2D(kXMax-kDefenseRadius/2, 0), kDefenseRadius/2+50, M_PI-0.9272, M_PI+0.9272, kLineThickness),
      FieldCircularArc("GoalLeftArc", QVector2D(kXMin+kDefenseRadius/2, 0), kDefenseRadius/2+50, -0.9272, +0.9272, kLineThickness),
    };

    const std::size_t kNumFieldTriangles = 4;

    const FieldTriangle kFieldTriangles[kNumFieldTriangles] = {
      FieldTriangle("RightTop", QVector2D(kXMax-kFieldCorner, kYMax), QVector2D(kXMax, kYMax-kFieldCorner), QVector2D(kXMax, kYMax), kLineThickness),
      FieldTriangle("RightBottom", QVector2D(kXMax-kFieldCorner, -kYMax), QVector2D(kXMax, -kYMax+kFieldCorner), QVector2D(kXMax, -kYMax), kLineThickness),
      FieldTriangle("LeftTop", QVector2D(-kXMax+kFieldCorner, kYMax), QVector2D(-kXMax, kYMax-kFieldCorner), QVector2D(-kXMax, kYMax), kLineThickness),
      FieldTriangle("LeftBottom", QVector2D(-kXMax+kFieldCorner, -kYMax), QVector2D(-kXMax, -kYMax+kFieldCorner), QVector2D(-kXMax, -kYMax), kLineThickness),
    };

}

#endif // DEFAULT_3V3_FIELD_H
