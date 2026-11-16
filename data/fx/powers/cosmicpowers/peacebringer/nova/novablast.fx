#############################################################
## Peacebringer - Dawn Strike
#############################################################

FxInfo
	
lifespan	90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	90
	End
End

#############################################################

Condition
	On	Time 
	Time	0
	
	Event
		Type	Local 
		At	hips
		Sound Nova4 100.0 100.0 .98
	End

	Event
		Ename	GravitationalForce
		Type	Local
		At	Head
		BhvrOverride
			PhysForceType		In
			PhysForceRadius		20
			PhysForcePower		25
			PhysForcePowerJitter	10
		End
		Lifespan	50
	End
End

Condition
	On 	Time
	Time 	47

	Event
		EName 	Prime
		Type	Local 
		At	hips
		Part2	:Inferno.part
		Lifespan	40
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		35
			PhysForcePower		1500
			PhysForcePowerJitter	200
		End
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	52

	Event
		EName 	Primex
		Type	Local 
		At	hips
		Bhvr	Behaviors/CameraShake01.bhvr
		Part	:NovaTendrils.part
		Part	:InfernorockBits2.part
		Lifespan	35
	End

	Event
		EName   RadiusFxScale
		Type	local
		At	Root
		Bhvr	:PeaceBringerTest.bhvr
	End

	Event
		EName 	Prime1
		Type	Local 
		At	RadiusFxScale
		bhvr    :Infernobubble.bhvr
		Part	:NovaSmoke.part
		Part	:NovaSmoke01.part
		Part	:NovaSmoke02.part
		Lifespan	35
	End		
End

Condition
	On 	Time
	Time 	50

	Event
		EName 	Prime8
		Type	Local 
		At	root
		bhvr	:BlastRingScale.bhvr
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End
End

Condition
	On 	Time
	Time 	54

	Event
		Type	Posit
		At	HIPS
		Bhvr 	Powers/cosmicpowers/PeaceBringer/Nova/BlastScaller.bhvr
		Geom	CosmicPeaceBringerShield
		LifeSpan	17
	End

	Event
		EName	ring
		Type	start
		At	RadiusFxScale
		geom	MegaRing2
		bhvr	:BlastRingScale2a.bhvr
		LifeSpan		2
	End
End

#############################################################

End