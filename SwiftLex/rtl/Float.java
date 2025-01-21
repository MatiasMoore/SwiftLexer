package rtl;

public class Float {
    public float _value;

    public Float(float value)
    {
        _value = value;
    }

    public Float sum(Float other)
    {
        _value += other._value;
        return this;
    }

    public Float sub(Float other)
    {
        _value -= other._value;
        return this;
    }

    public Float mul(Float other)
    {
        _value *= other._value;
        return this;
    }

    public Float div(Float other)
    {
        _value /= other._value;
        return this;
    }
}
