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

Lifespan	140


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
		EName   WindFast
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		part	:LevelupLightRaysBase2FaintThin.part
		
		part2	:LevelupLightRaysBaseFaintb.part
		part3	:LevelupLightRaysBase2Faintb.part

		LifeSpan	25
	End

	Event
		EName   pointA
		Type	local 
		At	Root
		Geom	RootToChestAdjustment
		
	End
	
	Event
		Type	local 
		At	pointA
		Altpiv	1
		
		Part	:Stars1.part
		
		LifeSpan	20
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
		
		part	:LevelUpStars.part
		part	:LevelUpGlows.part
		Part	:ChestLevelUpRays.part
		Part	:ChestLevelUpRaysThinFaint.part
		LifeSpan	20
	End

End


End