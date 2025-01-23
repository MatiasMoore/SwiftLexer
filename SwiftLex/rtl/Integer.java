package rtl;

public class Integer {
    public int _value;

    public Integer(int value)
    {
        _value = value;
    }
    
    public Integer sum(Integer other)
    {
        return new Integer(_value + other._value);
    }

    public int toInt()
    {
        return this._value;
    }

    public Integer sub(Integer other)
    {
        return new Integer(_value - other._value);
    }

    public Integer mul(Integer other)
    {
        return new Integer(_value * other._value);
    }

    public Integer div(Integer other)
    {
        return new Integer(_value / other._value);
    }

    public Boolean lt(Integer other)
    {
        return new Boolean(this._value < other._value);
    }

    public Boolean gt(Integer other)
    {
        return new Boolean(this._value > other._value);
    }
}
