package rtl;

public class Integer {
    public int _value;

    public Integer(int value)
    {
        _value = value;
    }
    
    public Integer sum(Integer other)
    {
        _value += other._value;
        return this;
    }

    public int toInt()
    {
        return this._value;
    }

    public Integer sub(Integer other)
    {
        _value -= other._value;
        return this;
    }

    public Integer mul(Integer other)
    {
        _value *= other._value;
        return this;
    }

    public Integer div(Integer other)
    {
        _value /= other._value;
        return this;
    }

    public Boolean lt(Integer other)
    {
        return new Boolean(this._value < other._value);
    }
}
