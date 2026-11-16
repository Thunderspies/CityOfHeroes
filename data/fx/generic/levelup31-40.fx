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

Condition
	On 	Time
	Time 	0

	Event
		EName   Wind
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAirStreaks.bhvr
		geom	FX_Levelup2		
				
		LifeSpan	10
	End

	Event
		EName   Wind
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAirStreaks.bhvr
		geom	FX_Levelup2	#FX_TmpCtrlWisp01
		Sound levelup7 90 90 .86
				
		LifeSpan	1
	End

	Event
		EName   WindFast
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		
		part	:LevelUpStars.part
		part	:LevelUpGlows.part
		part	:LevelupLightRaysThin.part
		part1	:LevelupLightRays.part
		part2	:LevelupLightRaysBase.part
		part3	:LevelupLightRaysBase2.part
		part	:LevelUpRing1.part
		part	:LevelUpRing2.part
		LifeSpan	25
	End

	Event
		EName   pointA
		Type	local 
		At	Root
		Geom	RootToChestAdjustment
		
	End

	Event
		EName   point
		Type	local 
		At	Root
		Bhvr	Behaviors/LevelupBaseSpin.bhvr
		Geom	RootToChestAdjustment
		
	End

	Event
		EName   WindFast
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:LevelUpStarsFast.part
		part	:LevelUpGlowsFast.part
		
		LifeSpan	20
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
	Time 	1

	Event
		EName   point0
		Type	local 
		At	pointA
		Altpiv	1
		
		Part	:ChestLevelUpRays.part
		Part	:ChestLevelUpRaysThin.part
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
	Time 	3
	
	Event
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:LevelUpRing1.part
		part	:LevelUpRing2.part
		LifeSpan	15
	End

	
End

Condition
	On 	Time
	Time 	7.5
	
	Event
		EName   WindFirst
		Type	local 
		At	Root
		#Bhvr 	Behaviors/LevelupStreak.bhvr
		#geom	FX_LevelupStreak
		LifeSpan	1
	End

	
End

Condition
	On 	Time
	Time 	9.5

	Event
		EName   WindFirst
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupStreakCounter.bhvr
		geom	FX_LevelupStreak
		LifeSpan	1
	End

End

Condition
	On 	Time
	Time 	11.5

	Event
		EName   WindFirst
		Type	local 
		At	Root
		#Bhvr 	Behaviors/LevelupStreak.bhvr
		#geom	FX_LevelupStreak
		LifeSpan	20
	End

	Event
		EName   point0
		Type	local 
		At	pointA
		Altpiv	1
		Part	:Stars.part
		Part	:Stars3.part
		Part	:ChestLevelUpRays.part
		Part	:ChestLevelUpRaysThin.part
		LifeSpan	20
	End

End

#####################################
Condition
	On 	Time
	Time 	13.5

	Event
		EName   WindFirst
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupStreakCounter.bhvr
		geom	FX_LevelupStreak
		LifeSpan	1
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
	Time 	5
	
	Event
		EName   WindFirst
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAirStreaks1d.bhvr
		geom	FX_Levelup2
		LifeSpan	23
	End
End


####################################################

Condition
	On 	Time
	Time 	18
	
	Event
		EName   WindFirst
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAir.bhvr
		geom	FX_Levelup2
		LifeSpan	15
	End

	Event
		EName   WindFast
		Type	local 
		At	Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		part	:LevelUpStarsFastx.part
		part	:LevelUpGlowsFastx.part
		
		LifeSpan	23
	End

End

Condition
	On 	Time
	Time 	25

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
	Time 	22

	Event
		EName   WindFirst
		Type	local 
		At	Root
		Bhvr 	Behaviors/LevelupAir.bhvr
		geom	FX_Levelup2
		LifeSpan	18
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