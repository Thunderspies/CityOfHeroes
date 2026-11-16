#############################################################
## Red_ElectricalMeleeHandClapHit.fx
#############################################################

FxInfo
LifeSpan	60

#############################################################

Input
	Inpname	Hips
End

#############################################################


Condition
	On 	Time
	Time 	0


	Event
		EName 	Mallet2
		Type	local
		At	Chest

#		Part1	:Red_ElectricityMeleeHitEmber.part
		Part2	:Red_ElectricityMeleeHitStar.part
		Part3	:Red_ElectricityMeleeHitRing.part
		Part4	:Red_ElectricityMeleeHitRing2.part
		Part5	:Red_ElectricityMeleeHitGlows.part
		Part	:Red_ElectricityMeleeHitGlowsWhite.part

		Sound elecmeleehit 100.0 100.0 .8
	End

	Event
		EName 	shock
		Type	local
		At	Chest

		Part1	:Red_ElectricityArchContinuinga.part
		Part2	:Red_ElectricityArchContinuing2a.part
		Part2	:Red_ElectricityArchContinuingaWhite.part

	End

	Event
		EName 	shock2
		Type	local
		At	Chest

		Part	:Red_ElectricityArchLarge2White.part
		Part	:Red_ElectricityArchLarge2.part
		Part	:Red_ElectricityArchLarge.part
		lifeSpan	10
	End

#############################################################################
##Head dizzy
########################################################

	Event
		EName	core
		Type	Local
		At	head
		Geom	LocalPivot01
		Part1	:Red_HeadDizzies.part
		Part2	:Red_HeadDizzyRings.part
#		part3	:Red_ElectricityMeleeGlowHit2.part
	End

End

####################################################################

Condition
	On 	Time
	Time 	6

	Event
		EName 	Mallet
		Type	Destroy

	End

	Event
		EName 	Mallet2
		Type	Destroy

	End

End

Condition
	On 	Time
	Time 	36

	Event
		EName 	Core
		Type	Destroy

	End

End

End
