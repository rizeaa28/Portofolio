package com.company;

public class PublishingRetailer {
    int ID;
    String name;
    IPublishingArtifact[] publishingArtifacts;
    Country[] countries;

    public PublishingRetailer(int ID, String name) {
        this.ID = ID;
        this.name = name;
        this.publishingArtifacts = new IPublishingArtifact[0];
        this.countries = new Country[0];
    }

    public int getID() {
        return ID;
    }

    public void addCountry(Country newCountry) {
        Country[] newArray = new Country[this.countries.length + 1];
        System.arraycopy(this.countries, 0, newArray, 0, this.countries.length);
        newArray[newArray.length - 1] = newCountry;
        this.countries = newArray;
    }

    public void addPublishingArtifact(IPublishingArtifact newPublishingArtifact) {
        IPublishingArtifact[] newArray = new IPublishingArtifact[this.publishingArtifacts.length + 1];
        System.arraycopy(this.publishingArtifacts, 0, newArray, 0, this.publishingArtifacts.length);
        newArray[newArray.length - 1] = newPublishingArtifact;
        this.publishingArtifacts = newArray;
    }

    @Override
    public String toString() {
        return "PublishingRetailer{" +
                "ID=" + ID +
                ", name='" + name + '\'' +
                '}';
    }
}
