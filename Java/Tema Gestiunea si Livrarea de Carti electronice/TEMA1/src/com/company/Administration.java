package com.company;

public class Administration {
    Book[] books;
    Author[] authors;
    Language[] languages;
    EditorialGroup[] editorialGroups;
    PublishingBrand[] publishingBrands;
    PublishingRetailer[] publishingRetailers;
    Country[] countries;

    public Administration(Book[] books, Author[] authors, Language[] languages, EditorialGroup[] editorialGroups, PublishingBrand[] publishingBrands, PublishingRetailer[] publishingRetailers, Country[] countries) {
        this.books = books;
        this.authors = authors;
        this.languages = languages;
        this.editorialGroups = editorialGroups;
        this.publishingBrands = publishingBrands;
        this.publishingRetailers = publishingRetailers;
        this.countries = countries;
    }

    public int checkIfBookRepeats(Book[] books, int bookID) {

        for(int i = 0; i < books.length; i++) {
            if(books[i].getID() == bookID) //checks if the element of the array is the book
                return 1;
        }
        return 0;
    }

    public int checkIfLanguageRepeats(Language[] languages, int languageID) {

        for(int i = 0; i < languages.length; i++) {
            if(languages[i].getID() == languageID) //checks if the element of the array is the language
                return 1;
        }
        return 0;
    }

    public int checkIfCountryRepeats(Country[] countries, int countryID) {

        for(int i = 0; i < countries.length; i++) {
            if(countries[i].getID() == countryID) //checks if the element of the array is the country
                return 1;
        }
        return 0;
    }

    public Book[] addBook(Book[] listOfBooks, int newBookID) {

        Book newBook = null;
        for(int i = 0; i < books.length; i++) {
            if(books[i].getID() == newBookID) { //if the book from the array of objects is the needed one
                newBook = books[i]; //it assigns it to the newBook refference
                break;
            }
        }

        //then it adds it to the array if it has found a book with that ID
        if(newBook != null) {
            Book[] newArray = new Book[listOfBooks.length + 1]; //it makes a new array with one element longer
            System.arraycopy(listOfBooks, 0, newArray, 0, listOfBooks.length); //copies the old array into the new one
            newArray[newArray.length - 1] = newBook; //adds the new book to the array
            return newArray; //return the new array
        }
        else
            return listOfBooks;
    }

    //same as addBook but with languages
    public Language[] addLanguage(Language[] listOfLanguages, int newLanguageID) {

        Language newLanguage = null;
        for(int i = 0; i < languages.length; i++) {
            if(languages[i].getID() == newLanguageID) {
                newLanguage = languages[i];
                break;
            }
        }

        if(newLanguage != null) {
            Language[] newArray = new Language[listOfLanguages.length + 1];
            System.arraycopy(listOfLanguages, 0, newArray, 0, listOfLanguages.length);
            newArray[newArray.length - 1] = newLanguage;
            return newArray;
        }
        else
            return listOfLanguages;
    }

    //same as addBook but with countries
    public Country[] addCountry(Country[] listOfCountries, int newCountryID) {

        Country newCountry = null;
        for(int i = 0; i < countries.length; i++) {
            if(countries[i].getID() == newCountryID) {
                newCountry = countries[i];
                break;
            }
        }

        if(newCountry != null) {
            Country[] newArray = new Country[listOfCountries.length + 1];
            System.arraycopy(listOfCountries, 0, newArray, 0, listOfCountries.length);
            newArray[newArray.length - 1] = newCountry;
            return newArray;
        }
        else
            return listOfCountries;
    }

