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
		EName 	rocks
		Type	start 
		At	Root
		Bhvr	Behaviors/RockScale.bhvr
		Geom	FX_StoneWall_11
	End

	Event
		EName 	rocks
		Type	start 
		At	Root
		Bhvr	Behaviors/RockScaleSmall.bhvr
		Geom	FX_StoneWall_10
	End

	Event
		EName 	rocks
		Type	start 
		At	Root
		Bhvr	Behaviors/RockScaleSmallest.bhvr
		Geom	FX_StoneWall_03
	End

	Event
		EName 	rocks
		Type	start 
		At	Root
		Part	:Rockglow.part
		Part	:Rockglow2.part
		Part	:RockglowBase.part
		Part	:MushroomglowBaseFlat2.part
		Part	:QuakeRing.part
		Part	:QuakeRing2.part
		Part	:QuakeRingJagged.part
		Part	:QuakeRingJaggedGlow.part
		Part	:QuakeRingRough.part

	End

End

End			