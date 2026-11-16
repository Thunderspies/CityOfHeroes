#############################################################
## FX System Name
#############################################################

FxInfo
	
Lifespan	90

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local 
		At	Origin
		BHVR	Behaviors/OffsetEyes.bhvr
		ChildFX	POWERS\CosmicPowers\Peacebringer\ActivationEyes\ActivationEyes_Child.fx
		LifeSpan	45
	End
End

#############################################################

Condition

	On	Time 
	Time	0

	Event
		Type	Local 
		At	hips
		Sound footstomp3 90.0 90.0 1.0
	End
End

#############################################################

Condition
	On 	Time
	Time 	20

	Event
		EName 	Primex
		Type	Local 
		At	Mystic
		Part	:NovaTendrils.part
		Part	:NovaTendril.part
		Part	:InfernorockBits2.part
		LifeSpan	25
	End

	Event
		Type	start
		At	Mystic
		Bhvr	:FootStompCracks_Additive.bhvr
		Splat	ImpactCracks.tga
		Lifespan	1
	End

	Event
		Type	Start
		At	Mystic
		Bhvr	:FootStompCameraShake.bhvr
		LifeSpan	10
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
	End		
End

Condition
	On 	Time
	Time 	19

	Event
		EName 	Prime8
		Type	Local 
		At	root
		bhvr	:BlastRingScale.bhvr
		Part1   :Infernoflash.part
		Part2   :Infernoflash2.part
	End
End

#############################################################

Condition
	On 	Time
	Time 	19

	Event
		Ename	BounceForce
		Type	Start
		At	Root
		BhvrOverride
			PhysForceType		Up
			PhysForceRadius		50
			PhysForcePower		500
			PhysForcePowerJitter	100
		End
		Lifespan	1
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Ename	ExplosiveForce
		Type	Start
		At	RadiusFxScale
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		45
			PhysForcePower		50
			PhysForcePowerJitter	10
		End
		Lifespan	15
	End
End

#############################################################

Condition
	On 	Time
	Time	27

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	Primex
		Type	Destroy
	End
End

Condition
	On 	Time
	Time	27

	Event
		EName 	Prime1
		Type	Destroy
	End
End	

#############################################################

End