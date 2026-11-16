#############################################################
## SingleLargePowerPunch.fx
#############################################################

FxInfo
Flags	InheritGeoScale

#############################################################

Condition
	On 		TRIGGERBITS
	TRIGGERBITS 	MALE

	Event
		EName	Tattoo_Male
		Type	Local
		At	Origin
		Flags	OneAtATime
		WHILE	MALE
		CHILDFX	:CHILD_ArmsAndBack_Male.fx
		Lifespan 0
	End
End

Condition
	On 		TRIGGERBITS
	TRIGGERBITS 	FEMALE

	Event
		EName	Tattoo_Female
		Type	Local
		At	Origin
		Flags	OneAtATime
		WHILE	FEMALE
		CHILDFX	:CHILD_ArmsAndBack_Female.fx
		Lifespan 0
	End
End

#############################################################

End