package rtl;

public class String {
    public java.lang.String _value;

    public String(java.lang.String value)
    {
        _value = value;
    }

    public String sum(String other)
    {
        _value += other._value;
        return this;
    }

    public Boolean isEmpty()
    {
        return new Boolean(this._value.isEmpty());
    }

    public Boolean hasSuffix(String other)
    {
        return new Boolean(_value.endsWith(other._value));
    }

    public Boolean hasPrefix(String other)
    {
        return new Boolean(_value.startsWith(other._value));
    }

}
