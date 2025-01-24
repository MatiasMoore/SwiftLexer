package rtl;

public class String {
    public java.lang.String _value;

    public String(java.lang.String value)
    {
        _value = value;
    }

    public String(Integer i)
    {
        _value = java.lang.String.valueOf(i._value);
    }

    public String(Float f)
    {
        _value = java.lang.String.valueOf(f._value);
    }

}
