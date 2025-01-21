package rtl;

public class Boolean {
    public boolean _value;

    public Boolean(boolean value)
    {
        _value = value;
    }

    public boolean toBool()
    {
        return _value;
    }

    public Boolean logAnd(Boolean other)
    {
        _value = _value && other._value;
        return this;
    }

    public Boolean logOr(Boolean other)
    {
        _value = _value || other._value;
        return this;
    }

    public Boolean logNot()
    {
        _value = !_value;
        return this;
    }
}
