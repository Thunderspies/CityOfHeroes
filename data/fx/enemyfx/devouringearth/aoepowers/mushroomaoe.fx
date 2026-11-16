#########################################################
##	jack
##
FxInfo


Input  
	InpName	Root
End

Flags    InheritAlpha


Condition
	On 	Time
	Time 	0

	Event
		EName 	particles
		Type	posit 
		At	Root
		Part	:mushroomRingsDust.part
		Part	:MushroomRingsOut.part
		#Part	:MushroomRingsOutJagged.part
		Part	:MushroomRingsOut2.part
		Part	:MushroomglowBase.part
		Part	:MushroomglowBaseFlat.part
		Part	:MushroomglowBaseFlat2.part
	End

	Event
		EName 	rocks
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScale2.bhvr
		Geom	Mushrooms
	End

	Event
		EName 	rocks
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScale3.bhvr
		Geom	Mushroom01
	End

	Event
		EName 	rocks
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScale4.bhvr
		Geom	Mushroom02
	End

	Event
		EName 	rocks
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScale5.bhvr
		Geom	Mushroom03
	End

	Event
		EName 	rocks
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScale4.bhvr
		Geom	Mushroom04
	End

	Event
		EName 	rocks
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScale4.bhvr
		Geom	Mushroom05
	End

End

End			