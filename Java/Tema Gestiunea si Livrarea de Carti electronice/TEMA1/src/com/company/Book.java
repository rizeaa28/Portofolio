package com.company;

import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Book implements IPublishingArtifact{
    int ID;
    String name;
    String subtitle;
    public String ISBN;
    int pageCount;
    String keywords;
    int languageID;
    java.util.Date createdOn;
    Author[] authors;

    public Book() {
        this.authors = new Author[0];
        this.subtitle = null;
    }

    public Book(int ID, String name, String ISBN, int pageCount, String keywords, int languageID, String strCreatedOn) {
        this.ID = ID;
        this.name = name;
        this.subtitle = null;
        this.ISBN = ISBN;
        this.pageCount = pageCount;
        this.keywords = keywords;
        this.languageID = languageID;

        SimpleDateFormat formatter = new SimpleDateFormat("dd.mm.yyyy HH:mm:ss");
        try {
            this.createdOn = formatter.parse(strCreatedOn);
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }

    public void setSubtitle(String subtitle) {
        this.subtitle = subtitle;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setISBN(String ISBN) {
        this.ISBN = ISBN;
    }

    public void setPageCount(int pageCount) {
        this.pageCount = pageCount;
    }

    public void setKeywords(String keywords) {
        this.keywords = keywords;
    }

    public void setLanguageID(int languageID) {
        this.languageID = languageID;
    }

    public void setCreatedOn(String createdOn) {
        SimpleDateFormat formatter = new SimpleDateFormat("dd.mm.yyyy HH:mm:ss");
        try {
            this.createdOn = formatter.parse(createdOn);
        } catch (ParseException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int getID() {
        return ID;
    }

    public void addAuthor(Author newAuthor) {
        Author[] newArray = new Author[this.authors.length + 1];
        System.arraycopy(this.authors, 0, newArray, 0, this.authors.length);
        newArray[this.authors.length] = newAuthor;
        this.authors = newArray;
    }

    public String toString(){
        return ID + " " + name + " " + subtitle + " " + ISBN + " " + pageCount + " " + keywords + " " + languageID + " " + createdOn;
    }

    @Override
    public String Publish() {

        String output = "<xml>\n";
        output = output + "\t<title>" + this.name + "</title>\n";
        if(this.subtitle.length() > 0)
            output = output + "\t<subtitle>" + this.subtitle + "</subtitle>\n";

        output = output + "\t<isbn>" + this.ISBN + "</isbn>\n";
        output = output + "\t<pageCount>" + this.pageCount + "</pageCount>\n";
        output = output + "\t<keywords>" + this.keywords + "</keywords>\n";
        output = output + "\t<languageID>" + this.languageID + "</languageID>\n";
        output = output + "\t<createdOn>" + this.createdOn + "</createdOn>\n";

        output = output + "\t<authors>";
        for(int i = 0; i < this.authors.length; i++) {
            output = output + this.authors[i].firstName + " " + this.authors[i].lastName;
            if(i < this.authors.length - 1)
                output = output + ";";
        }
        output = output + "</authors>\n";

        output = output + "</xml>";
        return output;
    }
}
