#pragma once

#include <stdint.h>


typedef struct urVec2 {
	float x, y;
} urVec2;

typedef struct urVec3 {
	float x, y;
} urVec3;


typedef struct urColor {
	float r, g, b, a;
} urColor;


typedef enum s2BodyType {
	s2_staticBody = 0,
	s2_kinematicBody = 1,
	s2_dynamicBody = 2,
	s2_bodyTypeCount
} s2BodyType;

typedef struct s2WorldId
{
	int16_t index;
	uint16_t revision;
} s2WorldId;

/// References a rigid body instance
typedef struct s2BodyId
{
	int32_t index;
	int16_t world;
	uint16_t revision;
} s2BodyId;

/// References a shape instance
typedef struct s2ShapeId
{
	int32_t index;
	int16_t world;
	uint16_t revision;
} s2ShapeId;

/// References a joint instance
typedef struct s2JointId
{
	int32_t index;
	int16_t world;
	uint16_t revision;
} s2JointId;


static const s2BodyId s2_nullBodyId = {-1, -1, 0};


/// Gravitational Physics
/// ------------------------------
/// Defines the rules and types that are used by physics bodies that interact with gravity

typedef enum urGravSolverType
{
	ur_gravSolverNI,
	ur_gravSolverTCM,
	ur_gravSolverFMM,
	ur_gravSolverPMM,
	ur_gravSolverPMM_PMT,
	ur_gravSolverGMFM,	
	ur_gravSolverTypeCount,
} s2SolverType;