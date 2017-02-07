// 2D Vector
typedef struct Vectors2D
{
    double x;
    double y;
}Vec2d;

// 3D Vector
typedef struct Vectors3D
{
    double x;
    double y;
    double z;
}Vec3d;

//@2D: Subtract v1 from v2 and put answer in ans
#define subtract(v1, v2, ans)   {ans.x = v2.x - v1.x;ans.y = v2.y - v1.y;}

//@2D: Add v1 and v2 and put answer in ans
#define add(v1, v2, ans)   {ans.x = v2.x + v1.x;ans.y = v2.y + v1.y;}

float length(Vec2d v);

float distanceBetween(Vec2d v1, Vec2d v2);

Vec2d getNewVec2d(double, double);
