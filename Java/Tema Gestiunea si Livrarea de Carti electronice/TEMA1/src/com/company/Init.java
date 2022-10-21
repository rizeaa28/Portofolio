package com.company;

import java.io.*;
import java.util.Scanner;


public class Init {

    public Init() {
    }

    public Book[] initBooks() {

        Book[] booksArray = new Book[1];

        try (BufferedReader br = new BufferedReader(new FileReader("books.in"))) {
            String line;

            int i = 0;
            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###"); //set delimiter for scanner

                //constructs the new book object with set methods and no-arg constructor
                booksArray[i] = new Book();

                booksArray[i].setID(s.nextInt());
                booksArray[i].setName(s.next());

                if(s.hasNextLong() != true)
                    booksArray[i].setSubtitle(s.next());

                booksArray[i].setISBN(Long.toString(s.nextLong()));
                booksArray[i].setPageCount(s.nextInt());
                booksArray[i].setKeywords(s.next());
                booksArray[i].setLanguageID(s.nextInt());
                booksArray[i].setCreatedOn(s.next());

                //adds it to the array
                Book[] extendedBookArray = new Book[i + 2];
                System.arraycopy(booksArray, 0, extendedBookArray, 0, booksArray.length);
                booksArray = extendedBookArray;
                i++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return booksArray;
    }

    public Language[] initLanguages(){

        Language[] languagesArray = new Language[1];
        try (BufferedReader br = new BufferedReader(new FileReader("languages.in"))) {
            String line;

            int i = 0;
            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                //constructs the new language object
                languagesArray[i] = new Language(s.nextInt(), s.next(), s.next());

                //adds it to array
                Language[] extendedLanguageArray = new Language[i+2];
                System.arraycopy(languagesArray, 0, extendedLanguageArray, 0, languagesArray.length);
                languagesArray = extendedLanguageArray;
                i++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        return languagesArray;
    }

    public Author[] initAuthor() {

        Author[] authorsArray = new Author[1];
        try (BufferedReader br = new BufferedReader(new FileReader("authors.in"))) {
            String line;

            int i = 0;
            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");

                //constructs the new author object
                authorsArray[i] = new Author(s.nextInt(), s.next(), s.next());

                //adds it to array
                Author[] extendedAuthorArray = new Author[i + 2];
                System.arraycopy(authorsArray, 0, extendedAuthorArray, 0, authorsArray.length);
                authorsArray = extendedAuthorArray;
                i++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return authorsArray;
    }

    public void initBooksAuthors(Book[] books, Author[] authors) {
        try (BufferedReader br = new BufferedReader(new FileReader("books-authors.in"))) {
            String line;

            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                int bookID = s.nextInt();
                int authorID = s.nextInt();
                Author newAuthor = new Author();

                //find the author with the id
                for(int i = 0; i < authors.length; i++) {
                    if(authors[i].getID() == authorID) {
                        newAuthor = authors[i];
                        break;
                    }
                }

                //add the author to the array in the book object
                for(int i = 0; i < books.length; i++) {
                    if(books[i].getID() == bookID) {
                        books[i].addAuthor(newAuthor);
                        break;
                    }
                }
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public EditorialGroup[] initEditorialGroup() {

        EditorialGroup[] editorialGroupsArray = new EditorialGroup[1];
        try (BufferedReader br = new BufferedReader(new FileReader("editorial-groups.in"))) {
            String line;

            int i = 0;
            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");

                //create new object
                editorialGroupsArray[i] = new EditorialGroup(s.nextInt(), s.next());

                //add it to the array
                EditorialGroup[] extendedEditorialGroupsArray = new EditorialGroup[i + 2];
                System.arraycopy(editorialGroupsArray, 0, extendedEditorialGroupsArray, 0, editorialGroupsArray.length);
                editorialGroupsArray = extendedEditorialGroupsArray;
                i++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return editorialGroupsArray;
    }

    public PublishingBrand[] initPublishingBrand() {

        PublishingBrand[] publishingBrandsArray = new PublishingBrand[1];
        try (BufferedReader br = new BufferedReader(new FileReader("publishing-brands.in"))) {
            String line;

            int i = 0;
            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");

                //create new object
                publishingBrandsArray[i] = new PublishingBrand(s.nextInt(), s.next());

                //add it to the array
                PublishingBrand[] extendedPublishingBrandsArray = new PublishingBrand[i + 2];
                System.arraycopy(publishingBrandsArray, 0, extendedPublishingBrandsArray, 0, publishingBrandsArray.length);
                publishingBrandsArray = extendedPublishingBrandsArray;
                i++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return publishingBrandsArray;
    }

    public void initEditorialBooks(Book[] books, EditorialGroup[] editorialGroups) {
        try (BufferedReader br = new BufferedReader(new FileReader("editorial-groups-books.in"))) {
            String line;

            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                int editorialGroupID = s.nextInt();
                int bookID = s.nextInt();
                Book newBook = new Book();

                //find the book with the ID
                for(int i = 0; i < books.length; i++) {
                    if(books[i].getID() == bookID) {
                        newBook = books[i];
                        break;
                    }
                }

                //add it to coresponding editorialGroup
                for(int i = 0; i < editorialGroups.length - 1; i++) {
                    if(editorialGroups[i].getID() == editorialGroupID) {
                        editorialGroups[i].addBook(newBook);
                        break;
                    }
                }
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void initPublishingBooks(Book[] books, PublishingBrand[] publishingBrands) {
        try (BufferedReader br = new BufferedReader(new FileReader("publishing-brands-books.in"))) {
            String line;

            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                int publishingBrandID = s.nextInt();
                int bookID = s.nextInt();
                Book newBook = new Book();

                //find the book with the ID
                for(int i = 0; i < books.length; i++) {
                    if(books[i].getID() == bookID) {
                        newBook = books[i];
                        break;
                    }
                }

                //add it to the corresponding publishingBrand
                for(int i = 0; i < publishingBrands.length - 1; i++) {
                    if(publishingBrands[i].getID() == publishingBrandID) {
                        publishingBrands[i].addBook(newBook);
                        break;
                    }
                }
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public PublishingRetailer[] initPublishingRetailer() {

        PublishingRetailer[] publishingRetailerArray = new PublishingRetailer[1];
        try (BufferedReader br = new BufferedReader(new FileReader("publishing-retailers.in"))) {
            String line;

            int i = 0;
            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");

                //create new object
                publishingRetailerArray[i] = new PublishingRetailer(s.nextInt(), s.next());

                //add it to array
                PublishingRetailer[] extendedPublishingRetailersArray = new PublishingRetailer[i + 2];
                System.arraycopy(publishingRetailerArray, 0, extendedPublishingRetailersArray, 0, publishingRetailerArray.length);
                publishingRetailerArray = extendedPublishingRetailersArray;
                i++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return publishingRetailerArray;
    }

    public Country[] initCountry() {

        Country[] countryArray = new Country[1];
        try (BufferedReader br = new BufferedReader(new FileReader("countries.in"))) {
            String line;

            int i = 0;
            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");

                //create new object
                countryArray[i] = new Country(s.nextInt(), s.next());

                //add it to array
                Country[] extendedCountriesArray = new Country[i + 2];
                System.arraycopy(countryArray, 0, extendedCountriesArray, 0, countryArray.length);
                countryArray = extendedCountriesArray;
                i++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return countryArray;
    }

    public void initPublishingCountries(Country[] countries, PublishingRetailer[] publishingRetailers) {
        try (BufferedReader br = new BufferedReader(new FileReader("publishing-retailers-countries.in"))) {
            String line;

            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                int publishingRetailerID = s.nextInt();
                int countryID = s.nextInt();
                Country newCountry = new Country();

                //find the country with the ID
                for(int i = 0; i < countries.length; i++) {
                    if(countries[i].getID() == countryID) {
                        newCountry = countries[i];
                        break;
                    }
                }

                //add it to the array of the corresponding retailer
                for(int i = 0; i < publishingRetailers.length - 1; i++) {
                    if(publishingRetailers[i].getID() == publishingRetailerID) {
                        publishingRetailers[i].addCountry(newCountry);
                        break;
                    }
                }
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void initPublishingRetailerBooks(Book[] books, PublishingRetailer[] publishingRetailers) {
        try (BufferedReader br = new BufferedReader(new FileReader("publishing-retailers-books.in"))) {
            String line;

            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                int publishingRetailerID = s.nextInt();
                int bookID = s.nextInt();
                Book newBook = new Book();

                //find the book with the ID
                for(int i = 0; i < books.length; i++) {
                    if(books[i].getID() == bookID) {
                        newBook = books[i];
                        break;
                    }
                }

                //add it to the publishing artifacts array in the corresponding retailer
                for(int i = 0; i < publishingRetailers.length - 1; i++) {
                    if(publishingRetailers[i].getID() == publishingRetailerID) {
                        publishingRetailers[i].addPublishingArtifact(newBook);
                        break;
                    }
                }
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void initPublishingRetailerEditorialGroups(EditorialGroup[] editorialGroups, PublishingRetailer[] publishingRetailers) {
        try (BufferedReader br = new BufferedReader(new FileReader("publishing-retailers-editorial-groups.in"))) {
            String line;

            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                int publishingRetailerID = s.nextInt();
                int editorialGroupID = s.nextInt();
                EditorialGroup newEditorialGroup = new EditorialGroup();

                //find the editorial group with the ID
                for(int i = 0; i < editorialGroups.length; i++) {
                    if(editorialGroups[i].getID() == editorialGroupID) {
                        newEditorialGroup = editorialGroups[i];
                        break;
                    }
                }

                //add it to the publishing artifacts array in the corresponding retailer
                for(int i = 0; i < publishingRetailers.length - 1; i++) {
                    if(publishingRetailers[i].getID() == publishingRetailerID) {
                        publishingRetailers[i].addPublishingArtifact(newEditorialGroup);
                        break;
                    }
                }
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void initPublishingRetailerPublishingBrands(PublishingBrand[] publishingBrands, PublishingRetailer[] publishingRetailers) {
        try (BufferedReader br = new BufferedReader(new FileReader("publishing-retailers-publishing-brands.in"))) {
            String line;

            line = br.readLine();
            while ((line = br.readLine()) != null) {
                Scanner s = new Scanner(line).useDelimiter("###");
                int publishingRetailerID = s.nextInt();
                int publishingBrandID = s.nextInt();
                PublishingBrand newPublishingBrand = new PublishingBrand();

                //find the publishing brand with the ID
                for(int i = 0; i < publishingBrands.length; i++) {
                    if(publishingBrands[i].getID() == publishingBrandID) {
                        newPublishingBrand = publishingBrands[i];
                        break;
                    }
                }

                //add it to the publishing artifacts array in the corresponding retailer
                for(int i = 0; i < publishingRetailers.length - 1; i++) {
                    if(publishingRetailers[i].getID() == publishingRetailerID) {
                        publishingRetailers[i].addPublishingArtifact(newPublishingBrand);
                        break;
                    }
                }
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

}
