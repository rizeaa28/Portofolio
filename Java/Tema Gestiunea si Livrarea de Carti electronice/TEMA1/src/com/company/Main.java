package com.company;

import java.io.File;
import java.sql.SQLOutput;

public class Main {

    public static void main(String[] args) {

        Init doInit = new Init();

        //Initialize inputs
        Language[] languagess = doInit.initLanguages();
        Book[] bookss = doInit.initBooks();
        Author[] authors = doInit.initAuthor();
        EditorialGroup[] editorialGroups = doInit.initEditorialGroup();
        PublishingBrand[] publishingBrands = doInit.initPublishingBrand();
        PublishingRetailer[] publishingRetailers = doInit.initPublishingRetailer();
        Country[] countries = doInit.initCountry();

        //Initialize connection between inputs
        doInit.initBooksAuthors(bookss, authors);
        doInit.initEditorialBooks(bookss, editorialGroups);
        doInit.initPublishingBooks(bookss, publishingBrands);
        doInit.initPublishingCountries(countries, publishingRetailers);
        doInit.initPublishingRetailerBooks(bookss, publishingRetailers);
        doInit.initPublishingRetailerEditorialGroups(editorialGroups, publishingRetailers);
        doInit.initPublishingRetailerPublishingBrands(publishingBrands, publishingRetailers);

        //Create an Administration object to test the methods
        Administration testAdm = new Administration(bookss, authors, languagess, editorialGroups, publishingBrands, publishingRetailers, countries);

        //Tests for getBooksForPublishingRetailerID
        System.out.println("=================TESTING FIRST METHOD=================\n");

        Book[] listTest1 = testAdm.getBooksForPublishingRetailerID(1);
        System.out.println("===== Test 1 - Books for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest1.length; k++) {
            System.out.println((listTest1[k].toString()));
        }

        listTest1 = testAdm.getBooksForPublishingRetailerID(2);
        System.out.println("\n\n===== Test 2 - Books for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest1.length; k++) {
            System.out.println((listTest1[k].toString()));
        }

        listTest1 = testAdm.getBooksForPublishingRetailerID(3);
        System.out.println("\n\n===== Test 3 - Books for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest1.length; k++) {
            System.out.println((listTest1[k].toString()));
        }

        listTest1 = testAdm.getBooksForPublishingRetailerID(4);
        System.out.println("\n\n===== Test 4 - Books for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest1.length; k++) {
            System.out.println((listTest1[k].toString()));
        }

        listTest1 = testAdm.getBooksForPublishingRetailerID(5);
        System.out.println("\n\n===== Test 5 - Books for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest1.length; k++) {
            System.out.println((listTest1[k].toString()));
        }

        //Tests for getLanguagesForPublishingRetailerID
        System.out.println("\n\n=================TESTING SECOND METHOD=================\n");

        Language[] listTest2 = testAdm.getLanguagesForPublishingRetailerID(1);
        System.out.println("\n\n===== Test 1 - Languages for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest2.length; k++) {
            System.out.println((listTest2[k].toString()));
        }

        listTest2 = testAdm.getLanguagesForPublishingRetailerID(2);
        System.out.println("\n\n===== Test 2 - Languages for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest2.length; k++) {
            System.out.println((listTest2[k].toString()));
        }

        listTest2 = testAdm.getLanguagesForPublishingRetailerID(3);
        System.out.println("\n\n===== Test 3 - Languages for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest2.length; k++) {
            System.out.println((listTest2[k].toString()));
        }

        listTest2 = testAdm.getLanguagesForPublishingRetailerID(4);
        System.out.println("\n\n===== Test 4 - Languages for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest2.length; k++) {
            System.out.println((listTest2[k].toString()));
        }

        listTest2 = testAdm.getLanguagesForPublishingRetailerID(5);
        System.out.println("\n\n===== Test 5 - Languages for Publishing Retailer ID ======\n");
        for(int k = 0; k < listTest2.length; k++) {
            System.out.println((listTest2[k].toString()));
        }

        //Tests for getCountriesForBookID
        System.out.println("\n\n=================TESTING THIRD METHOD=================\n");

        Country[] listTest3 = testAdm.getCountriesForBookID(11633);
        System.out.println("\n\n===== Test 1 - Countries for Book ID ======\n");
        for(int k = 0; k < listTest3.length; k++) {
            System.out.println((listTest3[k].toString()));
        }

        listTest3 = testAdm.getCountriesForBookID(591);
        System.out.println("\n\n===== Test 2 - Countries for Book ID ======\n");
        for(int k = 0; k < listTest3.length; k++) {
            System.out.println((listTest3[k].toString()));
        }

        listTest3 = testAdm.getCountriesForBookID(839);
        System.out.println("\n\n===== Test 3 - Countries for Book ID ======\n");
        for(int k = 0; k < listTest3.length; k++) {
            System.out.println((listTest3[k].toString()));
        }

        listTest3 = testAdm.getCountriesForBookID(3872);
        System.out.println("\n\n===== Test 4 - Countries for Book ID ======\n");
        for(int k = 0; k < listTest3.length; k++) {
            System.out.println((listTest3[k].toString()));
        }

        listTest3 = testAdm.getCountriesForBookID(7900);
        System.out.println("\n\n===== Test 5 - Countries for Book ID ======\n");
        for(int k = 0; k < listTest3.length; k++) {
            System.out.println((listTest3[k].toString()));
        }

        //Tests for getCommonBooksForRetailerIDs
        System.out.println("\n\n=================TESTING FOURTH METHOD=================\n");

        Book[] listTest4 = testAdm.getCommonBooksForRetailerIDs(4, 5);
        System.out.println("\n\n===== Test 1 - Common Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest4.length; k++) {
            System.out.println((listTest4[k].toString()));
        }

        listTest4 = testAdm.getCommonBooksForRetailerIDs(2, 7);
        System.out.println("\n\n===== Test 2 - Common Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest4.length; k++) {
            System.out.println((listTest4[k].toString()));
        }

        listTest4 = testAdm.getCommonBooksForRetailerIDs(1, 3);
        System.out.println("\n\n===== Test 3 - Common Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest4.length; k++) {
            System.out.println((listTest4[k].toString()));
        }

        listTest4 = testAdm.getCommonBooksForRetailerIDs(9, 10);
        System.out.println("\n\n===== Test 4 - Common Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest4.length; k++) {
            System.out.println((listTest4[k].toString()));
        }

        listTest4 = testAdm.getCommonBooksForRetailerIDs(6, 1);
        System.out.println("\n\n===== Test 5 - Common Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest4.length; k++) {
            System.out.println((listTest4[k].toString()));
        }

        //Tests for getAllBooksForRetailerIDs
        System.out.println("\n\n=================TESTING FIFTH METHOD=================\n");

        Book[] listTest5 = testAdm.getAllBooksForRetailerIDs(4, 5);
        System.out.println("\n\n===== Test 1 - All Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest5.length; k++) {
            System.out.println((listTest5[k].toString()));
        }

        listTest5 = testAdm.getAllBooksForRetailerIDs(1, 9);
        System.out.println("\n\n===== Test 2 - All Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest5.length; k++) {
            System.out.println((listTest5[k].toString()));
        }

        listTest5 = testAdm.getAllBooksForRetailerIDs(5, 6);
        System.out.println("\n\n===== Test 3 - All Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest5.length; k++) {
            System.out.println((listTest5[k].toString()));
        }

        listTest5 = testAdm.getAllBooksForRetailerIDs(8, 2);
        System.out.println("\n\n===== Test 4 - All Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest5.length; k++) {
            System.out.println((listTest5[k].toString()));
        }

        listTest5 = testAdm.getAllBooksForRetailerIDs(3, 8);
        System.out.println("\n\n===== Test 5 - All Books for Retailer IDs ======\n");
        for(int k = 0; k < listTest5.length; k++) {
            System.out.println((listTest5[k].toString()));
        }

    }
}
