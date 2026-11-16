#########################################################
##	template

FxInfo

LifeSpan	15

#########################################################
####################### AUDIO ###########################
#########################################################


Condition
	On	Time
	Time	0
	

	Event
		Type	Local
		At 	Origin
		Sound kinetic4 80 80 .84
	End
End


#########################################################
#################### Explosion ########################
#########################################################

Condition
	On	cycle
	Time	5

	Event	
		Type	Local
		At	Root	
		BHVR	:RepulseScale.bhvr
		GEOM	RepulseWave
		LifeSpan	3
	End

End

Condition
	On 	Time
	Time 	0	
	
	Event
		Ename	ExplosiveForce
		Type	Start
		At	Head
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		500
			PhysForcePowerJitter	50
		End

		Lifespan	5

	End
	Event
		EName 	Prime
		Type	local
		At	Root
		Bhvr 	:ForceBombLarge2.bhvr
		Part1	:RepulsionBubbleSphere.part
		Lifespan	8		
	End

	Event
		EName 	Prime2
		Type	local 
		At	Root
		Bhvr 	:ForceBombLarge.bhvr
		Part1	:RepulsionRing3.part
		Geom	FX_ForceBubble
		Lifespan	8		
	End

End

Condition
	On 	Time
	Time 	0
	Event
		EName 	Core
		Type	Local 
		At	root
		Bhvr	Behaviors/CameraShake02.bhvr
		Part3	POWERS\EnergyBlast\Inferno.part
		Part1	POWERS\EnergyBlast\InfernorockBits.part
		Part2	POWERS\EnergyBlast\InfernorockBits2.part
	End

End

Condition
	On	Time
	Time	3
	
	Event	
		Type	Local
		At	Root
		BHVR	:RepulseRingsplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	5
	End

	Event
		EName 	Prime2
		Type	local 
		At	Root
		Bhvr 	:ForceBombLarge.bhvr
		Part1	:RepulsionRing3.part
		Geom	FX_ForceBubble
		Lifespan	8		
	End
	
	Event
		EName 	Prime
		Type	local
		At	Root
		Bhvr 	:ForceBombLarge2.bhvr
		Part1	:RepulsionBubbleSphere.part
		Lifespan	8		
	End

End

Condition
	On	Time
	Time	8

	Event	
		Type	Local
		At	Root
		BHVR	:RepulseRingsplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	5
	End

	Event
		EName 	Prime2
		Type	local 
		At	Root
		Bhvr 	:ForceBombLarge.bhvr
		Part1	:RepulsionRing3.part
		Geom	FX_ForceBubble
		Lifespan	8		
	End
	
	Event
		EName 	Prime
		Type	local
		At	Root
		Bhvr 	:ForceBombLarge2.bhvr
		Part1	:RepulsionBubbleSphere.part
		Lifespan	8		
	End

End

Condition
	On	Time
	Time	13

	Event	
		Type	Local
		At	Root
		BHVR	:RepulseRingsplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	5
	End

End

End