    public Book[] getBooksForPublishingRetailerID(int publishingRetailerID) {

        Book[] listOfBooks = new Book[0];
        PublishingRetailer current = null;

        for(int i = 0; i < publishingRetailers.length - 1; i++) { //iterates to find the publishingRetailer object with specified ID
            if(publishingRetailers[i].getID() == publishingRetailerID) {
                current = publishingRetailers[i];
                break;
            }
        }

        for(int i = 0; i < current.publishingArtifacts.length; i++) {
            Class c = current.publishingArtifacts[i].getClass(); //gets the name of the class that is a PublishingArtifact

            if(c.getName() == "com.company.Book") { //if the object is a book it adds it to the list if it doesnt repeat

                if (checkIfBookRepeats(listOfBooks, current.publishingArtifacts[i].getID()) == 0) {
                    listOfBooks = addBook(listOfBooks, current.publishingArtifacts[i].getID());
                }
            }
            if(c.getName() == "com.company.EditorialGroup") { //if the object is an editorialGroup then it iterates through its books array and adds it to the list if it doesnt repeat
                EditorialGroup currentEditorialGroup = null;
                for(int j = 0; j < editorialGroups.length; j++) {
                    if(editorialGroups[j].getID() == current.publishingArtifacts[i].getID()) {
                        currentEditorialGroup = editorialGroups[j];
                        break;
                    }
                }

                for(int k = 0; k < currentEditorialGroup.books.length; k++) {
                    if(checkIfBookRepeats(listOfBooks, currentEditorialGroup.books[k].getID()) ==0) {
                        listOfBooks = addBook(listOfBooks, currentEditorialGroup.books[k].getID());
                    }
                }
            }
            if(c.getName() == "com.company.PublishingBrand") { //same as editorialGroup
                PublishingBrand currentPublishingBrand = null;
                for(int j = 0; j < publishingBrands.length; j++) {
                    if(publishingBrands[j].getID() == current.publishingArtifacts[i].getID()) {
                        currentPublishingBrand = publishingBrands[j];
                        break;
                    }
                }

                for(int k = 0; k < currentPublishingBrand.books.length; k++) {
                    if(checkIfBookRepeats(listOfBooks, currentPublishingBrand.books[k].getID()) ==0) {
                        listOfBooks = addBook(listOfBooks, currentPublishingBrand.books[k].getID());
                    }
                }
            }

        }
        return listOfBooks;
    }

    public Language[] getLanguagesForPublishingRetailerID(int publishingRetailerID) {
        Book[] listOfBooks = getBooksForPublishingRetailerID(publishingRetailerID); //gets the books array for specified publishingRetailerID
        Language[] listOfLanguages = new Language[0];

        for(int i = 0; i < listOfBooks.length; i++) {
            if(checkIfLanguageRepeats(listOfLanguages, listOfBooks[i].languageID) == 0) {//check if the book's language repeats, if nbot it adds it
                listOfLanguages = addLanguage(listOfLanguages, listOfBooks[i].languageID);
            }
        }
        return listOfLanguages;
    }

    public Country[] getCountriesForBookID(int bookID) {
        Country[] listOfCountries = new Country[0];
        Book[] listOfBooks = null;

        for(int i = 0; i < publishingRetailers.length - 1; i++) { //iterates through publishingRetailers
            listOfBooks = getBooksForPublishingRetailerID(publishingRetailers[i].getID()); //gets their list of books
            if(checkIfBookRepeats(listOfBooks, bookID) == 1) { //if the book specified in the parameter is in this array
                for(int k = 0; k < publishingRetailers[i].countries.length; k++) { //then add the coresponding publishing Retailer's countries to the list
                    if(checkIfCountryRepeats(listOfCountries, publishingRetailers[i].countries[k].getID()) == 0)
                        listOfCountries = addCountry(listOfCountries, publishingRetailers[i].countries[k].getID());
                }
            }
        }
        return listOfCountries;
    }

    public Book[] getCommonBooksForRetailerIDs(int retailerID1, int retailerID2) {
        Book[] listOfBooksRetailer1 = getBooksForPublishingRetailerID(retailerID1); //get books for retailer 1
        Book[] listOfBooksRetailer2 = getBooksForPublishingRetailerID(retailerID2); //get books for retailer 2
        Book[] listOfBooks = new Book[0];

        for(int i = 0; i < listOfBooksRetailer1.length; i++) { //iterates through the first list
            if(checkIfBookRepeats(listOfBooksRetailer2, listOfBooksRetailer1[i].getID()) == 1) //if the book exists in the second one as well it puts it in the returned list
                if(checkIfBookRepeats(listOfBooks, listOfBooksRetailer1[i].getID()) == 0)
                    listOfBooks = addBook(listOfBooks, listOfBooksRetailer1[i].getID());
        }
        return listOfBooks;
    }

    public Book[] getAllBooksForRetailerIDs (int retailerID1, int retailerID2) {
        Book[] listOfBooksRetailer1 = getBooksForPublishingRetailerID(retailerID1); //get books for retailer 1
        Book[] listOfBooks = getBooksForPublishingRetailerID(retailerID2); //get books for retailer 2 and put it in the returned list

        for(int i = 0; i < listOfBooksRetailer1.length; i++) { //iterates thorugh the first list and if the book isnt already in the returning list it adds it
            if(checkIfBookRepeats(listOfBooks, listOfBooksRetailer1[i].getID()) == 0)
                listOfBooks = addBook(listOfBooks, listOfBooksRetailer1[i].getID());
        }
        return listOfBooks;
    }
}
