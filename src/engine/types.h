#pragma once

#include <stdint.h>


const int s2_solverTypeCount = 7;

typedef struct s2Vec2 {
	float x, y;
} s2Vec2;


typedef struct s2Color {
	float r, g, b, a;
} s2Color;


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