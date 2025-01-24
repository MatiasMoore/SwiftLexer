package rtl;

public class Range {
    private int _startVal = 0;
    private int _endVal = 0;
    private boolean _isHalfOpen = false;
    public Range(Integer start, Integer end, Boolean isHalfOpen)
    {
        _startVal = start._value;
        _endVal = end._value;
        _isHalfOpen = isHalfOpen._value;
    }

    public Range(Integer start, Integer end, boolean isHalfOpen)
    {
        _startVal = start._value;
        _endVal = end._value;
        _isHalfOpen = isHalfOpen;
    }
    
    public Integer[] toArray() {
        int size = _isHalfOpen ? (_endVal - _startVal) : (_endVal - _startVal + 1);
        Integer[] result = new Integer[size];

        for (int i = 0; i < size; i++) {
            result[i] = new Integer(_startVal + i);
        }
        return result;
    }

}
