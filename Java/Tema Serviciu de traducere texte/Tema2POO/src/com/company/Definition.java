package com.company;

import java.util.Arrays;

public class Definition implements Comparable{
    String dict;
    String dictType;
    int year;
    String[] text;

    public Definition() {
    }

    @Override
    public int compareTo(Object o) {
        return this.year - ((Definition)o).year;
    }

    public Definition(String dict, String dictType, int year, String[] text) {
        this.dict = dict;
        this.dictType = dictType;
        this.year = year;
        this.text = text;
    }

    public String getDict() {
        return dict;
    }

    public String getDictType() {
        return dictType;
    }

    public int getYear() {
        return year;
    }

    public String[] getText() {
        return text;
    }

    public void setDict(String dict) {
        this.dict = dict;
    }

    public void setDictType(String dictType) {
        this.dictType = dictType;
    }

    public void setYear(int year) {
        this.year = year;
    }

    public void setText(String[] text) {
        this.text = text;
    }

    @Override
    public String toString() {
        return "Dictionary{" +
                "dict='" + dict + '\'' +
                ", dictType='" + dictType + '\'' +
                ", year=" + year +
                ", text=" + Arrays.toString(text) +
                '}';
    }
}
