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

    public String(Boolean f)
    {
        _value = f._value ? "true" : "false";
    }

    public Integer toInt()
    {
        return new Integer(java.lang.Integer.parseInt(this._value));
    }

    public Float toFloat()
    {
        return new Float(java.lang.Float.parseFloat(this._value));
    }

}
