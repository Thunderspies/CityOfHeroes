#########################################################
##	jack
##
FxInfo


Input  
	InpName	Root
End

Input  
	InpName	Origin
End

Flags    InheritAlpha

Condition
	On 	Time
	Time 	0

	Event
		EName	Glow
		Type	posit
		At	Root
		part	:Plantglow.part
		part	:Plantglow2.part

	End

End

Condition
	On 	Time
	Time 	10

	Event
		EName 	rocks
		Type	posit 
		At	Root
		Bhvr	Behaviors/RockScale2.bhvr
		Geom	FX_Plants
	End

End
#################################################################
##Core splat	
####################################


Condition
	On 	cycle
	Time 	10
#	Chance	.6

	Event
		Type	posit 
		At	Root
		
		bhvr	:PlantCoreSplat.bhvr

		Splat	ember01.tga

		LifeSpan	30

	End

End

#######################################################################
##Rings
#####################################33

Condition

	Event
		Type	posit 
		At	Root
		
		bhvr	:PlantExpandingRingSplat.bhvr

		Splat	Generic_Ring.tga

		LifeSpan	40

	End

End

Condition
	On 	cycle
	Time 	70
#	Chance	.6

	Event
		Type	posit 
		At	Root
		
		bhvr	:PlantExpandingRingSplat.bhvr

		Splat	Generic_Ring.tga

		LifeSpan	40

	End

End

##################################################################

Condition

	Event
		Type	posit 
		At	Root
		
		bhvr	:PlantExpandingRingSplat2.bhvr

		Splat	PlantRing.tga

		LifeSpan	10

	End

End

Condition
	On 	cycle
	Time 	40
#	Chance	.6

	Event
		Type	posit 
		At	Root
		
		bhvr	:PlantExpandingRingSplat2.bhvr

		Splat	PlantRing.tga

		LifeSpan	10

	End

End
####################################################################

Condition

	Event
		Type	posit 
		At	Root
		
		bhvr	:PlantExpandingRingSplat3.bhvr

		Splat	QuakeRingJagged.tga

		LifeSpan	40

	End

End

Condition
	On 	cycle
	Time 	5
	Chance	.6

	Event
		Type	posit 
		At	Root
		
		bhvr	:PlantExpandingRingSplat3.bhvr

		Splat	QuakeRingJagged.tga

		LifeSpan	40

	End

End
#
#Condition
#	On 	cycle
#	Time 	9
##	Chance	.4
#
#	Event
#		Type	posit 
#		At	Root
#		
#		bhvr	:PlantExpandingRingSplatSmall.bhvr
#
#		Splat	QuakeRingJagged.tga
#
#		LifeSpan	25
#
##		Part	:CrystalglowFlat.part
#	End
#
#End


End			