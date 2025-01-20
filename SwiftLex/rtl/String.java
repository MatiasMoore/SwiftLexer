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

}
