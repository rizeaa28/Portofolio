package com.company;

public class EditorialGroup implements  IPublishingArtifact{
    int ID;
    String name;
    Book[] books;

    public EditorialGroup() {
    }

    public EditorialGroup(int ID, String name) {
        this.ID = ID;
        this.name = name;
        this.books = new Book[0];
    }

    @Override
    public int getID() {
        return ID;
    }

    public void addBook(Book newBook) {
        Book[] newArray = new Book[this.books.length + 1];
        System.arraycopy(this.books, 0, newArray, 0, this.books.length);
        newArray[newArray.length - 1] = newBook;
        this.books = newArray;
    }

    @Override
    public String toString() {
        return "EditorialGroup{" +
                "ID=" + ID +
                ", name='" + name + '\'' +
                '}';
    }

    @Override
    public String Publish() {
        String output = "<xml\n";
        output = output + "\t<editorialGroup>\n";
        output = output + "\t\t<ID>" + this.ID + "</ID>\n";
        output = output + "\t\t<Name>" + this.name + "</Name>\n";
        output = output + "\t</editorialGroup>\n";

        output = output + "\t<books>\n";

        for(int i = 0; i < this.books.length; i++) {
            output = output + "\t\t<book>\n";
            output = output + "\t\t\t<title>" + this.books[i].name + "</title>\n";
            if(this.books[i].subtitle.length() > 0)
                output = output + "\t\t\t<subtitle>" + this.books[i].subtitle + "</subtitle>\n";

            output = output + "\t\t\t<isbn>" + this.books[i].ISBN + "</isbn>\n";
            output = output + "\t\t\t<pageCount>" + this.books[i].pageCount + "</pageCount>\n";
            output = output + "\t\t\t<keywords>" + this.books[i].keywords + "</keywords>\n";
            output = output + "\t\t\t<languageID>" + this.books[i].languageID + "</languageID>\n";
            output = output + "\t\t\t<createdOn>" + this.books[i].createdOn + "</createdOn>\n";

            output = output + "\t\t\t<authors>";
            for(int k = 0; k < this.books[i].authors.length; k++) {
                output = output + this.books[i].authors[k].firstName + " " + this.books[i].authors[k].lastName;
                if(k < this.books[i].authors.length - 1)
                    output = output + ";";
            }
            output = output + "</authors>\n";
            output = output + "\t\t</book>\n";
        }

        output = output + "\t</books>\n";
        output = output + "</xml>";
        return output;
    }
}
