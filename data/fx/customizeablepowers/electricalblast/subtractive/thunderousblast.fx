#############################################################
## ThunderousBlast.fx
#############################################################

FxInfo

LifeSpan 200

#############################################################

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
		Lifespan 113
	End

	Event
		Type	local
		At	HandR
		Part1	:ElectricityOuterGlow.part
		Part2	:ElectricityArch.part
		Part3	:ElectricityRing.part
		Lifespan 113
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

Condition
	On 	Time
	Time 	26

	Event
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .55
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	45

	Event
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		SOund	lightningbolt2 80 30 .55
		LifeSpan 20
	End
End

Condition
	On 	Time
	Time 	72

	Event
		Type	local
		At	HandR
		Part4	:ElectricitySparkBurst.part
		LifeSpan 20
	End

	Event
		Type	local
		At	HandL
		Part4	:ElectricitySparkBurst.part
		LifeSpan 20
	End
End

#############################################################

Condition
	On 	Time
	Time 	75

	Event
		Type	start
		At	HandR
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	78

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	82

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	85

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	89

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	92

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	96

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandR
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	103

	Event
		Type	start
		At	HandR
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	106

	Event

		Type	start
		At	HandR
		Part3	:QuickBolt06.part
		Part4	:QuickBolt05.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	75

	Event
		Type	start
		At	HandL
		Part1	:QuickBolt06.part
		Part2	:QuickBolt05.part
		Part3	:QuickBolt01.part
		Part4	:QuickBolt02.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	78

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	83

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	86

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End


Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	93

	Event
		Type	start
		At	HandL
		Part4	:QuickBolt03.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	96

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt03.part
		Part3	:QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	103

	Event
		Type	start
		At	HandL
		Part2	:QuickBolt01.part
		Part3	:QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	106

	Event

		Type	start
		At	HandL
		Part3	:QuickBolt06.part
		Part4	:QuickBolt05.part
		Part5	:QuickBolt04.part
		POther  Target
	End
End

#############################################################

Condition
	On 	Time
	Time 	76

	Event
		EName 	PrimeA
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
		Lifespan 15
	End
End

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
	Time 	85

	Event
		EName 	Prime2A
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
	Time 	92

	Event
		EName 	Prime2B
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
	Time 	99

	Event
		EName 	Prime2
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

#############################################################

Condition
	On 	Time
	Time 	74

	Event
		EName 	Prime3A
		Type	start
		At	HandR
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
	Time 	80

	Event
		EName 	Prime3
		Type	start
		At	HandR
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
	Time 	83

	Event
		EName 	Prime4B
		Type	start
		At	HandR
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
	Time 	93.5

	Event
		EName 	Prime4A
		Type	start
		At	HandR
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
	Time 	102

	Event
		EName 	Prime4
		Type	start
		At	HandR
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

#############################################################

Condition
	On 	PrimeHit

	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName 	PrimeA
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
		ENAME	TargetOffset
		Type	start
		At	T_Root
		geom	roottochestadjustment
		lifespan 30
	End

	Event
		EName 	Mallet
		Type	start
		At	TargetOffset
		altpiv	1
		Bhvr	:ScaleBubble2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_Dark_ElectricityHit
		Lifespan 4
	End

	Event
		Type	start
		At	TargetOffset
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

Condition
	On 	Prime2Hit

	Event
		EName 	Prime2
		Type	Destroy
	End

	Event
		EName 	Prime2A
		Type	Destroy
	End

	Event
		EName 	Prime2B
		Type	Destroy
	End
End

Condition
	On 	Prime3Hit

	Event
		EName 	Prime3
		Type	Destroy
	End

	Event
		EName 	Prime3A
		Type	Destroy
	End
End

Condition
	On 	Prime4Hit

	Event
		EName 	Prime4
		Type	Destroy
	End

	Event
		EName 	Prime4A
		Type	Destroy
	End

	Event
		EName 	Prime4B
		Type	Destroy
	End
End

#############################################################

End