//
// Created by ambar on 17/05/24.
//

#ifndef PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H
#define PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H
#include <string>
#include <ctype.h>

// registro para un record simpleID,Time_GMT,Phone,Organization,OLF,Rating,NumberReview,Category,Country,CountryCode,State,City,Street,Building
// ID,Time_GMT,Phone,Organization,OLF,Rating,NumberReview,Category,Country,CountryCode,State,City,Street,Building
struct Registro {

    int ID;
    std::string Time_GMT;
    std::string Phone;
    std::string Organization;
    std::string OLF;
    int Rating;
    int NumberReview;
    std::string Category;
    std::string Country;
    std::string CountryCode;
    std::string State;
    std::string City;
    std::string Street;
    std::string Building;
    Registro() {}
    Registro(int id, const std::string& time_gmt, const std::string& phone, const std::string& organization, const std::string& olf,
             int rating, int numberReview, const std::string& category, const std::string& country, const std::string& countryCode,
             const std::string& state, const std::string& city, const std::string& street, const std::string& building)
            : ID(id), Time_GMT(time_gmt), Phone(phone), Organization(organization), OLF(olf), Rating(rating), NumberReview(numberReview),
              Category(category), Country(country), CountryCode(countryCode), State(state), City(city), Street(street), Building(building) {}
    void setField(const std::string& field, const std::string& value) {
        if (field == "ID") ID = std::stoi(value);
        else if (field == "Time_GMT") Time_GMT = value;
        else if (field == "Phone") Phone = value;
        else if (field == "Organization") Organization = value;
        else if (field == "OLF") OLF = value;
        else if (field == "Rating") {
            if(isdigit(stoi(value)))
                Rating = std::stoi(value);
            else
                Rating = 0;
        }
        else if (field == "NumberReview"){
            if(isdigit(stoi(value)))
                NumberReview = std::stoi(value);
            else
                NumberReview = 0;
        }
        else if (field == "Category") Category = value;
        else if (field == "Country") Country = value;
        else if (field == "CountryCode") CountryCode = value;
        else if (field == "State") State = value;
        else if (field == "City") City = value;
        else if (field == "Street") Street = value;
        else if (field == "Building") Building = value;
    }

    int getKey() const {
        return ID;
    }
};


// sin ebmargo como voy a seterale n parametros del csv, yo no se que valores tendra como datos, por ende:
// no encontre solucion a eso, por lo que sera un record fijo
#endif //PROYECTO1_BASE_DE_DATOS_II_REGISTRO_H
