#########################################################
##	iceweapon
########################################################
FxInfo

LifeSpan	65

################################################################################################


Condition
	On Time 
	Time 10
	
	Event
		Type 	local
		At	Origin
		Sound elec_thunderclap 70 70 .8
	End
End


Condition
	On 	Time
	Time 	15

	Event
		EName 	Thingy
		Type	local
		At	HandR
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part	:Red_ElectricityControl01White.part
		Part	:Red_ElectricityControl02white.part
		Part3	:Red_ElectricityInnerGlow.part
		
		LifeSpan	25

	End

	Event
		EName 	Thingy2
		Type	local
		At	HandL
		Part1	:Red_ElectricityControl01.part
		Part2	:Red_ElectricityControl02.part
		Part	:Red_ElectricityControl01White.part
		Part	:Red_ElectricityControl02white.part
		Part3	:Red_ElectricityInnerGlow.part
		
		LifeSpan	25
	End


	Event
		EName 	Thingy3
		Type	local
		At	HandR
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
		LifeSpan	25
	End

	Event
		EName 	Thingy4
		Type	local
		At	HandL
		Part1	:Red_ElectricityOuterGlow.part
		Part2	:Red_ElectricityArch.part
		Part3	:Red_ElectricityRing.part
		Part4	:Red_ElectricityArchInvert.part
		
		LifeSpan	25
	End
		

End

##########################################################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Mallet
		Type	start 
		At	mystic
		Geom	RadiationShield
		bhvr	behaviors/RedScaleBubble2.bhvr
		
		LifeSpan	15
		
	End

	Event
		EName 	Mallet2
		Type	start 
		At	mystic
		
#		Part1	:Red_ElectricityMeleeHitEmber.part
		Part2	:Red_ElectricityMeleeHitStar.part
		Part3	:Red_ElectricityMeleeHitRing.part
		Part4	:Red_ElectricityMeleeHitRing2.part
		Part5	:Red_ElectricityMeleeHitGlows.part
		Part	:Red_ElectricityMeleeHitGlowsWhite.part
		
		LifeSpan	10
		Sound elecmeleehit 100.0 100.0 .8
	End

	Event
		EName 	shock
		Type	start 
		At	mystic
		
		Part1	:Red_ElectricityArchContinuinga.part
		Part2	:Red_ElectricityArchContinuing2a.part
		Part2	:Red_ElectricityArchContinuingaWhite.part
		
		LifeSpan	10
	End

	Event
		EName 	shock2
		Type	start 
		At	mystic
		
		Part	:Red_ElectricityArchLarge2WhiteA.part
		Part	:Red_ElectricityArchLarge2A.part
		Part	:Red_ElectricityArchLargeA.part
		lifeSpan	13		
	End


End

####################################################################

Condition
	On 	Time
	Time 	25

	Event
		EName 	Mallet
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	35

	Event
		EName 	Mallet2
		Type	Destroy
		
	End

End

Condition
	On 	Time
	Time 	65
	Event
		EName 	Core
		Type	Destroy
		
	End

End

End
