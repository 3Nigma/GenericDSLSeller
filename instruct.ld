- Class initialization area
create class Insurance with the following properties: basecost(8000) and evaluates with 'basecost'
create class HouseInsurance inheriting from Insurance with the following properties: nrOfRooms, areaCode and evaluates with '<Insurance> + nrOfRooms * 2.7 / areaCode'
create class MansionInsurance inheriting from HouseInsurance with the following properties: ownerIncome, nrOfLuxuryItems and evaluates with '<HouseInsurance> + ownerIncome * 0.5 + nrOfLuxuryItems * 1.3'
create class CarInsurance inheriting from Insurance with the following properties: nrOfDoors, horsePower and evaluates with '(<Insurance> * 2.7) / nrOfDoors + horsePower * 1.58'

- Object creation area
instantiate HouseInsurance in myHouseInsurance setting nrOfRooms(3), areaCode(1)
instantiate CarInsurance in myDaciaInsurance setting nrOfDoors(4), horsePower(140)