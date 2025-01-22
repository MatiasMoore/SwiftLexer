package rtl;

public class Array {
    public java.lang.Object[] _arr;

    public Array(java.lang.Object[] arr)
    {
        _arr = arr;
    }

    public Integer count()
    {
        return new Integer(_arr.length);
    }

}
