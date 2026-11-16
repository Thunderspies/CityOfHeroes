#########################################################
##	template

FxInfo

Input  
	InpName	Root
End

Input  
	InpName	Chest
End

Input
	InpName Origin
End

Lifespan	20


###########################################################

###########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName   Wind
		Type	local 
		At	Root
		
		Sound levelup7 90 90 .86
				
		LifeSpan	1
	End

	Event
		EName   pointA
		Type	local 
		At	Root
		Geom	RootToChestAdjustment
		
	End
	
End

Condition
	On 	Time
	Time 	1

	Event
		EName   point0
		Type	local 
		At	pointA
		Altpiv	1
		Part	:Stars3.part
		Part	:EnergyRingFainta.part
		Part	:EnergyRing1Fainta.part
		Part	:EnergyRing2Fainta.part
		Part	:ChestLevelUpRays.part
		Part	:ChestLevelUpRaysThinFaint.part
		Part	:ChestLevelUpRays.part
		Part	:ChestLevelUpRaysThin.part
		LifeSpan	10
	End

End

Condition
	On 	Time
	Time 	9

	Event
		EName   point0
		Type	local 
		At	pointA
		Altpiv	1
		
		Part	:EnergyRingFainta.part
		Part	:EnergyRing1Fainta.part
		Part	:EnergyRing2Fainta.part
		
		LifeSpan	10
	End

End


End