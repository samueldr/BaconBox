#include "Matrix2D.h"
#include "Vector2.h"
#include <cmath>

#include "BaconBox/Helper/MathHelper.h"
#include "BaconBox/Helper/Serialization/Array.h"
#include "BaconBox/Helper/Serialization/Object.h"

#include "BaconBox/Helper/Serialization/Value.h"
#include "BaconBox/Helper/Serialization/DefaultSerializer.h"
#include "BaconBox/Helper/Serialization/Serializer.h"

namespace BaconBox {
	Matrix2D::Matrix2D(const Matrix2D &src) : a(src.a), b(src.b), c(src.c), d(src.d), tx(src.tx), ty(src.ty) {}
	
	Matrix2D::Matrix2D():a(1),b(0),c(0),d(1),tx(0),ty(0){}
	
	Matrix2D::Matrix2D(float a, float b, float c, float d, float tx, float ty):a(a),b(b),c(c),d(d),tx(tx),ty(ty){}

	Matrix2D &Matrix2D::operator=(const Matrix2D &src) {
		a = src.a;
		b = src.b;
		c = src.c;
		d = src.d;
		tx = src.tx;
		ty = src.ty;
		return *this;
	}

	
	Matrix2D Matrix2D::operator*(const Matrix2D & m) const{
	    return Matrix2D(a*m.a + c*m.b, 
			     b*m.a + d*m.b, 
			     a*m.c + c*m.d,
			     b*m.c + d*m.d,
			     a*m.tx+ c*m.ty +tx,
			     b*m.tx+ d*m.ty +ty);
	}
	

	
	Matrix2D &Matrix2D::operator*=(const Matrix2D& m) {
		(*this) = (*this) * m;
		return (*this);
	}
	
	Matrix2D & Matrix2D::concat(const Matrix2D & m){
	    (*this) = m * (*this);
	    return (*this);
	}
	
//	bool Matrix2D::isSkewed() const{
//	    return !(-b == c);
//	}

	
	
	Vector2 Matrix2D::multiplyWithVector(const Vector2 & v) const{
	    return Vector2(a*v.x + c*v.y + tx, b*v.x + d*v.y + ty);
	}

	
	void Matrix2D::translate(const Vector2 & v){
	    Matrix2D temp;
	    temp.tx = v.x;
	    temp.ty = v.y;
	    (*this) = temp * (*this);
	}
	
	void Matrix2D::scale(const Vector2 & v){
	    Matrix2D temp;
	    temp.a *= v.x;
	    temp.d *= v.y;
	    float tempTx = tx;
	    float tempTy = ty;
	    tx = 0;
	    ty =0;
	    (*this) = (*this) * temp;
	    tx = tempTx;
	    ty = tempTy;
	}
	
	void Matrix2D::rotate(float angle){
	    angle *=  MathHelper::AngleConvert<float>::DEGREES_TO_RADIANS; 
	    float cosValue = MathHelper::cos(angle);
	    float sinValue = MathHelper::sin(angle);
	    (*this) = (*this) * Matrix2D(cosValue, -sinValue, sinValue, cosValue,0,0);
	
	}
	
	std::ostream &operator<<(std::ostream &output, const Matrix2D& m) {
		Value tmpValue;
		DefaultSerializer::serialize(m, tmpValue);
		DefaultSerializer::getDefaultSerializer().writeToStream(output, tmpValue);
		return output;
	}
	
	void Matrix2D::serialize(Value &node, bool setName) const {
			if (setName) {
				node.setName("Matrix2D");
			}
			node["a"].setDouble(static_cast<double>(a));
			node["b"].setDouble(static_cast<double>(b));
			node["c"].setDouble(static_cast<double>(c));
			node["d"].setDouble(static_cast<double>(d));
			node["tx"].setDouble(static_cast<double>(tx));
			node["ty"].setDouble(static_cast<double>(ty));
		}

		
		bool Matrix2D::deserialize(const Value &node) {
			bool result = true;
			if (node.getArray().size() == 6) {
				a = static_cast<float>(node.getArray()[0].getDouble());
				b = static_cast<float>(node.getArray()[1].getDouble());
				c = static_cast<float>(node.getArray()[2].getDouble());
				d = static_cast<float>(node.getArray()[3].getDouble());
				tx = static_cast<float>(node.getArray()[4].getDouble());
				ty = static_cast<float>(node.getArray()[5].getDouble());
			} else {
				Object::const_iterator itA = node.getObject().find("a");
				Object::const_iterator itB = node.getObject().find("b");
				Object::const_iterator itC = node.getObject().find("c");
				Object::const_iterator itD = node.getObject().find("d");
				Object::const_iterator itTX = node.getObject().find("tx");
				Object::const_iterator itTY = node.getObject().find("ty");
				

				// We make sure the value contains the "x" and "y" values.
				if (itA != node.getObject().end() &&
				    itB != node.getObject().end() &&
				    itC != node.getObject().end() &&
				    itD != node.getObject().end() &&
				    itTX != node.getObject().end() &&
				    itTY != node.getObject().end() &&
				    itA->second.isNumeric() && itB->second.isNumeric() &&
				    itC->second.isNumeric() && itD->second.isNumeric() &&
				    itTX->second.isNumeric() && itTY->second.isNumeric()) {

					a = static_cast<float>(itA->second.getDouble());
					b = static_cast<float>(itB->second.getDouble());
					c = static_cast<float>(itC->second.getDouble());
					d = static_cast<float>(itD->second.getDouble());
					tx = static_cast<float>(itTX->second.getDouble());
					ty = static_cast<float>(itTY->second.getDouble());

				} else {
					result = false;
				}
			}

			return result;
		}

		
		bool Matrix2D::isValidValue(const Value &node) {
			Object::const_iterator itA = node.getObject().find("a");
			Object::const_iterator itB = node.getObject().find("b");
			Object::const_iterator itC = node.getObject().find("c");
			Object::const_iterator itD = node.getObject().find("d");
			Object::const_iterator itTX = node.getObject().find("tx");
			Object::const_iterator itTY = node.getObject().find("ty");

			return node.getArray().size() == 6 || (itA != node.getObject().end() && itB != node.getObject().end() &&
								itC != node.getObject().end() && itD != node.getObject().end() &&
								itTX != node.getObject().end() && itTY != node.getObject().end() &&
								itA->second.isNumeric() && itB->second.isNumeric() &&
								itC->second.isNumeric() && itD->second.isNumeric() &&
								itTX->second.isNumeric() && itTY->second.isNumeric());
		}
}
