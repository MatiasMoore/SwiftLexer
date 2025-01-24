package rtl;

public class Operators {

    // Boolean
    public static Boolean logAnd(Boolean a, Boolean b)
    {
        return new Boolean(a._value && b._value);
    }

    public static Boolean logOr(Boolean a, Boolean b)
    {
        return new Boolean(a._value || b._value);
    }

    public static Boolean logNot(Boolean a)
    {
        return new Boolean(!a._value);
    }

    // Integer

    public static Integer unaryMinus(Integer a)
    {
        return new Integer(-a._value);
    }
    public static Integer sum(Integer a, Integer b)
    {
        return new Integer(a._value + b._value);
    }

    public static Integer sub(Integer a, Integer b)
    {
        return new Integer(a._value - b._value);
    }

    public static Integer mul(Integer a, Integer b)
    {
        return new Integer(a._value * b._value);
    }

    public static Integer div(Integer a, Integer b)
    {
        return new Integer(a._value / b._value);
    }

    public static Boolean lt(Integer a, Integer b)
    {
        return new Boolean(a._value < b._value);
    }

    public static Boolean gt(Integer a, Integer b)
    {
        return new Boolean(a._value > b._value);
    }

    public static Boolean lte(Integer a, Integer b)
    {
        return new Boolean(a._value <= b._value);
    }

    public static Boolean gte(Integer a, Integer b)
    {
        return new Boolean(a._value >= b._value);
    }

    public static Boolean neq(Integer a, Integer b)
    {
        return new Boolean(a._value != b._value);
    }

    public static Boolean eq(Integer a, Integer b)
    {
        return new Boolean(a._value == b._value);
    }

    // Float

    public static Integer unaryFloat(Integer a)
    {
        return new Integer(-a._value);
    }
    public static Float sum(Float a, Float b)
    {
        return new Float(a._value + b._value);
    }

    public static Float sub(Float a, Float b)
    {
        return new Float(a._value - b._value);
    }

    public static Float mul(Float a, Float b)
    {
        return new Float(a._value * b._value);
    }

    public static Float div(Float a, Float b)
    {
        return new Float(a._value / b._value);
    }

    public static Boolean lte(Float a, Float b)
    {
        return new Boolean(a._value <= b._value);
    }

    public static Boolean gte(Float a, Float b)
    {
        return new Boolean(a._value >= b._value);
    }

    public static Boolean neq(Float a, Float b)
    {
        return new Boolean(a._value != b._value);
    }

    public static Boolean eq(Float a, Float b)
    {
        return new Boolean(a._value == b._value);
    }

    // String
    public static String sum(String a, String b)
    {
        return new String(a._value + b._value);
    }

}
