package rtl;

public class Float {
    public float _value;

    public Float(Integer i)
    {
        _value = i._value;
    }
    public Float(float value)
    {
        _value = value;
    }
    
    public Float sum(Float other)
    {
        return new Float(_value + other._value);
    }

    public Float sub(Float other)
    {
        return new Float(_value - other._value);
    }

    public Float mul(Float other)
    {
        return new Float(_value * other._value);
    }

    public Float div(Float other)
    {
        return new Float(_value / other._value);
    }
}
