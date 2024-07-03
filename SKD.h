#pragma once
#include <corecrt_math_defines.h>
#include <random>


#include "spoofer.h"
#include <d3dx9math.h>
extern int screen_width;
extern int screen_height;
class vector2
{
public:
	vector2() : x(0.f), y(0.f)
	{

	}

	vector2(double _x, double _y) : x(_x), y(_y)
	{

	}
	~vector2()
	{

	}


	vector2 operator-(vector2 v)
	{
		return vector2(x - v.x, y - v.y);
	}


	double x;
	double y;

	inline bool valid_location() {
		SPOOF_FUNC;
		if (x > screen_width or y > screen_height)
			return false;
		else if (x <= 0 or y <= 0)
			return false;
		else if (std::isnan(x) or std::isnan(y))
			return false;
		else
			return true;

	}

};
class vector3
{
public:
	vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~vector3()
	{

	}

	double x;
	double y;
	double z;

	inline double dot(vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double distance(vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double length() {
		return sqrt(x * x + y * y + z * z);
	}

	vector3 operator+(vector3 v)
	{
		return vector3(x + v.x, y + v.y, z + v.z);
	}

	vector3 operator-(vector3 v)
	{
		return vector3(x - v.x, y - v.y, z - v.z);
	}

	vector3 operator/(double flNum)
	{
		return vector3(x / flNum, y / flNum, z / flNum);
	}


	vector3 operator*(double flNum) { return vector3(x * flNum, y * flNum, z * flNum); }
	void set_random_coordinates() {
		SPOOF_FUNC;
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> dis(0.0, 360.0);

		x = dis(gen);
		y = dis(gen);
		z = dis(gen);
	}
};
struct fquat
{
	double x;
	double y;
	double z;
	double w;
};
struct frotator
{
	double Pitch;
	double Yaw;
	double Roll;
};
auto in_screen(vector2 screen_location) -> bool {

	if (screen_location.x > 0 && screen_location.x < screen_width && screen_location.y > 0 && screen_location.y < screen_height) return true;
	else return false;
}


inline bool is_in_screen(vector2 head_pos_dol, int width, int height) {
	if (((head_pos_dol.x <= 0 || head_pos_dol.x > width) && (head_pos_dol.y <= 0 || head_pos_dol.y > height)) || ((head_pos_dol.x <= 0 || head_pos_dol.x > width) || (head_pos_dol.y <= 0 || head_pos_dol.y > height))) {
		return false;
	}
	else {
		return true;
	}
}
struct FTransform
{
	fquat rot;
	vector3 translation;
	char pad[4];
	vector3 scale;
	char pad1[4];
	D3DMATRIX ToMatrixWithScale()
	{
		SPOOF_FUNC;
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};
inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	SPOOF_FUNC;
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}
#define PI 3.14159265358979323846f
struct _MATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
};
inline _MATRIX Matrix(vector3 Vec4, vector3 origin = vector3(0, 0, 0))
{
	SPOOF_FUNC;
	double radPitch = (Vec4.x * double(PI) / 180.f);
	double radYaw = (Vec4.y * double(PI) / 180.f);
	double radRoll = (Vec4.z * double(PI) / 180.f);

	double SP = sinf(radPitch);
	double CP = cosf(radPitch);
	double SY = sinf(radYaw);
	double CY = cosf(radYaw);
	double SR = sinf(radRoll);
	double CR = cosf(radRoll);

	_MATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}
inline char* wchar_to_char(const wchar_t* pwchar)
{
	SPOOF_FUNC;
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}



struct camera_position_s {
	vector3 location{};
	vector3 rotation{};
	float fov{};
};
inline camera_position_s camera_postion{};

extern int screen_width;
extern int screen_height;


int dynamicfovval;

inline vector2 w2s(vector3 WorldLocation)
{
	SPOOF_FUNC;
	if (WorldLocation.x == 0)
		return vector2(0, 0);

	_MATRIX tempMatrix = Matrix(camera_postion.rotation);

	vector3 vAxisX = vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	vector3 vAxisY = vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	vector3 vAxisZ = vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	vector3 vDelta = WorldLocation - camera_postion.location;
	vector3 vTransformed = vector3(vDelta.dot(vAxisY), vDelta.dot(vAxisZ), vDelta.dot(vAxisX));

	dynamicfovval = camera_postion.fov;

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return vector2((screen_width / 2.0f) + vTransformed.x * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z, (screen_height / 2.0f) - vTransformed.y * (((screen_width / 2.0f) / tanf(camera_postion.fov * (float)M_PI / 360.f))) / vTransformed.z);
}

#include "protect/xorstr.h"




