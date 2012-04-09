- File automatically generated. Be careful what you're touching here ...
create class Insurance with the following properties: basecost(Numerical) and evaluates with 'result = basecost'
create class HouseInsurance inherits Insurance with the following properties: clientAge(Numerical), nrOfRooms(Numerical), areaCode(Numerical) and evaluates with 'if clientAge < 18 then result = <Insurance> + nrOfRooms * 0.65 / areaCode else result = nrOfRooms * 1.3 end'
create class MansionInsurance inherits HouseInsurance with the following properties: ownerIncome(Numerical), nrOfLuxuryItems(Numerical) and evaluates with 'result = <HouseInsurance> + ownerIncome * 0.5 + nrOfLuxuryItems * 1.3'
create class CarInsurance inherits Insurance with the following properties: nrOfDoors(Numerical), horsePower(Numerical) and evaluates with 'result = (<Insurance> * 2.7) / nrOfDoors + horsePower * 1.58'
instantiate HouseInsurance in myHouseInsurance setting basecost(790), nrOfRooms(3), areaCode(1), clientAge(15)
instantiate CarInsurance in myDaciaInsurance setting nrOfDoors(4), horsePower(140)
update class Insurance adding the following properties : crisisMargin(Numerical), baseprofit(Numerical)
update class Insurance erasing the following properties : basecost
update class Insurance evaluating in : result = baseprofit + crisisMargin * 1.24
update instance myHouseInsurance setting property values : crisisMargin(1000)
update instance myHouseInsurance setting property values : clientAge(19)
