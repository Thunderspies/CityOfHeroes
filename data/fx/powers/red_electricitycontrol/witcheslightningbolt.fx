#########################################################
##	forceBubble
########################################################
FxInfo


Lifespan	75

########################################################


#
######################################################################################

Condition
	On 	Time
	Time 	20


	Event
		Ename	Prime
		Type	start
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchElectricityBlastHazeSmall.part
		Bhvr	:WitchHaze.bhvr
		
		Magnet	Target
		LookAt	Target
		
	End

	Event
		Type	start
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBolt.Part
		Part	:WitchBolt2.Part
		POther Target	
		Lifespan	7
	End		
	
End

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBolt.Part
		Part	:WitchBolt2.Part
		POther Target	
		Lifespan	7
	End		
	
End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBolt.Part
		Part	:WitchBolt2.Part
		POther  Target
		Lifespan	7
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBolt.Part
		Part	:WitchBolt2.Part
		POther  Target
		Lifespan	7
	End

End

Condition
	On 	Time
	Time 	46
		
	Event

		Type	start
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part2	:WitchBolt.Part
		Part	:WitchBolt2.Part
		POther  Target
		Lifespan	7
	End

End

#############################################################################
##Hands
############################################################

Condition
	On 	Time
	Time 	10

	Event
		EName 	Thingy1
		Type	local
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part1	:ElectricityControl01Witches.part
		#Part2	:ElectricityControl02Witches.part
		#Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		LifeSpan	50
	End

	Event
		EName 	ThingyB
		Type	local
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
		LifeSpan	50
	End
End

Condition
	On 	Time
	Time 	15

	Event
		EName 	BurstL
		Type	local
		At	wepL
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:WitchSparks.part
		LifeSpan	20

	End

End

Condition
	On 	PrimeHit
	
	Event
		
		EName	Prime
		Type	destroy
		
		
	End

	Event
		
		EName	Prime1
		Type	destroy
		
		
	End

End

Condition
	On 	Time
	Time 	55
	Event
		
		EName	Thingy
		Type	destroy
		
		
	End

	Event
		
		EName	Thingy1
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyA
		Type	destroy
		
		
	End

	Event
		
		EName	ThingyB
		Type	destroy
		
		
	End

	

End

End