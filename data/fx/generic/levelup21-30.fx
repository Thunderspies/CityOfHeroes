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
		EName   Wind
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAirStreaks.bhvr
		geom	FX_Levelup2		
				
		LifeSpan	10
	End

	Event
		EName   WindFast
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		part	:LevelupLightRaysThinFaint.part
		part1	:LevelupLightRaysFaint.part
		part2	:LevelupLightRaysBaseFaint.part
		part3	:LevelupLightRaysBase2Faint.part
		#part	:LevelUpRing1.part
		#part	:LevelUpRing2.part
		LifeSpan	19
	End

	Event
		EName   WindFa
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		part	:LevelUpStars.part
		part	:LevelUpGlows.part
		LifeSpan	15
	End

	Event
		EName   pointA
		Type	local 
		At	Root
		Geom	RootToChestAdjustment
		
	End

	Event
		EName   WindFast
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:LevelUpStarsFast.part
		part	:LevelUpGlowsFast.part
		
		LifeSpan	15
	End

End

Condition
	On 	Time
	Time 	10
	
	Event
		Type	local 
		At	Root	
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:LevelUpRing1a.part
		part	:LevelUpRing2a.part
		LifeSpan	1
	End

	Event
		EName   Wind
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAirStreaks.bhvr
		geom	FX_Levelup2	#FX_TmpCtrlWisp01
						
		LifeSpan	20
	End
			
End

Condition
	On 	Time
	Time 	20

	Event
		EName   point0
		Type	local 
		At	pointA
		Altpiv	1
		
		Part	:EnergyRingFaint.part
		Part	:EnergyRing1Faint.part
		Part	:EnergyRing2Faint.part
		
		LifeSpan	40
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
		Part	:Stars.part
		Part	:ChestLevelUpRays.part
		Part	:ChestLevelUpRaysThinFaint.part
		LifeSpan	20
	End

End

Condition
	On 	Time
	Time 	2.5
	
	Event
		EName   WindFirst
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAirStreaksB.bhvr
		geom	FX_Levelup
		LifeSpan	3
	End
End

Condition
	On 	Time
	Time 	18
	
	Event
		EName   WindFast
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:LevelUpStarsFastx.part
		part	:LevelUpGlowsFastx.part
		
		LifeSpan	13
	End

End

Condition
	On 	Time
	Time 	25

	Event
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:LevelUpRing1x.part
		part	:LevelUpRing2x.part
		LifeSpan	15
	End

End


End