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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Lifespan 113
	End

	Event
		Type	local
		At	HandR
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Lifespan 113
	End
End

Condition
	On 	Time
	Time 	20

	Event
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlow.part
		Lifespan 90
	End

	Event
		Type	local
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityOuterGlow.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityArch.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityRing.part
		Lifespan 90
	End
End

Condition
	On 	Time
	Time 	26

	Event
		Type	local
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
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
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
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
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		LifeSpan 20
	End

	Event
		Type	local
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
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
		Part1	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	78

	Event
		Type	start
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	82

	Event
		Type	start
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	85

	Event
		Type	start
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	89

	Event
		Type	start
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	92

	Event
		Type	start
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	96

	Event
		Type	start
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandR
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	103

	Event
		Type	start
		At	HandR
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	106

	Event

		Type	start
		At	HandR
		#Part2	:QuickBolt01a.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	75

	Event
		Type	start
		At	HandL
		Part1	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther Target
	End
End

Condition
	On 	Time
	Time 	78

	Event
		Type	start
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	83

	Event
		Type	start
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	86

	Event
		Type	start
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End


Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	93

	Event
		Type	start
		At	HandL
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	96

	Event
		Type	start
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	100

	Event
		Type	start
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt03.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	103

	Event
		Type	start
		At	HandL
		Part2	CustomizeablePowers\ElectricalBlast\QuickBolt01.part
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt02.part
		POther  Target
	End
End

Condition
	On 	Time
	Time 	106

	Event

		Type	start
		At	HandL
		Part3	CustomizeablePowers\ElectricalBlast\QuickBolt06.part
		Part4	CustomizeablePowers\ElectricalBlast\QuickBolt05.part
		Part5	CustomizeablePowers\ElectricalBlast\QuickBolt04.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ElectricityControl01b.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricityControl02b.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricityInnerGlowa.part
		Part4	CustomizeablePowers\ElectricalBlast\ElectricityArchb.part
		Part5	CustomizeablePowers\ElectricalBlast\ElectricityArchInvertb.part
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
		Part1	CustomizeablePowers\ElectricalBlast\ShockFieldArchsa.part
		Part2	CustomizeablePowers\ElectricalBlast\ShockFieldArchs2a.part
		Part3	CustomizeablePowers\ElectricalBlast\ShockFieldArchs1a.part
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
		Bhvr	CustomizeablePowers\ElectricalBlast\ScaleBubble2.bhvr
		BhvrOverride
			TintGeom	1
		End
		Geom	Tintable_ElectricityHit
		Lifespan 4
	End

	Event
		Type	start
		At	TargetOffset
		altpiv	1
		Part1	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		Part2	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		Part3	CustomizeablePowers\ElectricalBlast\ElectricitySparkBurst.part
		Part4	CustomizeablePowers\ElectricalBlast\ShockFieldRingA.part
		lifespan 30
	End

	Event
		Type	Local
		At	Target
		Part2	CustomizeablePowers\ElectricalBlast\ShockFieldGlow.part
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