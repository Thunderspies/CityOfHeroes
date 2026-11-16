#############################################################
## HEADER
#############################################################

FxInfo

LifeSpan 200

## SOUND ###########################################################

Condition
	On 	Time
	Time	0

	Event
		Type	Local
		At 	Origin
		Sound Thunderblast2 100 100 1.0
	End
End

## HANDS ###########################################################

Condition
	On 	Time
	Time 	7

	Event
		Type	local
		At	HandR
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 103
	End

	Event
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 103
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	HandL
		Part1	:ElectricityControl01.part
		Part2	:ElectricityControl02.part
		Part3	:ElectricityInnerGlow.part
		Lifespan 90
	End

	Event
		Type	local
		At	HandL
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 90
	End
End

## HANDS - SPARK BURSTS ###########################################################

Condition
	On 	Time
	Time 	26

	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		Sound lightningbolt2 80 80 .55
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName 	BurstR
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		Sound lightningbolt2 80 80 .55
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	72

	Event
		EName 	BurstR
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		LifeSpan 20
	End

	Event
		EName 	BurstL
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan 20
	End
End


## FIRE BOLTS ###########################################################

Condition
	On 	Time
	Time 	75

	Event
		Type	Local
		At	WepR
		ChildFx :Child_ElectricBeamCycle.fx
		Lifespan 30
	End
End

Condition
	On 	Time
	Time 	75

	Event
		Type	Local
		At	WepL
		ChildFx :Child_ElectricBeamCycle.fx
		Lifespan 30
	End
End

## PROJECTILES ###########################################################

Condition
	On 	Time
	Time 	79

	Event
		EName 	Prime
		Type	start
		At	HandL
		Bhvr	Behaviors\Fastprojectile6.bhvr
		Part1	:ElectricityControl01b.part
		Part2	:ElectricityControl02b.part
		Part3	:ElectricityInnerGlowa.part
		Part4	:ElectricityArchb.part
		Part5	:ElectricityArchInvertb.part
		Magnet	Target
		Lookat	Target
	End
End

Condition
	On 	Time
	Time 	76

	Event
		Type	Start
		At	HandL
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	85

	Event
		Type	Start
		At	HandL
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	92

	Event
		Type	Start
		At	HandL
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	99

	Event
		Type	Start
		At	HandL
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	74

	Event
		EName 	3
		Type	start
		At	HandR
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	80

	Event
		EName 	Prime3
		Type	start
		At	HandR
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	83

	Event
		EName 	4
		Type	start
		At	HandR
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	93.5

	Event
		EName 	5
		Type	start
		At	HandR
		ChildFX	:Child_ElectricProjectile.fx
	End
End

Condition
	On 	Time
	Time 	102

	Event
		EName 	Prime4
		Type	start
		At	HandR
		ChildFX	:Child_ElectricProjectile.fx
	End
End

## ON HIT ###########################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		Type	Local
		At	Target
		Part1	:ShockFieldArchsa.part
		Part2	:ShockFieldArchs2a.part
		Part3	:ShockFieldArchs1a.part
		lifespan 30
	End

	Event
		ENAME	ChestOffset
		Type	start
		At	T_Root
		geom	roottochestadjustment
		lifespan 30
	End

	Event
		EName 	Mallet
		Type	start
		At	ChestOffset
		altpiv	1
		bhvr	:BubbleScale01.bhvr
		Geom	GEO_WhiteSphere
		lifespan 10
	End

	Event
		Type	start
		At	ChestOffset
		altpiv	1
		Part1	:ElectricitySparkBurst.part
		Part2	:ElectricitySparkBurst.part
		Part3	:ElectricitySparkBurst.part
		Part4	:ShockFieldRingA.part
		lifespan 30
	End

	Event
		Type	Local
		At	Target
		Part2	:ShockFieldGlow.part
		lifespan 5
	End
End

#############################################################

End