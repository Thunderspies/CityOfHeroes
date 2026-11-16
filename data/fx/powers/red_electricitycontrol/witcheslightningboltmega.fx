#########################################################
##	forceBubble
########################################################
FxInfo


Lifespan	75

########################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	Prime
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchElectricityBlastHaze.part
		Bhvr	:WitchHaze.bhvr
		
		Magnet	Target
		LookAt	Target
		
	End

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target	
		Lifespan	7
	End		
	
End

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther Target	
		Lifespan	7
	End		
	
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	32

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
		Lifespan	7
	End

End

Condition
	On 	Time
	Time 	35

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	39

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
		Lifespan	7
	End

End

Condition
	On 	Time
	Time 	42

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	46
		
	Event

		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part2	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part2	:ChargedBolt01.part
		Part3	:ChargedBolt02.part
		POther  Target
		Lifespan	7
	End

End

######################################################################################

Condition
	On 	Time
	Time 	20

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part2	:ChargedBolt05.part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target	
		Lifespan	7
	End		
	
End

Condition
	On 	Time
	Time 	25

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther Target	
		Lifespan	7
	End		
	
End

Condition
	On 	Time
	Time 	28

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	33

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther  Target
		Lifespan	7
	End

End

Condition
	On 	Time
	Time 	36

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	40

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
		POther  Target
		Lifespan	7
	End

End

Condition
	On 	Time
	Time 	43

	Event
		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:ChargedBolt03.part
		Part5	:ChargedBolt04.part
		POther  Target
	End

End

Condition
	On 	Time
	Time 	46
		
	Event

		Type	start
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part2	:WitchBoltMega.Part
		Part	:WitchBolt2Mega.Part
		Part3	:ChargedBolt01.part
		Part4	:ChargedBolt02.part
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
		EName 	Thingy
		Type	local
		At	wepR
		Part1	:ElectricityControl01Witches.part
		Part2	:ElectricityControl02Witches.part
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		SOund	lightningbolt3 80 30 .76
		

	End

	Event
		EName 	Thingy1
		Type	local
		At	WepL
		Part1	:ElectricityControl01Witches.part
		Part2	:ElectricityControl02Witches.part
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		
	End


	Event
		EName 	ThingyA
		Type	local
		At	mystic
		#BHVR	:WitchLightningOffset.bhvr
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
	End

	Event
		EName 	ThingyB
		Type	local
		At	WepL
		#BHVR	:WitchLightningOffset.bhvr
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		
	End
End

Condition
	On 	Time
	Time 	15
	Event
		EName 	BurstR
		Type	local
		At	WepL
		#BHVR	:WitchLightningOffset.bhvr
		Part4	:WitchSparks.part
		
		LifeSpan	20

	End

	Event
		EName 	BurstL
		Type	local
		At	mystic
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